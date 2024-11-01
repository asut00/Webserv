/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asuteau <asuteau@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/23 15:07:02 by asuteau           #+#    #+#             */
/*   Updated: 2024/10/09 14:55:36 by asuteau          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "Webserv.hpp"

#define FILE "config_file/default.conf"
// #define FILE2 "config_file/config_file2.conf"
// #define FILE3 "config_file/config_file3.conf"
// #define FILE4 "config_file/config_file4.conf"

int	main(int ac, char **av)
{
	signal(SIGPIPE, SIG_IGN);

	std::string file(FILE);
	if (ac != 2)
		std::cout << YELLOW << "Argument number is different than 2, starting with default config: " << FILE <<  DFLT << std::endl;
	else
	{
		std::string newfile(av[1]);
		file = newfile;
	}

	initHttpStatusCode();
	try
	{
		webservManager webservManager(file.c_str());
		webservManager.run();
	}
	catch (std::exception &e)
	{
		std::cout << RED << e.what() << DFLT << std::endl;
	}
	return 0;
}
