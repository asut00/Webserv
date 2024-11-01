/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WebservManager.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thelaine <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/02 18:30:06 by thelaine          #+#    #+#             */
/*   Updated: 2024/10/02 18:30:08 by thelaine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Webserv.hpp"

////////////////////////////////// CANON //////////////////////////////////

// Constructor with argument
webservManager::webservManager(const char *configFile)
{
	_maxFd = 0;
	_nbrOfClients = 0;
	std::vector<int> vectAlreadyUsedPort;

	try
	{
		splitConfig(configFile);
		splitServer();
	}
	catch (std::exception &e)
	{
		throw webserverError(e.what());
	}
		FD_ZERO(&_currentSet);
		for (std::vector<Server>::iterator serverIterator = _vectServers.begin(); serverIterator != _vectServers.end(); serverIterator++)
		{
			try
			{
				serverIterator->start_server(_mapSocketServer, _currentSet, _maxFd, _nbrOfClients, _vectOfReadyToReadSocket, vectAlreadyUsedPort);
			}
			catch (std::exception &e)
			{
				std::cerr << RED << "Error: "  << e.what() << DFLT << std::endl;
				continue;
			}
		}
	if (vectAlreadyUsedPort.size() == 0)
		throw webserverError(FATAL_LAUNCH_ERROR);
}

// Destructor
webservManager::~webservManager()
{
}

////////////////////////////////// Tools //////////////////////////////////

int webservManager::isServerFd(int fd)
{
	for (std::vector<Server>::iterator serverIterator = _vectServers.begin(); serverIterator != _vectServers.end(); serverIterator++)
	{
		int isServerFd = serverIterator->isThisFdOnThisServer(fd);
		if (isServerFd != FAILED)
			return isServerFd;
	}
	return FAILED;
}

void webservManager::terminate()
{
	for (std::vector<int>::iterator readingSocketIterator = _vectOfReadyToWriteSocket.begin(); readingSocketIterator != _vectOfReadyToWriteSocket.end(); readingSocketIterator++)
		close(*readingSocketIterator);
	for (std::vector<int>::iterator writingSocketIterator = _vectOfReadyToReadSocket.begin(); writingSocketIterator != _vectOfReadyToReadSocket.end(); writingSocketIterator++)
		close(*writingSocketIterator);
}

////////////////////////////////// Creating servers //////////////////////////////////

void webservManager::splitConfig(const char *configFile)
{
	std::ifstream file(configFile);

	if (!file.is_open())
		throw Server::ServerError(OPENING_ERROR);

	std::string line;
	std::vector<std::string> splittedConfig;

	while (std::getline(file, line))
	{
		std::istringstream linestream(line);
		std::string word;

		while (linestream >> word)
			splittedConfig.push_back(word);
	}
	file.close();
	_splittedConfig = splittedConfig;
}

void webservManager::splitServer()
{
		std::vector<std::string>::iterator	wordIterator = _splittedConfig.begin();
		std::vector<std::string>::iterator	wordIteratorEnd = _splittedConfig.end();
		if (wordIterator == wordIteratorEnd)
			throw webserverError(FILE_EMPTY);
		std::string word = *wordIterator;
		if (word != "server")
			throw webserverError(START_WITH_THE_BASICS);
		int				serverStudied = 0;
		while (word == "server")
		{
			serverStudied++;
			try
				{
					Server server(wordIterator, wordIteratorEnd);
					_vectServers.push_back(server);
				}
			catch (std::exception & e)
			{
				std::cerr << RED << "Error: Server " << serverStudied <<
				" has a error in its configuration : " << e.what() << "; it will not be launch" << DFLT << std::endl;
				while (wordIterator !=	wordIteratorEnd && *wordIterator != "server")
					wordIterator++;
			}
			if (wordIterator != wordIteratorEnd)
				word = *wordIterator;
			else
				word = "";
		}
		if (_vectServers.size() == 0)
			throw webserverError(FATAL_INIT_ERROR);
}

////////////////////////////////// Run //////////////////////////////////

void webservManager::run()
{
	struct timeval	timeout;

	while (RUNNING)
	{
		timeout.tv_sec = TIMEOUT_SELECT_SEC;
		timeout.tv_usec = TIMEOUT_SELECT_USEC;
		_readSet = _currentSet;
		FD_ZERO(&_writeSet);
		for (std::vector<int>::iterator writingSocketIterator = _vectOfReadyToWriteSocket.begin(); writingSocketIterator != _vectOfReadyToWriteSocket.end(); writingSocketIterator++)
			FD_SET(*writingSocketIterator, &_writeSet);
		int ret = select(_maxFd + 1, &_readSet, &_writeSet, 0, &timeout);
		if (ret == 0)
			continue;
		if (ret < 0)
		{
			resetConnections();
			continue;
		}
		manageReadingSocks();
		ret = manageWritingSocks();
		if (ret == EXIT)
		{
			terminate();
			return ;
		}
	}
}

////////////////////////////////// Manage connections //////////////////////////////////

void webservManager::manageNewConnection(int serverFd)
{
	if (_nbrOfClients == MAX_CLIENT)
	{
		std::cerr << RED << "This server has reached maximum capacity, he can not accept new clients" << std::endl; // /!\ il y avait ce commentaire la : A MODIFIER
		return ;
	}
	struct sockaddr_in  serverAddr;
	socklen_t	len = sizeof(serverAddr);
	int clientFd = accept(serverFd, (struct sockaddr *)&serverAddr, &len);
	if (clientFd == FAILED)
	{
		std::cerr << RED << "Accept failed, the client won't be added " << std::endl;
		return ;
	}
	if (clientFd > _maxFd)
		_maxFd = clientFd;
	_nbrOfClients ++;
	FD_SET(clientFd, &_currentSet);
	_vectOfReadyToReadSocket.push_back(clientFd);
	FD_CLR(serverFd, &_readSet);
	_mapSocketServer[clientFd] = &getServerFd(serverFd);
	std::cout << GREEN << "New connection accepted on server " << _mapSocketServer[clientFd]->getName() << DFLT << std::endl;
}

Server &webservManager::getServerFd(int fd)
{
	for (std::vector<Server>::iterator serverIterator = _vectServers.begin(); serverIterator != _vectServers.end(); serverIterator++)
	{
		if (serverIterator->isThisFdOnThisServer(fd) != FAILED)
			return *serverIterator;
	}
	return *_vectServers.begin();
}

void webservManager::resetConnections()
{
	std::cerr << RED << "Warning: select error, reseting all connection" << std::endl;
	FD_ZERO(&_currentSet);
	_vectOfReadyToReadSocket.clear();
	_vectOfReadyToWriteSocket.clear();
	_maxFd = 0;
	_mapSocketServer.clear();
	for (std::map<int, Request>::iterator mapSocketRequestIterator = _mapSocketRequest.begin(); mapSocketRequestIterator != _mapSocketRequest.end(); mapSocketRequestIterator++)
	{
		mapSocketRequestIterator->second.setErrorStatusCode(HTTP_INTERNAL_SERVER_ERROR);
		mapSocketRequestIterator->second.sendErrResponse();
	}
	_mapSocketRequest.clear();
	for (std::vector<Server>::iterator serverIterator = _vectServers.begin(); serverIterator != _vectServers.end(); serverIterator++)
		serverIterator->reset_server(_mapSocketServer, _currentSet, _maxFd, _vectOfReadyToReadSocket);
}


void webservManager::endConnection(int fd)
{
	if (fd == _maxFd)
		_maxFd--;
	_nbrOfClients--;
	FD_CLR(fd, &_currentSet);
	FD_CLR(fd, &_readSet);
	close(fd);
	_mapSocketServer.erase(fd);
	_mapSocketRequest.erase(fd);
}

////////////////////////////////// Reading from socket //////////////////////////////////

void webservManager::manageReadingSocks()
{
	for (std::vector<int>::iterator readingSocketIterator = _vectOfReadyToReadSocket.begin() ; readingSocketIterator != _vectOfReadyToReadSocket.end() ; readingSocketIterator++)
	{
		if(FD_ISSET(*readingSocketIterator, &_readSet))
		{
			int serverFd = isServerFd(*readingSocketIterator); // check if new connection
			if (serverFd != FAILED)
			{
				std::cout << YELLOW << "New connection attempt detected" << DFLT << std::endl;
				manageNewConnection(serverFd);
				readingSocketIterator = _vectOfReadyToReadSocket.begin();
			}
			else
			{
				bzero(_recvBuffer, sizeof(_recvBuffer));
				int recvBytesRead = recv(*readingSocketIterator, _recvBuffer, sizeof(_recvBuffer), 0);
				if (recvBytesRead <= 0)
				{
					if (recvBytesRead < 0)
						std::cerr << RED << "Error: recv failed, closing the connection" << DFLT << std::endl;
					if (recvBytesRead == 0)
						std::cout << CYAN << "One client has left the server " << _mapSocketServer[*readingSocketIterator]->getName() << DFLT << std::endl;
					endConnection(*readingSocketIterator);
					readingSocketIterator = _vectOfReadyToReadSocket.erase(readingSocketIterator);
					readingSocketIterator = _vectOfReadyToReadSocket.begin();
				}
				else
				{
					manageRecv(readingSocketIterator, recvBytesRead);
					readingSocketIterator = _vectOfReadyToReadSocket.begin();
				}
			}
		}
	}
}

void webservManager::manageRecv(std::vector<int>::iterator &readingSocketIterator, int recvBytesRead)
{
	int returnReadRequest = READING_ERROR;
	if (_mapSocketRequest.find(*readingSocketIterator) != _mapSocketRequest.end())
		returnReadRequest = _mapSocketRequest[*readingSocketIterator].readAgain(_recvBuffer, recvBytesRead);
	else
	{
		_mapSocketRequest.insert(std::make_pair(*readingSocketIterator, Request(*readingSocketIterator, _recvBuffer, recvBytesRead, _mapSocketServer[*readingSocketIterator])));
		returnReadRequest = _mapSocketRequest[*readingSocketIterator].readRequest();
	}
	if (returnReadRequest == READING_ERROR)
	{
		FD_CLR(*readingSocketIterator, &_currentSet);
		close(*readingSocketIterator);
		_mapSocketRequest.erase(*readingSocketIterator);
		readingSocketIterator = _vectOfReadyToReadSocket.erase(readingSocketIterator);
	}
	else if (returnReadRequest == NEED_TO_READ_AGAIN)
		FD_CLR(*readingSocketIterator, &_readSet);
	else if (returnReadRequest == READING_COMPLETE)
	{
		FD_CLR(*readingSocketIterator, &_readSet);
		_vectOfReadyToWriteSocket.push_back(*readingSocketIterator);
		readingSocketIterator = _vectOfReadyToReadSocket.erase(readingSocketIterator);
	}
}

////////////////////////////////// Writing in sockets //////////////////////////////////

int webservManager::manageWritingSocks()
{
	for (std::vector<int>::iterator writingSocketIterator = _vectOfReadyToWriteSocket.begin() ;writingSocketIterator != _vectOfReadyToWriteSocket.end() ;)
	{
		if (FD_ISSET(*writingSocketIterator, &_writeSet))
		{
			int returnHandleRequest = _mapSocketRequest[*writingSocketIterator].handleRequest(this);
			if (returnHandleRequest == EXIT)
				return EXIT;
			else if (returnHandleRequest == FAILED)
				endConnection(*writingSocketIterator);
			else if (returnHandleRequest == CHUNK_SENT)
			{
				writingSocketIterator++;
				continue;
			}
			else
				_vectOfReadyToReadSocket.push_back(*writingSocketIterator);
			_mapSocketRequest.erase(*writingSocketIterator);
			writingSocketIterator = _vectOfReadyToWriteSocket.erase(writingSocketIterator);
		}
		else
			writingSocketIterator++;
	}
	return SUCCEEDED;
}

////////////////////////////////// EXCEPTIONS //////////////////////////////////

const char *webservManager::webserverError::what() const throw()
{
	return _message.c_str();
}
