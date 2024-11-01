/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thelaine <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/25 11:01:45 by thelaine          #+#    #+#             */
/*   Updated: 2024/09/25 11:01:47 by thelaine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once



#define GET						0
#define POST					1
#define DELETE					2

#define OK						"allowed"
#define KO						"not allowed"

#define NONE					0
#define PYTHON					1
#define PHP						2

#define	SERVER					0
#define SUBLOCATION				1

#define DEFAULT_MAX_CLIENT_BODY	1000000000
#define DEFAULT_SERVER_NAME		"unamed server "

#define OPENING_ERROR			"error while opening the file."
#define SERVER_DEFINE			"servers must be define with server keyworkd"
#define SERVER_START			"servers must start with '{'"
#define NO_LISTEN				"servers must have listenning port"
#define UNEXPECTED_END_OF_FILE	"Parsing encountered an early unexpected end of file"
#define NO_PORT					"servers must have at least one listenning port"
#define NO_ROOT					"servers must have root set up"
#define NO_INDEX				"servers must have index set up"
#define WRONG_AUTO_INDEX		"auto index is not a bool type"
#define WRONG_MAX_CLIENT_BODY	"client body has wrong format"
#define SERVER_END				"servers must end with '}'"
#define CGI_TYPE_UNKNOWN		"cgi used is unknown"
#define LOCATION_START			"locations must start with '{'"
#define LOCATION_END			"locations must end with '}'"
#define SUBLOCATION_ALREADY_SET	"this sublocations has already been defined"

#define MAX_PORT				65535

class Server
{
	public :

		//usefull server manipulation
		virtual ~Server();
		Server(const Server& other);
		Server &operator=(const Server& other);
		Server(std::vector<std::string>::iterator &server, std::vector<std::string>::iterator &wordIteratorEnd);

		//managing the server
		void		start_server(std::map<int, Server *> &, fd_set &currentSet, int &maxFd, int &nbrOfClients, std::vector<int> &vectOfReadyToReadSocket, std::vector<int> &vectAlreadyUsedPort);
		void		reset_server(std::map<int, Server *> &, fd_set &currentSet, int &maxFd, std::vector<int> &vectOfReadyToReadSocket);


		int									isThisFdOnThisServer(int client_fd);
		void								printActualListenningPorts();
		Server								*goToActualSublocation(std::string &uri);

		//getter
		const std::string					&getName() const;
		const std::string 					&getRoot() const;
		const std::string 					&getDefaultRequest() const;
		const bool 							*getAllowed() const;
		const ull							&getMaxClientBody() const;
		const std::string					&getSpecialLocation() const;
		const int 							&getCgiType() const;
		const std::map<int, std::string>	&getMapErrorPages() const;
		const std::string					&getFavicon() const;
		const bool							&getAutoIndex() const;

		//usefull print
		void								printServer();
		//ERROR CLASS
		class	ServerError : public std::exception {
			private:
				std::string _message;
			public:
				ServerError(const char *message): _message(message) {}
				ServerError(const std::string &message): _message(message) {}
				const char *what() const throw();
				~ServerError() throw(){};
		};
	private :
		//function to create location
		Server(const Server& other, std::vector<std::string> &alreadyExistingSubLocations);


		std::string							_name;
		std::vector<int>					_listenningPort;
		std::vector<int>					_actuallyListenningPort;
		std::string							_root;
		std::string							_index;
		bool								_allowed[3];
		ull									_maxClientBody;
		std::vector<Server>					_subLocations;
		int									_cgiType;
		std::string							_specialLocation;
		std::vector<int>					socketFds;
		std::map<int, std::string>			_mapErrorPages;
		std::string							_favicon;
		bool								_autoIndex;

		std::vector<std::string>::iterator	&_wordIterator;
		std::vector<std::string>::iterator	&_wordIteratorEnd;
		std::string							_word;

		//ServerUtils
		std::string							incrementWord();
		//CreationServer
		void								checkStart(int mode);
		void								setName();
		void								setListenningPorts();
		void								setRoot(int mode);
		void								setIndex(int mode);
		void								setAutoIndex();
		void								setErrorPages();
		void								setFavicon();
		void								setRules();
		void								setMaxClientBody();
		void								setSubLocation(std::vector<std::string> &alreadyExistingSubLocations);
		void								checkIfSubLocationAlreadyExisting(std::vector<std::string> &alreadyExistingSubLocations);
		void								setCgiType();
		void								checkEnd(int mode);

		//startingServer
		void								checkMaxClient(int nbrOfClients);
		void								checkPortAlreadyInUse(std::vector<int> &vectAlreadyUsedPort, int portStudied);
		int									createSocketForPort(int portStudied);
		void								setSocketOpt(int portSocketFd, int portStudied);
		void								bindPortSocketFd(int portSocketFd, int portStudied);
		void								listenPortFd(int portSocketFd, int portStudied);
		void								updateSocketPortData(std::map<int, Server *> &map_socket_server, fd_set &currentSet, int &maxFd, int &nbrOfClients,
											std::vector<int> &vectOfReadyToReadSocket, std::vector<int> &vectAlreadyUsedPort, int portSocketFd, int portStudied);
		void								printServerStartStatus();
};

