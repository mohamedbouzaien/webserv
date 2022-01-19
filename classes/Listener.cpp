/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Listener.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouzaie <mbouzaie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/11 15:47:08 by mbouzaie          #+#    #+#             */
/*   Updated: 2022/01/19 19:16:30 by mbouzaie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/Listener.hpp"
# include <sys/ioctl.h>
# include <unistd.h>

Listener::Listener()
{


}

Listener::Listener(Listener &copy) : _fd(copy._fd), _address(copy._address)
{

}

Listener	&Listener::operator=(Listener &copy)
{
	if (this == &copy)
		return (*this);
	_fd = copy._fd;
	_address = copy._address;
	return (*this);
}

const char* Listener::CreationFailedException::what() const throw()
{
	return ("Failed to create socket");
}

const char* Listener::PortBindingFailedException::what() const throw()
{
	return ("Failed to bind to port ");
}

const char* Listener::ListeningFailedException::what() const throw()
{
	return ("Failed to listen on socket");
}

int			Listener::getFd()
{
	return (this->_fd);
}

sockaddr_in	&Listener::getAddress()
{
	return (this->_address);
}

void	Listener::execute()
{
	this->_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (this->_fd == -1)
		throw Listener::CreationFailedException();
	this->_address.sin_family = AF_INET;
	this->_address.sin_addr.s_addr = INADDR_ANY;
	this->_address.sin_port = htons(PORT);
	int	on, rc;
	rc = setsockopt(_fd, SOL_SOCKET,  SO_REUSEADDR,
				(char *)&on, sizeof(on));
	if (rc < 0)
	{
		perror("setsockopt() failed");
		close(_fd);
		exit(-1);
	}
	if (bind(_fd, (struct sockaddr*)&_address, sizeof(sockaddr)) < 0)
		throw Listener::PortBindingFailedException();
	if (listen(this->_fd, 10) < 0)
		throw Listener::ListeningFailedException();
}
