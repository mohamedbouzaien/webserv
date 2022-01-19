/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Poller.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouzaie <mbouzaie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/19 15:35:07 by mbouzaie          #+#    #+#             */
/*   Updated: 2022/01/19 19:20:31 by mbouzaie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../headers/Poller.hpp"
# include "../headers/Connector.hpp"

Poller::Poller(Listener &listener) : _nfds(1), _listener(listener)
{
	_fds[0].fd = listener.getFd();
	_fds[0].events = POLLIN | POLLPRI;
}

Poller::Poller(const Poller &copy)
{
	for (int i = 0; i < copy._nfds; i++)
		this->_fds[i] = copy._fds[i];
	this->_nfds = copy._nfds;
}

Poller  &Poller::operator=(const Poller &other)
{
	if (this == &other)
		return (*this);
	for (int i = 0; i < other._nfds; i++)
		this->_fds[i] = other._fds[i];
	this->_nfds = other._nfds;
	return (*this);
}

const char* Poller::PollFailedException::what() const throw()
{
	return ("Poll error");
}

void        Poller::start(void)
{
	int	rc;

	rc = poll(_fds, _nfds, -1);
	if (rc < 0)
		throw	Poller::PollFailedException();
}

void        Poller::handle(void)
{
	int current_sockets;
	Connector	connector(_listener);

	current_sockets = _nfds;
	for (int i = 0; i < current_sockets; i++)
	{
		if(_fds[i].revents == 0)
			continue;
		if (_fds[i].fd == _listener.getFd())
		{
			std::cout << "  Listening socket is readable" << std::endl;
			connector.accept_c();
			std::cout << "  New incoming connection - " << connector.getClientSocket() << std::endl;
			_fds[_nfds].fd = connector.getClientSocket();
			_fds[_nfds].events = POLLIN | POLLPRI;
			_nfds++;
			}
		else
		{	
			std::cout << "  Descriptor " << _fds[i].fd << " is readable" << std::endl;
			connector.setClientSocket(_fds[i].fd);
			if (connector.handle() < 0)
			{
				_fds[i].fd = 0;
				_fds[i].events = 0;
				_fds[i].revents = 0;
				_nfds--;
			}
		}
	}
}
