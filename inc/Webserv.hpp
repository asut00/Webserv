/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asuteau <asuteau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/23 15:06:51 by asuteau           #+#    #+#             */
/*   Updated: 2024/10/09 14:54:27 by asuteau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

typedef unsigned long long ull;

#include					<stdio.h>
#include					<stdlib.h>
#include					<unistd.h>
#include					<string.h>
#include					<sys/socket.h>
#include					<netinet/in.h>
#include					<sys/wait.h>
#include					<sys/stat.h>
#include					<signal.h>
#include					<dirent.h>
#include					<fcntl.h>

#include					<iostream>
#include					<map>
#include					<sstream>
#include					<string>
#include					<fstream>
#include					<vector>
#include					<exception>
#include					<cstdlib>
#include					<algorithm>
#include					<iomanip>

#include					"Request.hpp"
#include					"ErrResponse.hpp"
#include					"PostData.hpp"
#include					"Server.hpp"
#include					"WebservManager.hpp"
#include					"Server.hpp"
#include					"HttpCode.hpp"
#include					"CGIhandler.hpp"
#include					"AutoIndex.hpp"
#include					"DeleteIndex.hpp"
#include					"SuccesAnswers.hpp"
#include					"SessionCookie.hpp"

#define DFLT				"\033[0m"
#define RED					"\033[31m"
#define GREEN				"\033[32m"
#define YELLOW				"\033[33m"
#define BLUE				"\033[34m"
#define MAGENTA				"\033[35m"
#define CYAN				"\033[36m"
#define WHITE				"\033[37m"

// TEXT COLORS
#define RED_BRIGHT			"\033[38;5;9m"    // Rouge vif
#define CYAN_CLEAR			"\033[38;5;45m"  // Cyan clair
#define MAGENTA_CLEAR		"\033[38;5;129m" // Magenta clair
#define RED_INTENSE			"\033[38;5;196m" // Rouge intense
#define ORANGE				"\033[38;5;202m" // Orange
#define YELLOW_BRIGHT		"\033[38;5;226m" // Jaune vif
#define GREEN_BRIGHT		"\033[38;5;82m"   // Vert vif
#define BLUE_BRIGHT			"\033[38;5;27m"   // Bleu vif

#define INDEX				"./pages/index.html"
#define FAVICON				"./pages/favicon/favicon.ico"
#define LEN_ID				16

#define READING_ERROR		0
#define NEED_TO_READ_AGAIN	1
#define READING_COMPLETE	2
#define FAILED				-1
#define SUCCEEDED			0
#define CHUNK_SENT			1
#define EXIT				2
#define KEEP_GOING			42


extern std::map<int, std::string> httpStatusCodes;

//init
void	initHttpStatusCode();

// utils
bool			isOnlyWhiteSpaces(const char *s);
void			replaceSubstring(std::string &str, const std::string &from, const std::string &to);
void			putbytesread(const char *s, ull bytes);
bool			is_port(std::string word);
bool			is_client_body(std::string word);
std::string		ull_to_string(ull value);
std::string		ull_to_hexa(ull value);
std::string		generateSessionId();
int				isCgiExtension(std::string url);

//session cookie
int				setSessionCookie(std::string ID, std::string preference);
std::string		generateSessionCookieAnswer(std::string ID);

//RequestSendUtils
std::string		generateDeleteIndexLine(std::string uri, std::string dirEntryName);
std::string		generateAutoIndexLine(std::string uri, std::string dirEntryName);
