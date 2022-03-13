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

Listener::Listener(const Listener &copy) : _fd(copy._fd), _address(copy._address)
{

}

Listener	&Listener::operator=(const Listener &copy)
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

const sockaddr_in	&Listener::getAddress() const
{
	return (this->_address);
}

void    Listener::close_perror(const char *msg)
{
		perror(msg);
		close(_fd);
		exit(-1);
}

void	Listener::execute(std::string addr, in_port_t port)
{
	this->_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (this->_fd == -1)
		throw Listener::CreationFailedException();
	this->_address.sin_family = AF_INET;
    if (addr == "*")
        this->_address.sin_addr.s_addr = INADDR_ANY;
    else
        this->_address.sin_addr.s_addr = inet_addr(addr.c_str());
	this->_address.sin_port = htons(port);
	this->_address.sin_len = sizeof(struct sockaddr_in);

	int	rc;
    int on = 1;
	rc = setsockopt(_fd, SOL_SOCKET,  SO_REUSEADDR,
				(char *)&on, sizeof(on));
	if (rc < 0)
        close_perror("setsockopt() SO_REUSEADDR failed");

    rc = fcntl(_fd, F_SETFL, fcntl(_fd, F_GETFL, 0) | O_NONBLOCK);
	if (rc < 0)
        close_perror("fctnl() set NONBLOCK failed");

	if (bind(_fd, (struct sockaddr*)&_address, sizeof(sockaddr)) < 0)
		throw Listener::PortBindingFailedException();
	if (listen(this->_fd, 128) < 0)
		throw Listener::ListeningFailedException();
}
