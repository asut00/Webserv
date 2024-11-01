/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ManageSessionCookie.cpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asuteau <asuteau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 15:19:07 by asuteau           #+#    #+#             */
/*   Updated: 2024/10/02 18:54:41 by asuteau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Webserv.hpp"

std::string generateSessionId()
{
	std::string sessionId;
	static const char alphanum[] =
		"0123456789"
		"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
		"abcdefghijklmnopqrstuvwxyz";
	int len = LEN_ID;

	srand((unsigned)time(0));
	for (int i = 0; i < len; ++i)
		sessionId += alphanum[rand() % (sizeof(alphanum) - 1)];
	return sessionId;
}

int setSessionCookie(std::string ID, std::string preference)
{
	std::ifstream file(SESSION_COOKIE);
	std::map<std::string, std::string> map;
	if (!file.is_open())
	{
		std::cerr << RED <<  "file \'" << SESSION_COOKIE << "\' cant be opened" << DFLT << std::endl;
		return FAILED;
	}
	std::string line;
	std::getline(file, line);
	while (std::getline(file, line))
	{
		if (line.length() < LEN_ID + 2)
		{
			std::cerr << RED <<  "file \'" << SESSION_COOKIE << "\' has been corrupted" << DFLT << std::endl;
			file.close();
			return FAILED;
		}
		std::string newID = line.substr(0, LEN_ID);
		std::string newPreference = line.substr(LEN_ID + 1);
		map.insert(std::pair<std::string, std::string>(newID, newPreference));
	}
	file.close();

	map[ID] = " " + preference;

	std::ofstream stream;
	stream.open(SESSION_COOKIE, std::ios::out | std::ios::trunc);

	if (!stream.is_open())
	{
		std::cerr << RED <<  "file \'" << SESSION_COOKIE << "\' cant be opened" << DFLT << std::endl;
		return FAILED;
	}
	stream << "user_id | Preference" << std::endl;
	for (std::map<std::string, std::string>::iterator it = map.begin(); it != map.end(); it++)
		stream << it->first + it->second << std::endl;
	stream.close();
	return SUCCEEDED;
}

std::string generateSessionCookieAnswer(std::string ID)
{
	std::ifstream file(SESSION_COOKIE);
	std::map<std::string, std::string> map;
	if (!file.is_open())
	{
		std::cerr << RED <<  "file \'" << SESSION_COOKIE << "\' cant be opened" << DFLT << std::endl;
		return "";
	}
	std::string line;
	std::getline(file, line);
	while (std::getline(file, line))
	{
		if (line.length() < LEN_ID + 2)
		{
			std::cerr << RED <<  "file \'" << SESSION_COOKIE << "\' has been corrupted" << DFLT << std::endl;
			file.close();
			return "";
		}
		std::string newID = line.substr(0, LEN_ID);
		std::string newPreference = line.substr(LEN_ID + 1);
		map.insert(std::pair<std::string, std::string>(newID, newPreference));
	}
	file.close();
	std::string _httpResponse;
	if (map.find(ID) == map.end())
		_httpResponse = "No preference set yet";
	else
		_httpResponse = "You are a " + map[ID] + " person";
	_httpResponse = SESSION_COOKIE_ANSWER_START + _httpResponse + SESSION_COOKIE_ANSWER_END;
	return _httpResponse;
}
