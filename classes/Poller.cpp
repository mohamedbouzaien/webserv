/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Poller.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouzaie <mbouzaie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/19 15:35:07 by mbouzaie          #+#    #+#             */
/*   Updated: 2022/02/09 18:26:43 by mbouzaie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../headers/Poller.hpp"
# include "../headers/Connector.hpp"

Poller::Poller(lstnrs &listeners) : _nfds(listeners.size()), _listeners(listeners)
{
    unsigned int i = 0;
    for (lstnrs::iterator it = _listeners.begin(); it != _listeners.end(); ++it)
    {
        _fds[i].fd = it->getFd();
        _fds[i].events = POLLIN | POLLPRI;
        ++i;
    }
}

Poller::Poller(const Poller &copy):
    _listeners(copy._listeners)
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

void        Poller::handle(const Server_t &serv_conf)
{
	int current_sockets;

	current_sockets = _nfds;
	for (int i = 0; i < current_sockets; i++)
	{
		if(_fds[i].revents == 0)
			continue;
        lstnrs::iterator it = _listeners.begin();
        while (it != _listeners.end() && _fds[i].fd != it->getFd())
            ++it;
        if (it != _listeners.end())
        {
            Connector connector(*it);
            std::cout << "  Listening socket is readable" << std::endl;
            connector.accept_c();
            std::cout << "  New incoming connection - " << connector.getClientSocket() << std::endl;
            _fds[_nfds].fd = connector.getClientSocket();
            _fds[_nfds].events = POLLIN | POLLPRI;
            _index_map.insert(std::make_pair(_nfds, &(*it)));
            ++_nfds;
        }
        else
		{
			std::cout << "  Descriptor " << _fds[i].fd << " is readable. Refers to listen descriptor " << _index_map[i]->getFd() << std::endl;
            Connector connector(*_index_map[i]);
			connector.setClientSocket(_fds[i].fd);
			if (connector.handle(serv_conf) < 0)
			{
                std::cout << "   Closing descriptor " << _fds[i].fd << std::endl;
                close(_fds[i].fd);
				_fds[i].fd = 0;
				_fds[i].events = 0;
				_fds[i].revents = 0;
				--_nfds;
			}
		}
	}
}
