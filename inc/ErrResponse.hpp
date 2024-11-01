/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ErrResponse.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asuteau <asuteau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 15:27:51 by asuteau           #+#    #+#             */
/*   Updated: 2024/10/02 18:17:26 by asuteau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Webserv.hpp"

#define PAGE_NOT_FOUND 404
#define ERR_PAGE_TEMPLATE "./pages/error/_error_template.html"

class Server; // pre declaration to avoid compilation issues

class ErrResponse
{
	private:
		std::string		_version;
		int				_statusCode;
		std::string		_statusCodeStr;
		std::string		_statusTitle;

	public:
		ErrResponse();
		~ErrResponse();

		std::string		getErrResponse(int statusCode, const Server &server, std::string cookieID, bool sendCookieID);
		void			setErrTitle(int statusCode);
		void			setupErrResponseAttributes(int statusCode);
};
