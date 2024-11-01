/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asuteau <asuteau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 10:08:56 by asuteau           #+#    #+#             */
/*   Updated: 2024/10/02 18:30:23 by asuteau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Webserv.hpp"
#include "ErrResponse.hpp"


#define MAX_URI_SIZE			2048
#define END_THE_SERVER			"/terminate_serv"
#define FAVICON_REQUEST			"/favicon.ico"
#define DELETE_PAGE_REQUEST		"/method/delete.html"
#define TEST_REGULAR_COOKIE		"/cookie/regular_cookie/cookie.html"
#define TEST_SESSION_COOKIE		"/cookie/session_cookie/setSessionCookie.html"
#define TEST_SESSION_COOKIE_GET	"/cookie/session_cookie/get_session_cookie.html"
#define SIZE_OF_CHUNK			8000
#define USE_EXTENSION_FOR_CGI	1

// Pre inclusions for compilation
class PostData;
class Server;
class webservManager;

class Request
{

	public:
		Request();
		Request(int clientFd, char *data, ull bytesRead, Server *servers);
		Request(const Request &o);
		~Request();

		// Setters
		void								setErrorStatusCode(int errorstatuscode);

		// Main method

		//Request.read.cpp
		int									readRequest();
		int									readAgain(char *buffer, int recvBytesRead);

		// Request_handle.cpp
		int									handleRequest(webservManager *webservManager);

		// Request_send.cpp
		int									sendErrResponse();

	private:
		// Request_read.cpp
		int									parseFirstLine();
		bool								flIsBadRequest();
		int									parseHeaders();
		void								manageCookieID();
		bool								hasBody();
		int									parseBodyInFirstRecv();
		int									readAgainErrorCheck(int recvBytesRead_again);

		// Request_handle.cpp
		int									handleRequestInit(webservManager *webservManager);
		bool								checkIfMethodIsAllowed();
		int									handleCGI(webservManager *webservManager);
		int									deleteFirstAndLastBoundaries();

		int									handleGet();
		int									handleGetSessionCookie();

		int									handlePost();
		int									handlePostInit();
		int									setupBoundary();
		int									checksForPostMethod();
		int									handleCookie();
		int									handleSessionCookie();
		void								parsePostBodyContent();
		int									handlePostBodyContent();

		int									handleDelete();
		int									handleDeleteInit(std::string &file_path, struct stat &fileInfo);

		// Request.send.cpp
		int									sendCookieResponse(const std::string& username);
		int									readFileForResponse();
		int									readDirectory(std::string dirName, std::string createLine(std::string, std::string));
		int									sendDeleteIndex();
		int									sendAutoIndex();
		int									sendFavicon();
		int									sendResponse();
		int									sendStandard();
		int									sendFirstChunk();
		int									sendChunk();

		int									_clientFd;
		char								*_charData;
		ull									_dataLength;
		std::string							_data;
		std::istringstream					_ssdata;

		ErrResponse							_response;

		std::string							_method;
		std::string							_uri;
		std::string							_version;

		std::map<std::string, std::string>	_headers;

		char								*_charBody;
		char								*_charBodyTrimmed;
		ull									_contentLength;
		std::map<int, PostData>				_bodyParts;
		int									_numOfBodyParts;
		bool								_needBodyParsingLoop;
		ull									_recvBytesRead;

		ull									_bodyBytesAlreadyRead;

		std::string							_boundary;


		int									_errorStatusCode;
		Server								*_server;
		std::string							_httpResponse;
		bool								_chunkTransferStarted;
		std::string							_cookieID;
		int									_cgiType;
		bool								_sendCookieID;
		ull									_sizeOfTrimmedBody;

};
