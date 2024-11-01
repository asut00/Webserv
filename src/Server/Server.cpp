/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thelaine <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/25 11:01:52 by thelaine          #+#    #+#             */
/*   Updated: 2024/09/25 11:01:53 by thelaine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Webserv.hpp"

Server::~Server()
{
}


Server::Server(const Server& other):
_wordIterator(other._wordIterator), _wordIteratorEnd(other._wordIteratorEnd)
{
	*this = other;
}

Server& Server::operator=(const Server& other)
{
	_name = other._name;
	_listenningPort = other._listenningPort;
	_root = other._root;
	_index = other._index;
	_allowed[0] = other._allowed[0];
	_allowed[1] = other._allowed[1];
	_allowed[2] = other._allowed[2];
	_maxClientBody = other._maxClientBody;
	_cgiType = other._cgiType;
	_subLocations = other._subLocations;
	_specialLocation = other._specialLocation;
	_mapErrorPages = other._mapErrorPages;
	_favicon = other._favicon;
	_autoIndex = other._autoIndex;
	_wordIterator = other._wordIterator;
	_wordIteratorEnd = other._wordIteratorEnd;
	return *this;
}

Server::Server(const Server& other, std::vector<std::string> &alreadyExistingSubLocations):
_wordIterator(other._wordIterator), _wordIteratorEnd(other._wordIteratorEnd)
{
	*this = other;
	checkIfSubLocationAlreadyExisting(alreadyExistingSubLocations);
	checkStart(SUBLOCATION);
	setRoot(SUBLOCATION);
	setIndex(SUBLOCATION);
	setAutoIndex();
	setErrorPages();
	setRules();
	setMaxClientBody();
	setCgiType();
	checkEnd(SUBLOCATION);
}

Server::Server(std::vector<std::string>::iterator &wordIterator, std::vector<std::string>::iterator &wordIteratorEnd):
_maxClientBody(DEFAULT_MAX_CLIENT_BODY), _cgiType(NONE), _autoIndex(false),
_wordIterator(wordIterator), _wordIteratorEnd(wordIteratorEnd)
{
	std::vector<std::string> alreadyExistingSubLocations;
	for (int i = 0; i < 3; i++)
		_allowed[i] = false;
	checkStart(SERVER);
	setName();
	setListenningPorts();
	setRoot(SERVER);
	setIndex(SERVER);
	setAutoIndex();
	setErrorPages();
	setFavicon();
	setRules();
	setMaxClientBody();
	setSubLocation(alreadyExistingSubLocations);
	checkEnd(SERVER);
}

void	Server::start_server(std::map<int, Server *> &map_socket_server, fd_set &currentSet, int &maxFd, int &nbrOfClients, std::vector<int> &vectOfReadyToReadSocket, std::vector<int> &vectAlreadyUsedPort)
{
	int portSocketFd;
	std::cout << YELLOW_BRIGHT << "Launching server " << _name << DFLT << std::endl;
	for (std::vector<int>::iterator portIterator = _listenningPort.begin(); portIterator!= _listenningPort.end(); portIterator++)
	{
		try
		{
			checkMaxClient(nbrOfClients);
			checkPortAlreadyInUse(vectAlreadyUsedPort, *portIterator);
			portSocketFd = createSocketForPort(*portIterator);
			setSocketOpt(portSocketFd, *portIterator);
			bindPortSocketFd(portSocketFd, *portIterator);
			listenPortFd(portSocketFd, *portIterator);
			updateSocketPortData(map_socket_server, currentSet, maxFd, nbrOfClients,
				vectOfReadyToReadSocket, vectAlreadyUsedPort, portSocketFd, *portIterator);
		}
		catch (std::exception &e)
		{
			std::cerr << RED << "Error: " << e.what() << DFLT << std::endl;
			continue;
		}
	}
	printServerStartStatus();
}

void	Server::reset_server(std::map<int, Server *> & map_socket_server, fd_set &currentSet, int &maxFd, std::vector<int> &vectOfReadyToReadSocket)
{
	for (std::vector<int>::iterator socketFdIterator = socketFds.begin(); socketFdIterator != socketFds.end(); socketFdIterator++)
	{
		if (*socketFdIterator > maxFd)
			maxFd = *socketFdIterator;
		FD_SET(*socketFdIterator, &currentSet);
		vectOfReadyToReadSocket.push_back(*socketFdIterator);
		(map_socket_server)[*socketFdIterator] = this;
	}
}

int	Server::isThisFdOnThisServer(int client_fd)
{
	std::vector<int>::iterator clientFdOnServer;
	clientFdOnServer = std::find(socketFds.begin(), socketFds.end(), client_fd);
	if (clientFdOnServer != socketFds.end())
		return *clientFdOnServer;
	return FAILED;
}

void	Server::printActualListenningPorts()
{
	for (std::vector<int>::iterator portIterator = _actuallyListenningPort.begin(); portIterator != _actuallyListenningPort.end(); portIterator++)
		std::cout << *portIterator << " ";
}

Server		*Server::goToActualSublocation(std::string &uri)
{
	std::string LocationSavedName = "";
	std::string	locationStudied;
	ull	i;
	Server	*LocationSaved = 0;

	for (std::vector<Server>::iterator serverIterator = _subLocations.begin(); serverIterator != _subLocations.end(); serverIterator++)
	{
		locationStudied = "/" + serverIterator->getSpecialLocation();
		if (uri.length() >= locationStudied.length() && locationStudied.length() > LocationSavedName.length())
		{
			i = 0;
			for (; i < locationStudied.length() && i < uri.length(); i++)
			{
				if (locationStudied[i] != uri[i])
					break;
			}
			if (i == locationStudied.length())
			{
				LocationSavedName = locationStudied;
				LocationSaved = &(*serverIterator);
			}
		}
	}
	if (LocationSaved != 0)
	{
		std::cout << BLUE << "location is " << LocationSavedName << DFLT << std::endl; //PRINT
		if (LocationSaved->getRoot() != _root)
			uri.erase(0, LocationSavedName.length());
		return LocationSaved;
	}
	return this;
}
////////////////////////GETTER////////////////////////////////

const std::string &Server::getName() const
{
	return _name;
}

const std::string &Server::getRoot() const
{
	return _root;
}
const std::string &Server::getDefaultRequest() const
{
	return _index;
}
const bool* Server::getAllowed() const
{
	return _allowed;
}
const ull &Server::getMaxClientBody() const
{
	return _maxClientBody;
}

const std::string &Server::getSpecialLocation() const
{
	return _specialLocation;
}

const int &Server::getCgiType() const
{
	return _cgiType;
}

const std::string &Server::getFavicon() const
{
	return _favicon;
}

const std::map<int, std::string> &Server::getMapErrorPages() const
{
	return _mapErrorPages;
}

const bool								&Server::getAutoIndex() const
{
	return _autoIndex;
}


const char *Server::ServerError::what() const throw()
{
	return _message.c_str();
}

void	Server::printServer()
{
	std::cout << "server name:'" << _name << "'" << std::endl;
	std::cout << "listenning port:'";
	for (std::vector<int>::iterator it = _listenningPort.begin(); it != _listenningPort.end(); it++){
		std::cout << *it << " ";
	}
	std::cout << "'" << std::endl;
	std::cout << "root:'" << _root << "'" << std::endl;
	std::cout << "special location:'" << _specialLocation << "'" << std::endl;
	std::cout << "default request:'" << _index << "'" << std::endl;
	std::cout << "favicon:'" << _favicon << "'" << std::endl;
	std::cout << "Allowed Rules:" << std::endl;
	std::cout << "\t" << "GET: " << _allowed[GET] << std::endl;
	std::cout << "\t" << "POST: " << _allowed[POST] << std::endl;
	std::cout << "\t" << "DELETE: " << _allowed[DELETE] << std::endl;
	std::cout << "client body:'" << _maxClientBody << "'" << std::endl;
	std::cout << "cgi_type:'" <<_cgiType << "'" << std::endl;
	std::cout << "error_pages :" << std::endl; //Decomment to print sublocation
	for (std::map<int, std::string>::iterator it = _mapErrorPages.begin(); it != _mapErrorPages.end(); it ++)
		std::cout << "code " << it->first << ": " << it->second << std::endl;
	std::cout << "sub_location:" << std::endl; //Decomment to print sublocation
	for (std::vector<Server>::iterator it = _subLocations.begin(); it != _subLocations.end(); it++)
		it->printServer();
	std::cout << std::endl;
}
