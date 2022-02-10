/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Connector.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouzaie <mbouzaie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/11 16:37:13 by mbouzaie          #+#    #+#             */
/*   Updated: 2022/02/10 10:03:46 by acastelb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/Connector.hpp"
#include "../headers/Request.hpp"
#include "../headers/Cgi.hpp"

Connector::Connector(Listener &listener): _listener(listener)
{
	_client_socket = 0;
}

Connector::Connector(Connector &copy): _client_socket(copy._client_socket), _listener(copy._listener)
{
	
}

Connector	&Connector::operator=(Connector &other)
{
	if (this == &other)
		return (*this);
	this->_listener = other._listener;
	this->_client_socket = other._client_socket;
	return (*this);
}

const char* Connector::ConnectionFailedException::what() const throw()
{
	return ("Failed to grab connection");
}

const char* Connector::RecvFailedException::what() const throw()
{
	return ("Recv error");
}

void    Connector::accept_c()
{
	socklen_t	addrlen = sizeof(this->_listener.getAddress());
	_client_socket = accept(this->_listener.getFd(), (struct sockaddr*)&(_listener.getAddress()), &addrlen);
	if (_client_socket < 0)
		throw Connector::ConnectionFailedException();
}

#define BUFFER_SIZE 30

int    Connector::handle()
{
	Request request;
	std::string s_buffer;
	char buffer[BUFFER_SIZE + 1];
	int	bytesRead;
	
	std::cout << "\033[1;31m--- Exchange Started ---\033[0m\n";
	while (s_buffer.find("\r\n\r\n") == std::string::npos) {
		memset(buffer, 0, BUFFER_SIZE + 1);
		bytesRead = recv(_client_socket, buffer, BUFFER_SIZE, 0);
		s_buffer += buffer;
		if (bytesRead < 0)
			throw Connector::RecvFailedException();
		if (bytesRead == 0)
			return (-1);
	}

	std::cout << "The message was: " << s_buffer << std::endl;
	request.parseRequest((char *)s_buffer.c_str());
	std::cout << request << std::endl;
	std::string s("bin/php-cgi"); // Path to cgi binary
	Cgi cgi((char *)s.c_str(), request); // Cgi constr.
	cgi.runCgi(request); // run Cgi
	char *output = cgi.getOutput(); // get Cgi result, use getStatusCode for status code (int)
	char *body = strstr(output, "\r\n\r\n"); // get output body
	body += 4; // skip \r\n\r\n
	std::string result = ("HTTP/1.1 200 OK\nContent-Length: " + std::to_string(strlen(body)) + "\n" + output); // build test response
	send(_client_socket, result.c_str(), result.size(), 0);
	request.clear();
	memset(buffer, 0, BUFFER_SIZE + 1);
	std::cout << "\033[1;31m---- Exchange Ended ----\033[0m\n";
	return (0);
}

void	Connector::setClientSocket(int client_socket)
{
	this->_client_socket = client_socket;
}

int		Connector::getClientSocket()	const
{
	return (this->_client_socket);
}
