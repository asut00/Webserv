/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request_read.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asuteau <asuteau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 10:09:02 by asuteau           #+#    #+#             */
/*   Updated: 2024/10/09 15:00:14 by asuteau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Webserv.hpp"

/////////////////////// CANON ///////////////////////

Request::Request() :
_clientFd(-1), _charData(NULL), _data(), _ssdata(), _charBody(0), _charBodyTrimmed(0), _contentLength(0),
_needBodyParsingLoop(false), _recvBytesRead(0), _bodyBytesAlreadyRead(0),
_boundary(""), _errorStatusCode(HTTP_OK), _server(0),
_httpResponse(""), _chunkTransferStarted(false), _cookieID(""), _cgiType(NONE),
_sendCookieID(false)
{
}

Request::Request(int clientFd, char *data, ull recvBytesRead, Server *server) :
_clientFd(clientFd), _charData(data), _data(data), _ssdata(_data), _charBody(0), _charBodyTrimmed(0), _contentLength(0),
_needBodyParsingLoop(false), _recvBytesRead(recvBytesRead), _bodyBytesAlreadyRead(0),
_boundary(""), _errorStatusCode(HTTP_OK), _server(server),
_httpResponse(""), _chunkTransferStarted(false), _cookieID(""), _cgiType(NONE),
_sendCookieID(false)
{
}

Request::Request(const Request &o) :
_clientFd(o._clientFd), _charData(o._charData), _data(o._data), _ssdata(_data), _charBody(0), _charBodyTrimmed(0), _contentLength(o._contentLength),
_needBodyParsingLoop(false), _recvBytesRead(o._recvBytesRead), _bodyBytesAlreadyRead(o._bodyBytesAlreadyRead),
_boundary(o._boundary), _errorStatusCode(o._errorStatusCode), _server(o._server),
_httpResponse(o._httpResponse), _chunkTransferStarted(o._chunkTransferStarted), _cookieID(o._cookieID), _cgiType(o._cgiType),
_sendCookieID(o._sendCookieID)
{
}

Request::~Request()
{
	if (_charBody)
		delete[] _charBody;
	if (_charBodyTrimmed)
		delete[] _charBodyTrimmed;
}


/////////////////////// SETTERS ///////////////////////

void	Request::setErrorStatusCode(int errorstatuscode)
{
	_errorStatusCode = errorstatuscode;
}


////////////////////// READ FIRST REQUEST //////////////////////

int	Request::readRequest()
{
	if (parseFirstLine() == FAILED)
		return READING_COMPLETE;
	else if (parseHeaders() == FAILED)
	{
		_errorStatusCode = HTTP_BAD_REQUEST;
		return READING_COMPLETE;
	}
	manageCookieID();
	if (hasBody())
	{
		if (parseBodyInFirstRecv() == FAILED)
			return READING_COMPLETE;
		if (_contentLength > _server->getMaxClientBody())
		{
			_errorStatusCode = HTTP_PAYLOAD_TOO_LARGE;
			return READING_COMPLETE;
		}
		if (_needBodyParsingLoop)
			return NEED_TO_READ_AGAIN;
	}
	return READING_COMPLETE;
}

int		Request::parseFirstLine()
{
	_ssdata >> _method >> _uri >> _version;
	if (flIsBadRequest())
	{
		std::cerr << "Request parsing error: Wrong first line"<< std::endl;
		return FAILED;
	}
	_server = _server->goToActualSublocation(_uri);
	return SUCCEEDED;
}

bool	Request::flIsBadRequest()
{
	if (!_method.size() || !_uri.size() || !_version.size())
	{
		_errorStatusCode = HTTP_BAD_REQUEST;
		return true;
	}
	else if (_uri.length() > MAX_URI_SIZE)
	{
		_errorStatusCode = HTTP_URI_TOO_LONG;
		return true;
	}
	else if (!(_method == "GET" ||  _method == "POST" || _method == "DELETE"))
	{
		_errorStatusCode = HTTP_METHOD_NOT_ALLOWED;
		return true;
	}
	else if (!(_version == "HTTP/1.0" || _version == "HTTP/1.1"))
	{
		_errorStatusCode = HTTP_VERSION_NOT_SUPPORTED;
		return true;
	}
	return false;
}

int		Request::parseHeaders()
{
	std::string		line;
	ull				colonPos;
	std::string		key;
	std::string		value;
	int				i = 0;

	while (std::getline(_ssdata, line) && !line.empty())
	{
		if (i == 0 && isOnlyWhiteSpaces(line.c_str()))
			continue ;
		else if (i != 0 && isOnlyWhiteSpaces(line.c_str()))
			break ;
		colonPos = line.find(':');
		if (colonPos == std::string::npos)
			return FAILED;
		key = line.substr(0, colonPos);
		value = line.substr(colonPos + 1);
		value.erase(0, value.find_first_not_of(" \t"));
		value = value.substr(0, value.find_first_of("\n\r"));
		_headers[key] = value;
		i++;
	}
	return SUCCEEDED;
}

bool	Request::hasBody()
{
	return _headers.find("Content-Length") != _headers.end();
}

void Request::manageCookieID()
{
	if (_headers.find("Cookie") != _headers.end())
	{
		std::string cookieToFind = "Session-ID=";
		std::string cookie = _headers["Cookie"];
		size_t startCookieID = cookie.find("Session-ID");
		if (startCookieID != std::string::npos)
		{
			_cookieID = cookie.substr(startCookieID + cookieToFind.length(), LEN_ID);
			return;
		}
	}
	else
	{
		_cookieID = generateSessionId();
		_sendCookieID	= true;
	}
}

int	Request::parseBodyInFirstRecv()
{
	// Get Start of body
	char *startOfBody = strstr(_charData, "\r\n\r\n") + 4;
	if (startOfBody == NULL)
	{
		_errorStatusCode = HTTP_BAD_REQUEST;
		return FAILED;
	}

	// We init headersize and content length
	ull	headersSize = startOfBody - _charData;
	_contentLength = atol(_headers["Content-Length"].c_str());

	// We save the body bytes we have read
	_bodyBytesAlreadyRead = _recvBytesRead - headersSize;

	// Allocating and copying to charBody
	_charBody = new char[_bodyBytesAlreadyRead + 1];
	memcpy(_charBody, startOfBody, _bodyBytesAlreadyRead);

	// Check if complete or not
	if (_contentLength > _bodyBytesAlreadyRead || headersSize + _contentLength > BUFFER_EXCHANGE)
		_needBodyParsingLoop = true;
	else
		_charBody[_bodyBytesAlreadyRead] = '\0';

	return SUCCEEDED;
}


////////////////////// READ AGAIN //////////////////////

int		Request::readAgain(char *buffer, int recvBytesRead_again)
{
	int ret = readAgainErrorCheck(recvBytesRead_again);
	if (ret != KEEP_GOING)
		return ret;

	// Allocating and copying old charBody to new charBody
	char *newCharBody = new char[recvBytesRead_again + _bodyBytesAlreadyRead + 1];
	memcpy(newCharBody, _charBody, _bodyBytesAlreadyRead);

	// Copying data read by the last recv
	char *joiningPoint = newCharBody + _bodyBytesAlreadyRead;
	memcpy(joiningPoint, buffer, recvBytesRead_again);
	_bodyBytesAlreadyRead += recvBytesRead_again;

	// Deleteing and replacing old charBody
	delete []_charBody;
	_charBody = 0;
	_charBody = newCharBody;

	// Checking if reading is done or if we need to read again
	if (_bodyBytesAlreadyRead == _contentLength)
	{
		_charBody[_contentLength] = '\0';
		return READING_COMPLETE;
	}
	else if (_bodyBytesAlreadyRead < _contentLength)
		return NEED_TO_READ_AGAIN;
	else
	{
		_errorStatusCode = HTTP_BAD_REQUEST;
		return READING_COMPLETE;
	}
}

int		Request::readAgainErrorCheck(int recvBytesRead_again) {
		// Check if content length
	if (_headers.find("Content-Length") == _headers.end())
	{
		std::cerr << "Request parsing error: Missing Content-Length header for POST method" << std::endl;
		_errorStatusCode = HTTP_LENGTH_REQUIRED;
		return READING_COMPLETE;
	}

	// Check if recv negative
	if (recvBytesRead_again < 0)
	{
		std::cerr << "Error reading body data from client" << std::endl;
		_errorStatusCode = HTTP_INTERNAL_SERVER_ERROR;
		return READING_COMPLETE;
	}

	// Check if recv has read
	if (recvBytesRead_again == 0)
	{
		//CONNEXION WAS CLOSED BEFORE ALL CONTENT WAS RECEIVED
		std::cerr << "Client closed connection before complete body was received" << std::endl;
		return READING_ERROR;
	}

	return KEEP_GOING;
}
