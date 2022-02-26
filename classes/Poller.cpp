/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Poller.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouzaie <mbouzaie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/19 15:35:07 by mbouzaie          #+#    #+#             */
/*   Updated: 2022/02/24 09:44:33 by acastelb         ###   ########.fr       */
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

    std::cout << "Poll called !\n";
	rc = poll(_fds, _nfds, -1);
	if (rc < 0)
		throw	Poller::PollFailedException();
}

void        Poller::handle(const std::vector<Server_t> &servs)
{
	int current_sockets = _nfds;
    bool compress = false;

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
            _listen_map.insert(std::make_pair(_fds[_nfds].fd, &(*it)));
            ++_nfds;
        }
        else
		{
			std::cout << "  Descriptor " << _fds[i].fd << " is readable. Refers to listen descriptor " << _listen_map[_fds[i].fd]->getFd() << " on "  << inet_ntoa(_listen_map[_fds[i].fd]->getAddress().sin_addr) << ":" << ntohs(_listen_map[_fds[i].fd]->getAddress().sin_port)  << std::endl;
            Connector connector(*_listen_map[_fds[i].fd]);
			connector.setClientSocket(_fds[i].fd);
			if (connector.handle(servs))
			{
                std::cout << "   Closing descriptor " << _fds[i].fd << std::endl;
                close(_fds[i].fd);
                _listen_map.erase(_fds[i].fd);
				_fds[i].fd = -1;
                compress = true;
			}
		}
	}

    if (compress)
        for (int i = 0; i < _nfds; ++i)
            if (_fds[i].fd == -1)
            {
                for (int j = i; j < _nfds; ++j)
                    _fds[j].fd = _fds[j + 1].fd;
                --_nfds;
            }
}
