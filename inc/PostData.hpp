/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PostData.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asuteau <asuteau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/02 17:56:00 by asuteau           #+#    #+#             */
/*   Updated: 2024/10/02 18:51:31 by asuteau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Webserv.hpp"

class	PostData
{
	private :
		std::string							_data;
		char								*_charData;
		ull									_dataLength;

		std::string							_uri;
		std::string							_root;

		std::string							_boundary;
		std::string							_strHeaders;
		std::string							_content;

		std::map<std::string,std::string>	_headers;

		bool								_hasActualData;

		ull									_contentStart;


	public :
		PostData();
		PostData(char *data, ull dataLength, std::string boundary, std::string uri, std::string root);
		PostData(const PostData &o);
		PostData	&operator=(const PostData &o);
		~PostData();

		int			handlePostData();
		int			setupHeaders();
		int			handleContent();
		int			handleTextContent();
		int			handleUploadContent();

};
