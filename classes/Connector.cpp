/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Connector.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouzaie <mbouzaie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/11 16:37:13 by mbouzaie          #+#    #+#             */
/*   Updated: 2022/01/13 15:16:18 by acastelb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/Connector.hpp"
#include "../headers/Request.hpp"

Connector::Connector(Listener &listener): _listener(listener)
{

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

void    Connector::accept_c()
{
	socklen_t	addrlen = sizeof(this->_listener.getAddress());
	_client_socket = accept(this->_listener.getFd(), (struct sockaddr*)&(_listener.getAddress()), &addrlen);
	if (_client_socket < 0)
		throw Connector::ConnectionFailedException();
}

void    Connector::handle()
{
	Request request;
	char buffer[30000];
	int	bytesRead = read(_client_socket, buffer, 30000);
	if (bytesRead < 0)
		throw Connector::RecvFailedException();
	std::cout << "The message was: " << buffer << std::endl;

	request.parseRequest(buffer);
	std::string hello = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 12\n\nHello world!";
	send(_client_socket, hello.c_str(), hello.size(), 0);
	close(_client_socket);
}
