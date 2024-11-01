/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PostData.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asuteau <asuteau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/02 17:55:31 by asuteau           #+#    #+#             */
/*   Updated: 2024/10/02 18:40:43 by asuteau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "PostData.hpp"

PostData::PostData() :
_data("Default data"), _charData(NULL), _dataLength(0), _uri(""), _root(""), _hasActualData(false), _contentStart(0)
{
}

PostData::PostData(char *data, ull dataLength, std::string boundary, std::string uri, std::string root) :
_data(data), _charData(data), _dataLength(dataLength), _uri(uri), _root(root), _boundary(boundary), _hasActualData(false), _contentStart(0)
{
}

PostData::PostData(const PostData &o) :
_data(o._data), _charData(o._charData), _dataLength(o._dataLength), _uri(o._uri), _root(o._root), _boundary(o._boundary), _hasActualData(o._hasActualData), _contentStart(0)
{
}

PostData	&PostData::operator=(const PostData &o)
{
	if (this != &o)
	{
		_data = o._data;
		_charData = o._charData;
		_uri = o._uri;
		_root = o._root;
		_boundary = o._boundary;
		_strHeaders = o._strHeaders;
		_content = o._content;
		_headers = o._headers;
		_dataLength = o._dataLength;
		_hasActualData = o._hasActualData;
		_contentStart= o._contentStart;
	}
	return *this;
}

PostData::~PostData()
{
}

/////////////////////////// METHODS ///////////////////////////

int	PostData::handlePostData()
{
	ull headerStart = 0;
	ull headerEnd = _data.find("\r\n\r\n");
	if (headerEnd != std::string::npos)
		_contentStart = headerEnd + 4; // 4 for "\r\n\r\n"
	if (headerEnd != std::string::npos && _contentStart < _dataLength)
	{
		_hasActualData = true;
		_strHeaders = _data.substr(headerStart, headerEnd);
		_content = _data.substr(_contentStart, _dataLength - _contentStart);
		int ret_setupHeaders = setupHeaders();
		if (ret_setupHeaders != HTTP_OK)
			return ret_setupHeaders;
		return handleContent();
	}
	else
	{
		std::cerr << ORANGE << "Warning : Post Data has no actual data" << std::endl;
		_hasActualData = false;
		return HTTP_UNPROCESSABLE_ENTITY;
	}
	return HTTP_OK;
}

int	PostData::handleContent()
{
	if (_headers.find("name") != _headers.end() && _headers["name"] == "\"inputText\"")
		return handleTextContent();
	else if (_headers.find("name") != _headers.end() && (_headers["name"] == "\"fileUpload\"" || _headers["name"] == "\"file\""))
		return handleUploadContent();
	else
	{
		std::cerr << RED << "This server doesn't handle this type of content" << DFLT << std::endl;
		return HTTP_UNPROCESSABLE_ENTITY;
	}
}

int	PostData::handleTextContent()
{
	std::string		outputFileName = "Username";
	std::string 	outputPath = _root + _uri + "username/username.txt";
	std::ofstream	outFile(outputPath.c_str());
	if (!outFile)
	{
		std::cerr << "Error: can't create file" << std::endl;
		return HTTP_INTERNAL_SERVER_ERROR;
	}

	outFile << _content;
	outFile.close();

	std::cout << GREEN << "SUCCESS : Text file has been succesfully created" << DFLT << std::endl;
	return HTTP_OK;
}

int	PostData::handleUploadContent()
{
	std::string		outputFileName = "Default";

	if (_headers.find("filename") != _headers.end())
	{
		outputFileName = _headers["filename"];
		if (!outputFileName.empty() && outputFileName[0] == '"' && outputFileName[outputFileName.size() - 1] == '"')
		{
			outputFileName.erase(0, 1);
			outputFileName.erase(outputFileName.size() - 1);
		}
	}

	// Get contentLength and filepath
	ull contentLength = _dataLength - _contentStart;
	std::string	outputPath = _root + "/post/" + outputFileName;

	// Open file
	int file_descriptor = open(outputPath.c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (file_descriptor < 0)
	{
		std::cerr << RED << "Error: couldn't open file " + outputFileName << DFLT << std::endl;
		return HTTP_INTERNAL_SERVER_ERROR;
	}

	// Write to file
	int bytes_written = write(file_descriptor, _charData + _contentStart, sizeof(char) * contentLength);
	if (bytes_written < 0 || close(file_descriptor) < 0)
	{
		if (bytes_written < 0)
			std::cerr << RED << "Error: couldn't write in file " + outputFileName << DFLT << std::endl;
		else
			std::cerr << RED << "Error: couldn't close file " + outputFileName << DFLT << std::endl;
		return HTTP_INTERNAL_SERVER_ERROR;
	}
	else
	{
		std::cout << GREEN << "SUCCESS : File has been succesfully downloaded" << DFLT << std::endl;
		return HTTP_OK;
	}
}

int	PostData::setupHeaders(){
	// Init parsing variables
	ull start = 0;
	ull nextSeparator = _strHeaders.find_first_of("\n;", start);
	ull nextEqual = _strHeaders.find_first_of(":=", start);
	while (start != std::string::npos && nextEqual != std::string::npos)
	{
		std::string key = _strHeaders.substr(start, nextEqual - start);
		key = key.substr(key.find_first_not_of(";\n "));

		// Get the value
		std::string value;
		value = _strHeaders.substr(nextEqual+1);
		if (nextSeparator != std::string::npos)
			value = _strHeaders.substr(nextEqual+1, nextSeparator - (nextEqual+1));
		value = value.substr(value.find_first_not_of(" "));
		// Delete spaces at the end of value
		while (!value.empty() && (value[value.size() - 1] == ' ' || value[value.size() - 1] == '\n' || value[value.size() - 1] == '\r'))
			value.erase(value.size() - 1);  // Delete last character

		// Add key/value to the map
		_headers[key] = value;

		// update the parsing variables for next iteration
		start = nextSeparator;
		nextSeparator = _strHeaders.find_first_of("\n;\0", start + 1);
		nextEqual = _strHeaders.find_first_of(":=", start + 1);
	}
	return HTTP_OK;
}
