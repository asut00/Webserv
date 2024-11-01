/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asuteau <asuteau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 15:19:07 by asuteau           #+#    #+#             */
/*   Updated: 2024/10/02 18:54:41 by asuteau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Webserv.hpp"

bool		isOnlyWhiteSpaces(const char *s)
{
	while (*s == ' ' || (9 <= *s && *s <= 13))
		s++;
	if (!*s)
		return true;
	return false;
}

void		replaceSubstring(std::string &str, const std::string &from, const std::string &to)
{
	std::string::size_type startPos = str.find(from);
	if (startPos != std::string::npos)
		str.replace(startPos, from.length(), to);
}

void		putbytesread(const char *s, ull bytes)
{
	ull i = 0;
	while (i < bytes)
	{
		write(1, &s[i], 1);
		i++;
	}
}

bool		is_port(std::string word)
{
	int len = word.length();
	if (len > 5)
		return false;
	for (int i = 0; i < len; i++)
	{
		if (!isdigit(word[i]))
			return false;
	}
	int a = atoi(word.c_str());
	if (!a || a > MAX_PORT)
		return false;
	return true;
}

bool		is_client_body(std::string word)
{
	int len = word.length();
	if (len > 11)
		return false;
	for (int i = 0; i < len; i++)
	{
		if (!isdigit(word[i]))
			return false;
	}
	return true;
}

std::string	ull_to_string(ull value)
{
	std::ostringstream oss;
	oss << value;
	return oss.str();
}

std::string	ull_to_hexa(ull value)
{
	std::ostringstream chunk_stream;
	chunk_stream << std::hex << value;
	return chunk_stream.str();
}

int			isCgiExtension(std::string url)
{
	if (url.find(".php") != std::string::npos)
		return PHP;
	else if (url.find(".py") != std::string::npos)
		return PYTHON;
	return NONE;
}
