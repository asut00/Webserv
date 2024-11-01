
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WebservManager.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thelaine <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/02 18:30:02 by thelaine          #+#    #+#             */
/*   Updated: 2024/10/02 18:30:04 by thelaine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Webserv.hpp"
#define BUFFER_EXCHANGE			300000
#define MAX_CLIENT				1024
#define RUNNING					1
#define TIMEOUT_SELECT_SEC		2
#define TIMEOUT_SELECT_USEC		0
#define FILE_EMPTY				"There's no point of running this program with an empty file"
#define START_WITH_THE_BASICS	"config file must start with the word server"
#define FATAL_INIT_ERROR		"No Server was set properly, what a shame, exiting the program"
#define FATAL_LAUNCH_ERROR		"Failed to launch any ports on any servers, exiting the program"


class webservManager
{
	public:
		webservManager(const char *);
		~webservManager();

		void 					run();
		int						isServerFd(int fd);
		void					terminate();

	private:
		// Creating servers
		void						splitConfig(const char *);
		void						splitServer();

		// Manage connections
		void						manageNewConnection(int serverFd);
		Server						&getServerFd(int fd);
		void						resetConnections();
		void						endConnection(int fd);

		// Reading from socket
		void						manageReadingSocks();
		void						manageRecv(std::vector<int>::iterator &it, int recvBytesRead);

		// Writing in sockets
		int							manageWritingSocks();


		std::vector<std::string>	_splittedConfig;
		std::vector<Server>			_vectServers;
		char						_recvBuffer[BUFFER_EXCHANGE];
		std::map<int, Server *>		_mapSocketServer;
		std::map<int, Request>		_mapSocketRequest;
		std::vector<int>			_vectOfReadyToWriteSocket;
		std::vector<int>			_vectOfReadyToReadSocket;
		fd_set						_readSet;
		fd_set						_writeSet;
		fd_set						_currentSet;
		int							_maxFd;
		int							_nbrOfClients;

		// Exceptions
		class	webserverError : public std::exception
		{
			private:
				std::string _message;
			public:
				webserverError(const char *message): _message(message) {}
				webserverError(const std::string &message): _message(message) {}
				const char *what() const throw();
				~webserverError() throw(){};
		};
};

