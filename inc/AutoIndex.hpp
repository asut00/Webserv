/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AutoIndex.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thelaine <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/24 17:55:59 by thelaine          #+#    #+#             */
/*   Updated: 2024/10/24 17:56:00 by thelaine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#define AUTO_INDEX_PAGE "<!DOCTYPE html> \n\r\
<html lang=\"fr\"> \n\r\
<head> \n\r\
	<meta charset=\"UTF-8\"> \n\r\
	<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n\r\
	<title>Index | Webserv</title>\n\r\
	<style>\n\r\
		body {\n\r\
			font-family: Arial, sans-serif;\n\r\
			margin: 0;\n\r\
			padding: 0;\n\r\
			display: flex;\n\r\
			justify-content: center;\n\r\
			align-items: center;\n\r\
			height: 100vh;\n\r\
			background-color: #000;\n\r\
		}\n\r\
		h1 {\n\r\
			color: #f4f4f4;\n\r\
			text-align: center;\n\r\
		}\n\r\
		p {\n\r\
			color: #666;\n\r\
			font-size: 1.2em;\n\r\
		}\n\r\
		a {\n\r\
			text-decoration: none;\n\r\
			color: #007BFF;\n\r\
		}\n\r\
		.buttons {\n\r\
			margin-top: 20px;\n\r\
			display: flex;\n\r\
			justify-content: center;\n\r\
		}\n\r\
		.buttons a {\n\r\
			text-decoration: none;\n\r\
			color: white;\n\r\
			background-color: #007BFF;\n\r\
			padding: 10px 20px;\n\r\
			margin: 0 10px;\n\r\
			border-radius: 5px;\n\r\
			display: inline-block;\n\r\
		}\n\r\
		.buttons a:hover {\n\r\
			background-color: #0056b3;\n\r\
		}\n\r\
	</style>\n\r\
</head>\n\r\
<body>\n\r\
    <div>\n\r\
        <h1>🤵‍♂️ Auto index of "

#define AUTO_INDEX_END "        </div>\n\r\
    </div>\n\r\
</body>\n\r\
</html>"
