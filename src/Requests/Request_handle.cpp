/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request_handle.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asuteau <asuteau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 10:09:02 by asuteau           #+#    #+#             */
/*   Updated: 2024/10/09 15:00:14 by asuteau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Webserv.hpp"

int	Request::handleRequest(webservManager *webservManager)
{
	// Initial checks and setups
	int ret = handleRequestInit(webservManager);
	if (ret != KEEP_GOING)
		return ret;

	if (_method == "GET")
		return handleGet();
	else if (_method == "POST")
		return handlePost();
	else if (_method == "DELETE")
		return handleDelete();
	_errorStatusCode = HTTP_METHOD_NOT_ALLOWED;
	return sendErrResponse();
}

////////////////////////////////// handleRequestInit //////////////////////////////////

int		Request::handleRequestInit(webservManager *webservManager)
{
	if (_chunkTransferStarted == true)
		return sendResponse();
	if (_errorStatusCode != HTTP_OK)
		return sendErrResponse();
	if (_uri == END_THE_SERVER)
		return EXIT;
	if (checkIfMethodIsAllowed() == false)
	{
		_errorStatusCode = HTTP_METHOD_NOT_ALLOWED;
		return sendErrResponse();
	}
	if (USE_EXTENSION_FOR_CGI)
		_cgiType = isCgiExtension(_uri);
	else
		_cgiType = _server->getCgiType();
	if (_cgiType != NONE)
	{
		int ret = handleCGI(webservManager);
		unlink(CGI_ANSWER);
		return ret;
	}
	return KEEP_GOING;
}

bool	Request::checkIfMethodIsAllowed()
{
	const bool *methodsAllowed = _server->getAllowed();
	int	methodType;
	if (_method == "GET")
		methodType = GET;
	if (_method == "POST")
		methodType = POST;
	if (_method == "DELETE")
		methodType = DELETE;
	return methodsAllowed[methodType];
}

int		Request::handleCGI(webservManager *webservManager)
{
	if (_method == "POST")
	{
		int ret_check = checksForPostMethod();
		if (ret_check != SUCCEEDED)
			return ret_check;
		if (setupBoundary() == FAILED || deleteFirstAndLastBoundaries() == FAILED)
		{
			_errorStatusCode = HTTP_BAD_REQUEST;
			return sendErrResponse();
		}
	}
	std::string root = _server->getRoot();
	CGIhandler cgi(_uri, root, _method, _cgiType, _sizeOfTrimmedBody + 1, _headers);
	_errorStatusCode = cgi.HandleCGIRequest(webservManager, _charBodyTrimmed);
	if (_errorStatusCode != HTTP_OK)
		return sendErrResponse();
	_uri = CGI_ANSWER;
	return readFileForResponse();
}

int 	Request::deleteFirstAndLastBoundaries()
{
	std::string boundary = _boundary;
	boundary = "--" + _boundary;
	char *partStart = _charBody + boundary.length() + 2;
	char *partEnd = (char *)memmem(partStart, _contentLength, (boundary + "--").c_str(), (boundary + "--").length());
	if (partEnd == NULL)
	{
		_errorStatusCode = HTTP_BAD_REQUEST;
		return FAILED;
	}
	_sizeOfTrimmedBody = partEnd - partStart;
	_charBodyTrimmed = new char[_sizeOfTrimmedBody + 1];
	memcpy(_charBodyTrimmed, partStart, _sizeOfTrimmedBody);
	_charBodyTrimmed[_sizeOfTrimmedBody] = '\0';
	return SUCCEEDED;
}

////////////////////////////////// handleGET //////////////////////////////////

int		Request::handleGet()
{
	if (_uri == FAVICON_REQUEST)
		return sendFavicon();
	else if (_uri == TEST_SESSION_COOKIE_GET) // /!\ garder ce nom avec "TEST" dedans ?
		return handleGetSessionCookie();
	else if (_uri == DELETE_PAGE_REQUEST)
		return sendDeleteIndex();
	return readFileForResponse();
}

int		Request::handleGetSessionCookie()
{
	_httpResponse = generateSessionCookieAnswer(_cookieID);
	if (_httpResponse == "")
	{
		_errorStatusCode = HTTP_INTERNAL_SERVER_ERROR;
		return sendErrResponse();
	}
	return sendResponse();
}

////////////////////////////////// handlePost //////////////////////////////////

int		Request::handlePost()
{
	// Initial checks
	int ret = handlePostInit();
	if (ret != KEEP_GOING)
		return ret;

	// Parse body and create PostData instances
	parsePostBodyContent();

	// Upload post content or other actions
	ret = handlePostBodyContent();
	if (ret != KEEP_GOING)
		return ret;

	// Send Response
	return sendResponse();
}

int		Request::handlePostInit()
{
	if (_uri == TEST_REGULAR_COOKIE)
		return handleCookie();
	else if (_uri == TEST_SESSION_COOKIE)
		return handleSessionCookie();
	if (checksForPostMethod() == FAILED || setupBoundary() == FAILED)
		return sendErrResponse();
	return KEEP_GOING;
}

int		Request::setupBoundary()
{
	ull	colonPos;
	colonPos = _headers["Content-Type"].find("boundary=");
	if (colonPos == std::string::npos)
	{
		_errorStatusCode = HTTP_BAD_REQUEST;
		return FAILED;
	}
	_boundary = _headers["Content-Type"].substr(colonPos + 9);
	return SUCCEEDED;
}

int 	Request::checksForPostMethod()
{
	if (_headers.find("Content-Type") == _headers.end())
	{
		std::cerr << ORANGE << "Warning : post request without content type" << DFLT << std::endl;
		_errorStatusCode = HTTP_BAD_REQUEST;
		return FAILED;
	}
	if (_headers["Content-Type"].find("multipart/form-data") == std::string::npos)
	{
		std::cerr << ORANGE << "Warning : post request content type must be multiform data" << DFLT << std::endl;
		_errorStatusCode = HTTP_UNPROCESSABLE_ENTITY;
		return FAILED;
	}
	if (!hasBody())
	{
		std::cerr << "Error : post request without body" << std::endl;
		_errorStatusCode = HTTP_BAD_REQUEST;
		return FAILED;
	}
	return SUCCEEDED;
}

int		Request::handleCookie()
{
	int ret_check = checksForPostMethod();
	if (ret_check != SUCCEEDED)
		return ret_check;
	if (setupBoundary() == FAILED || deleteFirstAndLastBoundaries() == FAILED)
	{
		_errorStatusCode = HTTP_BAD_REQUEST;
		return sendErrResponse();
	}
	std::string content(_charBodyTrimmed);
	size_t end_header = content.find("\r\n\r\n");
	if (end_header == std::string::npos)
	{
		_errorStatusCode = HTTP_BAD_REQUEST;
		return sendErrResponse();
	}
	std::string username = content.substr(end_header + 4);
	size_t find_end_request = username.find("\r\n");
	if (find_end_request == std::string::npos)
	{
		_errorStatusCode = HTTP_BAD_REQUEST;
		return sendErrResponse();
	}
	return sendCookieResponse(username.substr(0, find_end_request));
}

int		Request::handleSessionCookie()
{
	int ret_check = checksForPostMethod();
	if (ret_check != SUCCEEDED)
		return ret_check;
	if (setupBoundary() == FAILED || deleteFirstAndLastBoundaries() == FAILED)
	{
		_errorStatusCode = HTTP_BAD_REQUEST;
		return sendErrResponse();
	}
	std::string content(_charBodyTrimmed);
	size_t end_header = content.find("\r\n\r\n");
	if (end_header == std::string::npos)
	{
		_errorStatusCode = HTTP_BAD_REQUEST;
		return sendErrResponse();
	}
	std::string preference = content.substr(end_header + 4);
	size_t find_end_request = preference.find("\r\n");
	if (find_end_request == std::string::npos)
	{
		_errorStatusCode = HTTP_BAD_REQUEST;
		return sendErrResponse();
	}
	if (setSessionCookie(_cookieID, preference.substr(0, find_end_request)) == FAILED)
	{
		_errorStatusCode = HTTP_INTERNAL_SERVER_ERROR;
		return sendErrResponse();
	}
	_httpResponse = COOKIE_SUCCESS_ANSWER;
	return sendResponse();
}

void	Request::parsePostBodyContent()
{
	std::string boundary = _boundary;
	boundary = "--" + _boundary;
    ull	bytesParsedFromBody = 0;
	int i = 0;
	bool finished = false;

	// Init start and end
	char *partStart = _charBody + strlen(boundary.c_str()) + 2;
	bytesParsedFromBody = partStart - _charBody;
	char *partEnd = partStart;
	ull partLength = 0;

	while (!finished)
	{
		// Update end at every iteration
		partEnd = strstr(partStart, boundary.c_str());

		// If we encountered a boundary with "--" then it's the last part
		if (partEnd == strstr(partStart, (boundary + "--").c_str()))
			finished = true;

		// If strstr returned something, we are in a part
		if (partEnd)
		{
			bytesParsedFromBody = partEnd - _charBody;
			partEnd = partEnd - 2;
		}

		// If strstr returned NULL, then we are in the last part and we must relay on contentLength
		else if (!partEnd)
		{
			partEnd = partStart + _contentLength - bytesParsedFromBody - 6; // getting rid of several \r\n
			finished = true;
		}

		// Get the length of the part
		partLength = partEnd - partStart;

		// We create a PostData instance from the part we parsed
		_bodyParts[i] = PostData(partStart, partLength, boundary, _uri, _server->getRoot());

		// Update start for the next iteration
		partStart = partEnd + 2 + strlen(boundary.c_str()) + 2;

		i++;
	}
	_numOfBodyParts = i;
}

int		Request::handlePostBodyContent()
{
	for (int i = 0; i < _numOfBodyParts; i++)
	{
		int ret_handlepost = _bodyParts[i].handlePostData();
		if (ret_handlepost != HTTP_OK)
		{
			_errorStatusCode = ret_handlepost;
			delete[] _charBody;
			_charBody = 0;
			return sendErrResponse();
		}
	}
	delete[] _charBody;
	_charBody = 0;
	_httpResponse = POST_SUCCESS_ANSWER;
	return KEEP_GOING;
}

////////////////////////////////// handleDelete //////////////////////////////////

int		Request::handleDelete()
{
	std::string file_path = _server->getRoot() + _uri;
	struct stat fileInfo;

	int ret = handleDeleteInit(file_path, fileInfo);
	if (ret != KEEP_GOING)
		return ret;

	// Delete file
	if (unlink(file_path.c_str()) == FAILED)
	{
		std::cerr << "Error : couldn't delete file" << std::endl;
		_errorStatusCode = HTTP_UNAUTHORIZED;
		return sendErrResponse();
	}

	// Finishers
	_httpResponse = DELETE_SUCCESS_ANSWER;
	std::cout << GREEN << "DELETE request was successfull" << DFLT << std::endl;
	return sendResponse();
}

int		Request::handleDeleteInit(std::string &file_path, struct stat &fileInfo)
{
	// Check if path exists
	if (stat(file_path.c_str(), &fileInfo) != 0)
	{
		// Replace "%20" by space
		size_t startPos = 0;
		while ((startPos = file_path.find("%20", startPos)) != std::string::npos) {
			file_path.replace(startPos, 3, " ");
			startPos++;
		}
		// Check if file is still not found
		if (stat(file_path.c_str(), &fileInfo) != 0)
		{
			std::cerr << RED << "Failed to access file : " << file_path << " sending a 404 response" << DFLT << std::endl;
			_errorStatusCode = HTTP_NOT_FOUND;
			return sendErrResponse();
		}
	}

	// Check if server can access it / if file is readable
	if (!(fileInfo.st_mode & S_IWUSR))
	{
		std::cerr << "Access to file is forbidden, sending error 403 response" << std::endl;
		_errorStatusCode = HTTP_FORBIDDEN;
		return sendErrResponse();
	}

	// Check if it is a dir
	if (fileInfo.st_mode & S_IFDIR)
	{
		if (rmdir(file_path.c_str()) == FAILED)
		{
			_errorStatusCode = HTTP_UNAUTHORIZED;
			std::cerr << "Error : couldn't delete repertory " << file_path << " : ";
			// perror("");
			return sendErrResponse();
		}
		_httpResponse = DELETE_SUCCESS_ANSWER;
		std::cout << GREEN << "DELETE request was successfull" << DFLT << std::endl;
		return sendResponse();

	}

	// Check if the file is a regular file
	else if (!S_ISREG(fileInfo.st_mode))
	{
		std::cerr << "The path is not a regular file, sending error 404 response" << std::endl;
		_errorStatusCode = HTTP_NOT_FOUND;
		return sendErrResponse();
	}

	return KEEP_GOING;
}
