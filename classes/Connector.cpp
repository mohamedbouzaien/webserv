/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Connector.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouzaie <mbouzaie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/11 16:37:13 by mbouzaie          #+#    #+#             */
/*   Updated: 2022/02/09 15:25:08 by acastelb         ###   ########.fr       */
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

int    Connector::handle()
{
	Request request;
	char buffer[30000];
	int	bytesRead = recv(_client_socket, buffer, 30000, 0);
	if (bytesRead < 0)
		throw Connector::RecvFailedException();
	if (bytesRead == 0)
		return (-1);
	std::cout << "The message was: " << buffer << std::endl;
	request.parseRequest(buffer);
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
	memset(buffer, 0, 30000);
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
