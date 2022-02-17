/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Connector.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouzaie <mbouzaie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/11 16:37:13 by mbouzaie          #+#    #+#             */
/*   Updated: 2022/02/17 14:34:17 by acastelb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/Connector.hpp"
#include "../headers/Request.hpp"
#include "../headers/Cgi.hpp"

Connector::Connector(Listener &listener): _listener(listener)
{
	_client_socket = 0;
}

Connector::Connector(const Connector &copy): _client_socket(copy._client_socket), _listener(copy._listener)
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

int    Connector::handle(const Server_t &serv_conf)
{
	Request request(_client_socket);
	Response	response;
	int status;

	std::cout << "\033[1;31m--- Exchange Started ---\033[0m\n";
	if ((status = request.handle()) < 1)
		return (status);
	std::cout << request << std::endl;
	response.prepare(request, serv_conf);
	std::string hello = response.parse();
	send(_client_socket, hello.c_str(), hello.size(), 0);
	request.clear();
	std::cout << "\033[1;31m--- Exchange Ended ---\033[0m\n";
	return (1);
}

void	Connector::setClientSocket(int client_socket)
{
	this->_client_socket = client_socket;
}

int		Connector::getClientSocket()	const
{
	return (this->_client_socket);
}
