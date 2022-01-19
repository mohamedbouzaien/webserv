/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Connector.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouzaie <mbouzaie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/11 16:37:13 by mbouzaie          #+#    #+#             */
/*   Updated: 2022/01/17 18:04:20 by mbouzaie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/Connector.hpp"

Connector::Connector(Listener &listener): _listener(listener)
{
	_client_socket = 0;
	_fds[0].fd = listener.getFd();
	_fds[0].events = POLLIN | POLLPRI;
	_nfds = 1;
}

Connector::Connector(Connector &copy)
{
	(void)copy;
}

Connector	&Connector::operator=(Connector &other)
{
	//to do
	return (other);
}

const char* Connector::ConnectionFailedException::what() const throw()
{
	return ("Failed to grab connection");
}

const char* Connector::RecvFailedException::what() const throw()
{
	return ("Recv error");
}

const char* Connector::PollFailedException::what() const throw()
{
	return ("Poll error");
}

void	Connector::poll_server()
{
	int	rc;

	rc = poll(_fds, _nfds, -1);
	if (rc < 0)
		throw	Connector::PollFailedException();
}

void    Connector::accept_c()
{
	socklen_t	addrlen = sizeof(this->_listener.getAddress());
	_client_socket = accept(this->_listener.getFd(), (struct sockaddr*)&(_listener.getAddress()), &addrlen);
	if (_client_socket < 0)
		throw Connector::ConnectionFailedException();
}

void    Connector::handle()
{
	char buffer[30000];
	int	bytesRead = recv(_client_socket, buffer, 30000, 0);
	if (bytesRead < 0)
		throw Connector::RecvFailedException();
	std::cout << "The message was: " << buffer;

	std::string hello = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 12\n\nHello world!";
	send(_client_socket, hello.c_str(), hello.size(), 0);
}

void	Connector::setClientSocket(int client_socket)
{
	this->_client_socket = client_socket;
}

int		Connector::getClientSocket()	const
{
	return (this->_client_socket);
}