/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouzaie <mbouzaie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/11 15:47:08 by mbouzaie          #+#    #+#             */
/*   Updated: 2022/01/11 22:11:24 by mbouzaie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/Socket.hpp"

Socket::Socket()
{


}

Socket::Socket(Socket &copy) : _fd(copy._fd), _address(copy._address)
{

}

Socket	&Socket::operator=(Socket &copy)
{
	//to do
	return (copy);
}

const char* Socket::CreationFailedException::what() const throw()
{
	return ("Failed to create socket");
}

const char* Socket::PortBindingFailedException::what() const throw()
{
	return ("Failed to bind to port 8080");
}

const char* Socket::ListeningFailedException::what() const throw()
{
	return ("Failed to listen on socket");
}

int			Socket::getFd()
{
	return (this->_fd);
}

sockaddr_in	&Socket::getAddress()
{
	return (this->_address);
}

void	Socket::execute()
{
	this->_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (this->_fd == -1)
		throw Socket::CreationFailedException();
	this->_address.sin_family = AF_INET;
	this->_address.sin_addr.s_addr = INADDR_ANY;
	this->_address.sin_port = htons(PORT);
	if (bind(_fd, (struct sockaddr*)&_address, sizeof(sockaddr)) < 0)
		throw Socket::PortBindingFailedException();
	if (listen(this->_fd, 10) < 0)
		throw Socket::ListeningFailedException();
}
