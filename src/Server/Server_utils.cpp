/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server_utils.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thelaine <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 16:18:55 by thelaine          #+#    #+#             */
/*   Updated: 2024/10/29 16:18:56 by thelaine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Webserv.hpp"

ull unamed_server_id = 0;


//////////SERVER CREATION

std::string Server::incrementWord()
{
	if (++_wordIterator == _wordIteratorEnd)
		throw ServerError(UNEXPECTED_END_OF_FILE);
	return *_wordIterator;
}

void	Server::checkStart(int mode)
{
	_word = incrementWord();
	if (_word != "{")
	{
		if (mode == SERVER)
			throw ServerError(SERVER_START);
		throw ServerError(LOCATION_START);
	}
	_word = incrementWord();
}

void	Server::setName()
{
	if (_word == "name")
	{
		_name = incrementWord();
		_word = incrementWord();
	}
	else
	{
		_name = DEFAULT_SERVER_NAME + ull_to_string(unamed_server_id);
		unamed_server_id++;
	}
}

void	Server::setListenningPorts()
{
	if (_word != "listen")
		throw ServerError(NO_LISTEN);
	_word = incrementWord();
	if (!is_port(_word))
		throw ServerError(NO_PORT);
	_listenningPort.push_back(atoi(_word.c_str()));
	_word = incrementWord();
	while (is_port(_word))
	{
		_listenningPort.push_back(atoi(_word.c_str()));
		_word = incrementWord();
	}
}

void Server::setRoot(int mode)
{
	if (_word != "root" && mode == SERVER)
		throw ServerError(NO_ROOT);
	else if (_word == "root")
	{
		_root = incrementWord();
		_word = incrementWord();
	}
}

void Server::setIndex(int mode)
{
	if (_word != "index" && mode == SERVER)
		throw ServerError(NO_INDEX);
	else if (_word == "index")
	{
		_index = incrementWord();
		_word = incrementWord();
	}
}

void Server::setAutoIndex()
{
	if (_word == "auto_index")
	{
		_word = incrementWord();
		if (_word == "true")
			_autoIndex = true;
		else if (_word == "false")
			_autoIndex = false;
		else
			throw ServerError(WRONG_AUTO_INDEX);
		_word = incrementWord();
	}
}

void Server::setErrorPages()
{
	while (_word == "error_page")
	{
		_word = incrementWord();
		std::vector<int> error_pages;
		int	error_code = atoi(_word.c_str());
		while (httpStatusCodes.find(error_code) != httpStatusCodes.end())
		{
			error_pages.push_back(error_code);
			_word = incrementWord();
			error_code = atoi(_word.c_str());
		}
		for (std::vector<int>::iterator err = error_pages.begin(); err != error_pages.end(); err++)
			_mapErrorPages[*err] = _word;
		_word = incrementWord();
	}
}

void Server::setFavicon()
{
	if (_word == "favicon")
	{
		_favicon = incrementWord();
		_word = incrementWord();
	}
}

void Server::setRules()
{
	if (_word == "allow")
	{
		_word = incrementWord();
		while (_word == "GET" || _word == "POST" || _word == "DELETE")
		{
			if (_word == "GET")
				_allowed[GET] = true;
			else if (_word == "POST")
				_allowed[POST] = true;
			else if (_word == "DELETE")
				_allowed[DELETE] = true;
			_word = incrementWord();
		}
	}
}
void Server::setMaxClientBody()
{
	if (_word == "client_body")
	{
		_word = incrementWord();
		if (!is_client_body(_word))
			throw ServerError(WRONG_MAX_CLIENT_BODY);
		_maxClientBody = atoi(_word.c_str());
		_word = incrementWord();
	}
}

void Server::setSubLocation(std::vector<std::string> &alreadyExistingSubLocations)
{
	int	subLocationNbr = 0;
	while (_word == "location")
	{
		try
		{
			Server sub_location(*this, alreadyExistingSubLocations);
			_subLocations.push_back(sub_location);
			if (_wordIterator != _wordIteratorEnd)
				_word = (*_wordIterator);
			else
				_word = "";
		}
		catch (std::exception & e)
		{
			std::cout << RED << "Error: Sublocation " << subLocationNbr << " of server " << _name <<
				" has a error in its configuration: " << e.what() << "; it will not be set again for this server" << DFLT << std::endl;
				while (_word != "}")
					_word = incrementWord();
				_word = incrementWord();
		}
		subLocationNbr++;
	}
}

void	Server::checkIfSubLocationAlreadyExisting(std::vector<std::string> &alreadyExistingSubLocations)
{
	_word = incrementWord();
	if (std::find(alreadyExistingSubLocations.begin(), alreadyExistingSubLocations.end(), _word) != alreadyExistingSubLocations.end())
		throw ServerError(SUBLOCATION_ALREADY_SET);
	alreadyExistingSubLocations.push_back(_word);
	_specialLocation = _word;
}

void	Server::setCgiType()
{
	if (_word == "cgi_type")
	{
		_word = incrementWord();
		if (_word == "py")
			_cgiType = PYTHON;
		else if (_word == "php")
			_cgiType = PHP;
		else
			throw ServerError(CGI_TYPE_UNKNOWN);
		_word = incrementWord();
	}
}

void Server::checkEnd(int mode)
{
	if (_word != "}")
	{
		if (mode == SERVER)
			throw ServerError(SERVER_END);
		throw ServerError(LOCATION_END);
	}
	_wordIterator++;
}

////////SERVER START/////////

void	Server::checkMaxClient(int nbrOfClients)
{
	if (nbrOfClients == MAX_CLIENT)
		throw ServerError("There is way to many servers and or port, max fd  of " + ull_to_string(MAX_CLIENT) + " been reached");
}

void	Server::checkPortAlreadyInUse(std::vector<int> &vectAlreadyUsedPort, int portStudied)
{
	std::vector<int>::iterator port = std::find(vectAlreadyUsedPort.begin(), vectAlreadyUsedPort.end(), portStudied);
	if (port != vectAlreadyUsedPort.end())
		throw ServerError("port " + ull_to_string(portStudied) + " already in use, it will not be set for server " + _name);
}
int		Server::createSocketForPort(int portStudied)
{
	int portSocketFd = socket(AF_INET, SOCK_STREAM, 0);
	if (portSocketFd == FAILED)
		throw ServerError("socket failed for port " + ull_to_string(portStudied) + " it will not be set for server " + _name);
	return portSocketFd;
}

void	Server::setSocketOpt(int portSocketFd, int portStudied)
{
		int opt = 1;
	if (setsockopt(portSocketFd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
	{
		close(portSocketFd);
		throw ServerError("setsockopt failed for port " + ull_to_string(portStudied) + " it will not be set for server " + _name);
	}
}
void	Server::bindPortSocketFd(int portSocketFd, int portStudied)
{
	struct sockaddr_in serverAddr;
	bzero(&serverAddr.sin_zero, sizeof(serverAddr.sin_zero));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serverAddr.sin_port = htons(portStudied);
	if (bind(portSocketFd, (const struct sockaddr *)&serverAddr, sizeof(serverAddr)) == FAILED)
	{
		close(portSocketFd);
		throw ServerError("bind failed for port " + ull_to_string(portStudied) + " it will not be set for server " + _name);
	}
}
void	Server::listenPortFd(int portSocketFd, int portStudied)
{
	if (listen(portSocketFd, 100) == FAILED)
	{
		close(portSocketFd);
		throw ServerError("listen failed for port " + ull_to_string(portStudied) + " it will not be set for server " + _name);
	}
}
void	Server::updateSocketPortData(std::map<int, Server *> &map_socket_server, fd_set &currentSet, int &maxFd, int &nbrOfClients,
std::vector<int> &vectOfReadyToReadSocket, std::vector<int> &vectAlreadyUsedPort, int portSocketFd, int portStudied)
{
	if (portSocketFd > maxFd)
		maxFd = portSocketFd;
	nbrOfClients++;
	FD_SET(portSocketFd, &currentSet);
	vectOfReadyToReadSocket.push_back(portSocketFd);
	socketFds.push_back(portSocketFd);
	_actuallyListenningPort.push_back(portStudied);
	vectAlreadyUsedPort.push_back(portStudied);
	(map_socket_server)[portSocketFd] = this;

}
void	Server::printServerStartStatus()
{
	if (_actuallyListenningPort.size() == 0)
	{
		throw ServerError("Server " + _name + " failed to launch any of his ports" );
	}
	else if (_actuallyListenningPort.size() != _listenningPort.size())
	{
		std::cout << YELLOW << "Server " + _name + " was launch partially with ports : ";
		printActualListenningPorts();
		std::cout << DFLT << std::endl;
	}
	else
	{
		std::cout << GREEN << "Server " + _name + " was launch fully with ports : ";
		printActualListenningPorts();
		std::cout << DFLT << std::endl;
	}
}
