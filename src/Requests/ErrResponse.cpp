/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errErrResponse.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asuteau <asuteau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 15:28:10 by asuteau           #+#    #+#             */
/*   Updated: 2024/09/24 17:20:20 by asuteau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Webserv.hpp"

/////////////////////// CANON ///////////////////////

ErrResponse::ErrResponse() : _version("HTTP/1.1"), _statusCode(0), _statusTitle()
{}

ErrResponse::~ErrResponse()
{}

/////////////////////// METHODS ///////////////////////

std::string		ErrResponse::getErrResponse(int statusCode, const Server &server, std::string cookieID, bool sendCookieID)
{
	std::string errorPageFile;
	std::map<int, std::string> serverErrorPages = server.getMapErrorPages();

	// Setup _statusCode and _statusTitle attributes
	setupErrResponseAttributes(statusCode);

	// Check if error page in config file otherwise using template
	if (serverErrorPages.find(statusCode) != serverErrorPages.end())
		errorPageFile = server.getRoot() + serverErrorPages[statusCode];
	else
		errorPageFile = ERR_PAGE_TEMPLATE ;
	// Open html template
	std::ifstream file(errorPageFile.c_str());
	if (!file.is_open())
	{
		std::cerr << RED << "Failed to open error page file: " << errorPageFile << DFLT << std::endl;
		if (statusCode == HTTP_NOT_FOUND)
			errorPageFile = ERR_PAGE_TEMPLATE;
		else
			return getErrResponse(HTTP_NOT_FOUND, server, cookieID, sendCookieID);
	}

	// Put the file content in a content string
	std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
	file.close();

	// Replace the XXXXX by the appropriate content if necessary
	if (errorPageFile == ERR_PAGE_TEMPLATE)
	{
		std::string replacement = "Error " + _statusCodeStr + ": " + _statusTitle;
		replaceSubstring(content, "XXXXX", replacement);
		replaceSubstring(content, "YYYYY", "/");
	}

	// Build the final response string
	std::string httpErrResponse;
	if (content.length() < SIZE_OF_CHUNK)
	{
		httpErrResponse =
			"HTTP/1.1 " + _statusCodeStr + " " + _statusTitle + "\r\n"
			"Content-Type: text/html; charset=UTF-8\r\n"
			"Content-Length: " + ull_to_string(content.length()) + "\r\n"
			"Connection: close\r\n";
			if (sendCookieID == true)
				httpErrResponse += "Set-Cookie: Session-ID=" + cookieID + "; Path=/; HttpOnly\r\n";
			httpErrResponse += "\r\n" + content;
	}
	else
	{
		httpErrResponse =
			"HTTP/1.1 " + _statusCodeStr + " " + _statusTitle + "\r\n"
			"Content-Type: text/html; charset=UTF-8\r\n"
			"Transfer-Encoding: chunked\r\n"
			"Connection: close\r\n";
			if (sendCookieID == true)
				httpErrResponse += "Set-Cookie: Session-ID=" + cookieID + "; Path=/; HttpOnly\r\n";
			httpErrResponse += content;
	}

	return httpErrResponse;
}

void			ErrResponse::setupErrResponseAttributes(int statusCode)
{
	_statusCode = statusCode;
	_statusCodeStr = ull_to_string(statusCode);
	setErrTitle(statusCode);
}

void	ErrResponse::setErrTitle(int statusCode)
{
	if (httpStatusCodes.find(statusCode) != httpStatusCodes.end())
	{
		_statusTitle = httpStatusCodes[statusCode];
		return ;
	}
	std::cerr << RED << "Error: Error status code |" << statusCode << "| not found" << DFLT << std::endl;
}
