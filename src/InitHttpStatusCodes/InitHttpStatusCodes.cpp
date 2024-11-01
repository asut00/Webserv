/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   InitHttpStatusCodes.cpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asuteau <asuteau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 17:30:51 by asuteau           #+#    #+#             */
/*   Updated: 2024/09/24 17:31:08 by asuteau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Webserv.hpp"

std::map<int, std::string> httpStatusCodes;

void	initHttpStatusCode()
{
    httpStatusCodes.insert(std::make_pair(100, "Continue"));
    httpStatusCodes.insert(std::make_pair(101, "Switching Protocols"));
    httpStatusCodes.insert(std::make_pair(200, "OK"));
    httpStatusCodes.insert(std::make_pair(201, "Created"));
    httpStatusCodes.insert(std::make_pair(202, "Accepted"));
    httpStatusCodes.insert(std::make_pair(203, "Non-Authoritative Information"));
    httpStatusCodes.insert(std::make_pair(204, "No Content"));
    httpStatusCodes.insert(std::make_pair(205, "Reset Content"));
    httpStatusCodes.insert(std::make_pair(206, "Partial Content"));
    httpStatusCodes.insert(std::make_pair(300, "Multiple Choices"));
    httpStatusCodes.insert(std::make_pair(301, "Moved Permanently"));
    httpStatusCodes.insert(std::make_pair(302, "Found"));
    httpStatusCodes.insert(std::make_pair(303, "See Other"));
    httpStatusCodes.insert(std::make_pair(304, "Not Modified"));
    httpStatusCodes.insert(std::make_pair(305, "Use Proxy"));
    httpStatusCodes.insert(std::make_pair(307, "Temporary Redirect"));
    httpStatusCodes.insert(std::make_pair(308, "Permanent Redirect"));
    httpStatusCodes.insert(std::make_pair(400, "Bad Request"));
    httpStatusCodes.insert(std::make_pair(401, "Unauthorized"));
    httpStatusCodes.insert(std::make_pair(402, "Payment Required"));
    httpStatusCodes.insert(std::make_pair(403, "Forbidden"));
    httpStatusCodes.insert(std::make_pair(404, "Not Found"));
    httpStatusCodes.insert(std::make_pair(405, "Method Not Allowed"));
    httpStatusCodes.insert(std::make_pair(406, "Not Acceptable"));
    httpStatusCodes.insert(std::make_pair(407, "Proxy Authentication Required"));
    httpStatusCodes.insert(std::make_pair(408, "Request Timeout"));
    httpStatusCodes.insert(std::make_pair(409, "Conflict"));
    httpStatusCodes.insert(std::make_pair(410, "Gone"));
    httpStatusCodes.insert(std::make_pair(411, "Length Required"));
    httpStatusCodes.insert(std::make_pair(412, "Precondition Failed"));
    httpStatusCodes.insert(std::make_pair(413, "Payload Too Large"));
    httpStatusCodes.insert(std::make_pair(414, "URI Too Long"));
    httpStatusCodes.insert(std::make_pair(415, "Unsupported Media Type"));
    httpStatusCodes.insert(std::make_pair(416, "Range Not Satisfiable"));
    httpStatusCodes.insert(std::make_pair(417, "Expectation Failed"));
    httpStatusCodes.insert(std::make_pair(426, "Upgrade Required"));
    httpStatusCodes.insert(std::make_pair(500, "Internal Server Error"));
    httpStatusCodes.insert(std::make_pair(501, "Not Implemented"));
    httpStatusCodes.insert(std::make_pair(502, "Bad Gateway"));
    httpStatusCodes.insert(std::make_pair(503, "Service Unavailable"));
    httpStatusCodes.insert(std::make_pair(504, "Gateway Timeout"));
    httpStatusCodes.insert(std::make_pair(505, "HTTP Version Not Supported"));
}
