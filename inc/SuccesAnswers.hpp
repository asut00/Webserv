/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SuccessAnswers.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thelaine <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/24 17:55:59 by thelaine          #+#    #+#             */
/*   Updated: 2024/10/24 17:56:00 by thelaine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#define DELETE_SUCCESS_ANSWER "HTTP/1.1 200 OK\n\r\
Content-Type: application/json\n\r\
Connection: keep-alive\n\r\
{\n\r\
    \"message\": \"Resource deleted successfully.\"\n\r\
}"

#define POST_SUCCESS_ANSWER "HTTP/1.1 200 OK\n\r\
Content-Type: application/json\n\r\
Connection: keep-alive\n\r\
{\n\r\
    \"message\": \"post was completed successfully.\"\n\r\
}"

#define COOKIE_SUCCESS_ANSWER "<!DOCTYPE html>\n\r\
<html lang=\"fr\">\n\r\
<head>\n\r\
    <meta charset=\"UTF-8\">\n\r\
    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n\r\
    <title>Delete | Webserv</title>\n\r\
    <style>\n\r\
        body {\n\r\
            font-family: Arial, sans-serif;\n\r\
            margin: 0;\n\r\
            padding: 0;\n\r\
            display: flex;\n\r\
            justify-content: center;\n\r\
            align-items: center;\n\r\
            height: 100vh;\n\r\
            background-color: black;\n\r\
        }\n\r\
        .container {\n\r\
            text-align: left;\n\r\
        }\n\r\
        h1 {\n\r\
            color: #f4f4f4;\n\r\
            margin-bottom: 20px;\n\r\
            text-align: center;\n\r\
        }\n\r\
        h3 {\n\r\
            font-weight: 100;\n\r\
        }\n\r\
        form {\n\r\
            display: flex;\n\r\
            flex-direction: column;\n\r\
            align-items: center;\n\r\
        }\n\r\
        input[type=\"text\"] {\n\r\
            padding: 10px;\n\r\
            margin: 20px 0 10px 0;\n\r\
            width: 200px;\n\r\
            border: none;\n\r\
            border-radius: 5px;\n\r\
            box-shadow: 0 2px 5px rgba(0,0,0,0.2);\n\r\
        }\n\r\
        button {\n\r\
            padding: 8px;\n\r\
            margin: 0 0;\n\r\
            background-color: black;\n\r\
            color: white;\n\r\
            border: none;\n\r\
            border-radius: 5px;\n\r\
            cursor: pointer;\n\r\
            width: 150px;\n\r\
            text-align: center;\n\r\
            font-size: 0.9em;\n\r\
            font-family: monospace;\n\r\
            transition: background-color 0.3s;\n\r\
        }\n\r\
        button:hover {\n\r\
            background-color: red;\n\r\
        }\n\r\
        .buttons {\n\r\
            margin-top: 20px;\n\r\
            display: flex;\n\r\
            flex-wrap: wrap;\n\r\
            justify-content: center;\n\r\
            gap: 10px;\n\r\
        }\n\r\
        .buttons a {\n\r\
            text-decoration: none;\n\r\
            color: white;\n\r\
            background-color: grey;\n\r\
            padding: 8px 20px;\n\r\
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
            <h1>🍪 Cookie was set successfully 🍪</h1>\n\r\
            <div class=\"buttons\">\n\r\
                <a href=\"./\" role=\"button\" style=\"font-family: Arial, sans-serif;\">Go Back</a>\n\r\
            </div>\n\r\
        </div>\n\r\
    </div>\n\r\
</body>\n\r\
</html>"