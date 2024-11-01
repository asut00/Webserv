/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGIhandler.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thelaine <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/02 11:54:25 by thelaine          #+#    #+#             */
/*   Updated: 2024/10/02 11:54:26 by thelaine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Webserv.hpp"

#define PATH_TO_PYTHON "/usr/bin/python3"
#define PATH_TO_PHP 	"/usr/bin/php"
#define CGI_ANSWER 		"src/CGIhandler/answer_from_cgi"

class webservManager;

class CGIhandler
{
	public:
		CGIhandler(std::string uri, std::string root, std::string method, int cgi_type, ull content_length, std::map<std::string, std::string> 	_headers);
		~CGIhandler();
		int HandleCGIRequest(webservManager *webservManager, char *body);

	private:
		std::string _scriptName; //get it with path to cgi + request;
		std::string	_args; // get it with requests
		std::string	_pathOfExec;
		std::string	_method;
		std::string	_contentLength;
		std::string	_contentType;
		std::string	_pathInfo;
		ull			_size;
};

