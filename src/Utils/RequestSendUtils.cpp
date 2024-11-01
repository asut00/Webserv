/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestSendUtils.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thelaine <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 13:18:30 by thelaine          #+#    #+#             */
/*   Updated: 2024/10/29 13:18:31 by thelaine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Webserv.hpp"

std::string generateDeleteIndexLine(std::string uri, std::string dirEntryName)
{
	(void)uri;
	return "<button onclick=\"sendDeleteRequest('/post/" + dirEntryName + "')\"> " + dirEntryName + "</button>\r\n";
}

std::string generateAutoIndexLine(std::string uri, std::string dirEntryName)
{
	return "<a href=\"" + uri + "/" + dirEntryName +"\" role=\"button\">" + dirEntryName + "</a>\r\n";
}


