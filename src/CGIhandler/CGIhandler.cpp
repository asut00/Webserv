/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGIhandler.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thelaine <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/02 11:54:19 by thelaine          #+#    #+#             */
/*   Updated: 2024/10/02 11:54:21 by thelaine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Webserv.hpp"

pid_t child_pid = -1;

CGIhandler::CGIhandler(std::string uri, std::string root, std::string method, int cgi_type, ull content_length, std::map<std::string, std::string> headers)
{
	_scriptName = root + uri.substr(0, uri.find('?'));
	_args = "QUERY_STRING=";
	if (uri.find('?') != std::string::npos)
		_args += uri.substr(uri.find('?') + 1);
	_pathOfExec = PATH_TO_PYTHON;
	if (cgi_type == PHP)
		_pathOfExec = PATH_TO_PHP;
	_method = "REQUEST_METHOD=" + method;
	_contentLength = "CONTENT_LENGTH=" + headers["Content-Length"];
	_contentType = "CONTENT_TYPE=" + headers["Content-Type"];
	_pathInfo =  "PATH_INFO=" + uri;
	_size = content_length;
}


CGIhandler::~CGIhandler()
{
}

void timeout_handler(int signum)
{
	if (signum == SIGALRM && child_pid > 0)
		kill(child_pid, SIGKILL);
}

int CGIhandler::HandleCGIRequest(webservManager *webservManager, char *body)
{
	signal(SIGALRM, timeout_handler);
	FILE *file_In = 0;

	int saved_std_in = dup(STDIN_FILENO);
	int saved_std_out = dup(STDOUT_FILENO);
	int fd_out = open(CGI_ANSWER, O_WRONLY | O_TRUNC | O_CREAT, S_IRUSR | S_IWUSR | S_IROTH);
	if (fd_out == FAILED)
	{
		close(saved_std_in);
		close(saved_std_out);
		std::cerr << "Error: couldn't open the file to answer CGI" << std::endl;
		return HTTP_INTERNAL_SERVER_ERROR;
	}
	file_In = tmpfile();
	if (file_In == 0)
	{
		std::cerr << "Error: couldn't create a temporary file" << std::endl;
		return HTTP_INTERNAL_SERVER_ERROR;
	}
	if (_method == "REQUEST_METHOD=POST")
	{
		fwrite(body, sizeof(char), _size, file_In);
		fflush(file_In);
		rewind(file_In);
	}
	child_pid = fork();

	if (child_pid < 0)
	{
		std::cerr << RED << "Error: fork function failed" << DFLT << std::endl;
		return HTTP_INTERNAL_SERVER_ERROR;
	}
	if (child_pid == 0)
	{
		webservManager->terminate();
		int ret_dup_out = dup2(fd_out, STDOUT_FILENO);
		int ret_dup_in = dup2(fileno(file_In), STDIN_FILENO);
		close (fd_out);
		fclose(file_In);
		close(saved_std_in);
		close(saved_std_out);
		if (ret_dup_out == FAILED || ret_dup_in == FAILED)
		{
			std::cerr << RED << "Error: dup2 function failed" << DFLT << std::endl;
			exit(EXIT_FAILURE);
		}
		char *env[] = {&_args[0],
		&_method[0], &_contentLength[0], &_contentType[0], &_pathInfo[0], NULL};

		char *args[] = {&_pathOfExec[0],
		&_scriptName[0], NULL};
		if (execve(&_pathOfExec[0], args, env) == FAILED)
		{
			// perror("execve failed");
			std::cerr << RED << "Error: excve function failed" << DFLT << std::endl;
			exit(EXIT_FAILURE);
		}
	}
	else
	{
		close(fd_out);
		fclose(file_In);
		dup2(saved_std_in, STDIN_FILENO);
		dup2(saved_std_out, STDOUT_FILENO);
		close(saved_std_in);
		close(saved_std_out);
		alarm(5);
		int status = 0;
		waitpid(child_pid, &status, 0);
		alarm(0);
		if (WIFEXITED(status) && WEXITSTATUS(status))
			return HTTP_INTERNAL_SERVER_ERROR;
		if (WIFSIGNALED(status))
		{
			std::cerr << RED << "Error: Python script took too long and got cancelled" << DFLT << std::endl;
			return HTTP_GATEWAY_TIMEOUT;
		}
		return HTTP_OK;
	}
	return HTTP_OK;
}
