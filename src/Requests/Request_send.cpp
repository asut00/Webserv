/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request_send.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asuteau <asuteau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 10:09:02 by asuteau           #+#    #+#             */
/*   Updated: 2024/10/09 15:00:14 by asuteau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Webserv.hpp"

///////////////////////// RESPONSES /////////////////////////

int		Request::sendCookieResponse(const std::string& username)
{
	_httpResponse =	"HTTP/1.1 200 OK\r\n";
	_httpResponse += "Set-Cookie: username=" + username + "; Max-Age=120; Path=/; SameSite=Strict\r\n";
	_httpResponse += "Content-Type: text/html\r\n";
	_httpResponse += "Connection: close\r\n\r\n";
	if (_sendCookieID == true)
			_httpResponse += "Set-Cookie: Session-ID=" + _cookieID + "; Path=/; HttpOnly\r\n";
	_httpResponse += COOKIE_SUCCESS_ANSWER;
	int returnSend = send(_clientFd, _httpResponse.c_str(), _httpResponse.length(), 0);
	if (returnSend <= 0)
	{
		if (returnSend == FAILED)
			std::cerr << RED <<  "Error: send function failed" << DFLT << std::endl;
		else if (returnSend == 0)
			std::cerr << RED <<  "Error: connection was closed, send function failed" << DFLT << std::endl;
		return FAILED;
	}
	return SUCCEEDED;
}

int	Request::readFileForResponse()
{
	struct stat	fileInfo;
	std::string	fullPath = _server->getRoot();
	if (_uri[_uri.length() - 1] == '/')
		fullPath +=  "/" + _server->getDefaultRequest();
	else if (_uri == CGI_ANSWER)
		fullPath = _uri;
	else
		fullPath += _uri;
	if (stat(fullPath.c_str(), &fileInfo) != 0)
	{
		std::cerr << RED << "Failed to access file : " << fullPath << " sending a 404 response" << DFLT << std::endl;
		_errorStatusCode = HTTP_NOT_FOUND;
		return sendErrResponse();
	}
	if (!(fileInfo.st_mode & S_IRUSR)) //check if user can access the file
	{
		std::cerr << RED << "Access to file is forbidden, sending error 403 response" << DFLT << std::endl;
		_errorStatusCode = HTTP_FORBIDDEN;
		return sendErrResponse();
	}
	if (!(fileInfo.st_mode & S_IROTH)) //check if other can access the file
	{
		std::cerr << RED << "Access to file is unauthorized, sending error 401 response" << DFLT << std::endl;
		_errorStatusCode = HTTP_UNAUTHORIZED;
		return sendErrResponse();
	}
	if (fileInfo.st_mode & S_IFDIR) // check if the file is in fact a dir
	{
		if (_server->getAutoIndex() == true)
			return sendAutoIndex();
		_errorStatusCode = HTTP_FORBIDDEN;
		return sendErrResponse();
	}
	if (!S_ISREG(fileInfo.st_mode)) // check if it is a regular file
	{
		std::cerr << RED << "The path is not a regular file, sending error 404 response" << DFLT << std::endl;
		_errorStatusCode = HTTP_NOT_FOUND;
		return sendErrResponse();
	}
	std::ifstream	file(fullPath.c_str());
	if (!file.is_open())
	{
		std::cerr << RED << "Failed to open page file, sending error 404 response" << DFLT << std::endl;
		_errorStatusCode = HTTP_NOT_FOUND;
		return sendErrResponse();
	}
	std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
	file.close();
	_httpResponse = content;
	return sendResponse();
}

int	Request::readDirectory(std::string dirName, std::string createLine(std::string, std::string))
{
	DIR *dir = opendir(dirName.c_str());
	if (dir == NULL)
	{
		std::cerr << RED << "Error: could not open " << dirName << DFLT << std::endl;
		_errorStatusCode = HTTP_NOT_FOUND;
		return FAILED;
	}
	for (struct dirent *dirEntry = readdir(dir); dirEntry; dirEntry = readdir(dir))
	{
		std::string dirEntryName =  std::string(dirEntry->d_name);
		if (dirEntryName[0] != '.')
			_httpResponse += createLine(_uri, dirEntryName);
	}
	closedir(dir);
	return SUCCEEDED;
}

int	Request::sendAutoIndex()
{
	std::string dirName(_server->getRoot() + _uri);
	_httpResponse = AUTO_INDEX_PAGE + _uri + "🤵‍♂️</h1>\
		<div class=\"buttons\">";
	if (readDirectory(dirName, generateAutoIndexLine) == FAILED)
		return sendErrResponse();
	_httpResponse += AUTO_INDEX_END;
	return sendResponse();
}

int	Request::sendDeleteIndex()
{
	std::string dirName(_server->getRoot() + "/post");
	_httpResponse = DELETE_INDEX_START;
	if (readDirectory(dirName, generateDeleteIndexLine) == FAILED)
		return sendErrResponse();
	_httpResponse += DELETE_INDEX_END;
	return sendResponse();
}

int	Request::sendFavicon()
{
	std::string faviconPath = _server->getRoot() + _server->getFavicon();
	std::ifstream file(faviconPath.c_str(), std::ios::binary);
	if (!file.is_open())
	{
		std::cerr << RED << "Failed to open favicon file." << DFLT << std::endl;
		_errorStatusCode = HTTP_NOT_FOUND;
		sendErrResponse();
		return FAILED;
	}
	file.seekg(0, std::ios::end);
	std::streamsize fileSize = file.tellg();
	file.seekg(0, std::ios::beg);
	std::vector<char> buffer(fileSize);
	if (!file.read(buffer.data(), fileSize))
	{
		std::cerr << RED << "Failed to read favicon file." << DFLT << std::endl;
		file.close();
		_errorStatusCode = HTTP_FORBIDDEN;
		return sendErrResponse();
	}
	file.close();
	_httpResponse =
		"HTTP/1.1 200 OK\r\n"
		"Content-Type: image/x-icon\r\n"
		"Content-Length: " + ull_to_string(fileSize) + "\r\n"
		"Connection: close\r\n"
		"\r\n";
	int returnSend = send(_clientFd, _httpResponse.c_str(), _httpResponse.length(), 0);
	if (returnSend <= 0)
	{
		if (returnSend == FAILED)
			std::cerr << RED <<  "Error: send function failed" << DFLT << std::endl;
		else if (returnSend == 0)
			std::cerr << RED <<  "Error: connection was closed, send function failed" << DFLT << std::endl;
		return FAILED;
	}
	returnSend = send(_clientFd, buffer.data(), buffer.size(), 0);
	if (returnSend <= 0)
	{
		if (returnSend == FAILED)
			std::cerr << RED <<  "Error: send function failed" << DFLT << std::endl;
		else if (returnSend == 0)
			std::cerr << RED <<  "Error: connection was closed, send function failed" << DFLT << std::endl;
		return FAILED;
	}
	return SUCCEEDED;
}

int	Request::sendErrResponse()
{
	_httpResponse = _response.getErrResponse(_errorStatusCode, *_server, _cookieID, _sendCookieID);
	return sendResponse();
}


int		Request::sendStandard()
{
	if (_errorStatusCode == HTTP_OK) // Need to had the header if it is not an error
		{
			std::string header = "HTTP/1.1 200 OK\r\n";
			if (_sendCookieID == true)
				header += "Set-Cookie: Session-ID=" + _cookieID + "; Path=/; HttpOnly\r\n";
			header += "Content-Type: text/html; charset=UTF-8\r\n"
			"Content-Length: " + ull_to_string(_httpResponse.length()) + "\r\n"
			"Connection: keep-alive\r\n";
			_httpResponse = header + "\r\n" + _httpResponse;
		}

		// Send the response
		int returnSend = send(_clientFd, _httpResponse.c_str(), _httpResponse.length(), 0);
		if (returnSend <= 0)
		{
			if (returnSend == FAILED)
				std::cerr << RED <<  "Error: send function failed" << DFLT << std::endl;
			else if (returnSend == 0)
				std::cerr << RED <<  "Error: connection was closed, send function failed" << DFLT << std::endl;
			return FAILED;
		}
		if (_errorStatusCode == HTTP_OK)
			return SUCCEEDED;
		return FAILED;
}

int		Request::sendFirstChunk()
{
	std::string chunk_header;
		if (_errorStatusCode != HTTP_OK)
		{
			int end_header = _httpResponse.find("\r\n\r\n") + 4;
			chunk_header = _httpResponse.substr(0, end_header);
			_httpResponse.erase(0, end_header);
		}
		else
		{
		chunk_header = "HTTP/1.1 200 OK\r\n";
		if (_sendCookieID == true)
			chunk_header += "Set-Cookie: Session-ID=" + _cookieID + "; Path=/; HttpOnly\r\n";
		chunk_header += "Content-Type: text/html; charset=UTF-8\r\n"
		"Transfer-Encoding: chunked\r\n"
		"Connection: keep-alive\r\n\r\n";
		}
		int returnSend = send(_clientFd, chunk_header.c_str(), chunk_header.length(), 0);
		if (returnSend <= 0)
		{
			if (returnSend == FAILED)
				std::cerr << RED <<  "Error: send function failed" << DFLT << std::endl;
			else if (returnSend == 0)
				std::cerr << RED <<  "Error: connection was closed, send function failed" << DFLT << std::endl;
			return FAILED;
		}
		_chunkTransferStarted = true;
		return CHUNK_SENT;
}

int		Request::sendChunk()
{
	size_t chunkSize = std::min((size_t)SIZE_OF_CHUNK, _httpResponse.length());
	std::string chunk = ull_to_hexa(chunkSize) + "\r\n";
	chunk += _httpResponse.substr(0, chunkSize) + "\r\n";
	int returnSend = send(_clientFd, chunk.c_str(), chunk.length(), 0);
	if (returnSend <= 0)
	{
		if (returnSend == FAILED)
			std::cerr << RED <<  "Error: send function failed" << DFLT << std::endl;
		else if (returnSend == 0)
			std::cerr << RED <<  "Error: connection was closed, send function failed" << DFLT << std::endl;
		return FAILED;
	}
	_httpResponse.erase(0, chunkSize);
	if (_httpResponse.empty())
	{
		returnSend = send(_clientFd, "0\r\n\r\n", 5, 0);
		if (returnSend <= 0)
		{
			if (returnSend == FAILED)
				std::cerr << RED <<  "Error: send function failed" << DFLT << std::endl;
			else if (returnSend == 0)
				std::cerr << RED <<  "Error: connection was closed, send function failed" << DFLT << std::endl;
			return FAILED;
		}
		if (_errorStatusCode != HTTP_ACCEPTED)
			return FAILED;
		return SUCCEEDED;
	}
	return CHUNK_SENT;
}


int		Request::sendResponse()
{
	if (_httpResponse.length() <= SIZE_OF_CHUNK && _chunkTransferStarted == false) // NO_NEED TO CHUNK
		return sendStandard();
	else if (_httpResponse.length() > SIZE_OF_CHUNK &&_chunkTransferStarted == false) // HEADER FOR CHUNK SENT
		return sendFirstChunk();
	else // sending chunks;
		return sendChunk();
}
