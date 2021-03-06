/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Poller.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouzaie <mbouzaie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/19 15:35:07 by mbouzaie          #+#    #+#             */
/*   Updated: 2022/03/13 17:06:17 by acastelb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../headers/Poller.hpp"
# include "../headers/Connector.hpp"
# include "../headers/common.hpp"

Poller::Poller(lstnrs &listeners) : _nfds(listeners.size()), _listeners(listeners)
{
    nfds_t i = 0;
    memset(_fds, 0, sizeof(_fds));
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
	for (nfds_t i = 0; i < copy._nfds; i++)
		this->_fds[i] = copy._fds[i];
	this->_nfds = copy._nfds;
}

Poller  &Poller::operator=(const Poller &other)
{
	if (this == &other)
		return (*this);
	for (nfds_t i = 0; i < other._nfds; i++)
		this->_fds[i] = other._fds[i];
	this->_nfds = other._nfds;
	return (*this);
}

const char* Poller::PollFailedException::what() const throw()
{
	return ("Poll error");
}

const char* Poller::max_clients_reached::what() const throw()
{
	return ("Max clients reached");
}



//Prints all fds, only used for testing
void Poller::print_fds()
{
    std::cout << "=";
    for (nfds_t i = 0; i < _nfds; ++i)
        if (_fds[i].fd >= 10 || _fds[i].fd == -1 )
            std::cout << "===";
        else
            std::cout << "==";
    std::cout << "\n";

    std::cout << "|";
    for (nfds_t i = 0; i < _nfds; ++i)
        std::cout << _fds[i].fd << "|";
    std::cout << "\n";


    std::cout << "=";
    for (nfds_t i = 0; i < _nfds; ++i)
        if (_fds[i].fd >= 10 || _fds[i].fd == -1 )
            std::cout << "===";
        else
            std::cout << "==";
    std::cout << "\n";
}

void        Poller::start(void)
{
	int	rc;

	if (should_run == 0)
		return ;
    std::cout << std::endl << BOLD << "Poll called, waiting for readable sockets..." << COLOR_OFF << std::endl;
	rc = poll(_fds, _nfds, -1);
	if (rc < 0 && should_run == 1)
		throw	Poller::PollFailedException();
}

void        Poller::handle(const std::vector<Server_t> &servs)
{
	nfds_t current_sockets = _nfds;
    bool compress = false;

	if (should_run == 0)
		return ;
	for (nfds_t i = 0; i < current_sockets; i++)
	{
		if(_fds[i].revents == 0)
			continue;
        lstnrs::iterator it = _listeners.begin();
        while (it != _listeners.end() && _fds[i].fd != it->getFd())
            ++it;
        if (it != _listeners.end())
        {
            std::cout << std::endl << B_GREEN << "Listening socket " << it->getFd() << " is readable" << COLOR_OFF << std::endl;
            while (1)
            {
                Connector connector(*it);
                if (!connector.accept_c())
                {
                    std::cout << "No more incoming connection on this socket" << std::endl;
                    break;
                }
                std::cout << "   New incoming connection. Descriptor " << connector.getClientSocket() << std::endl;
                _fds[_nfds].fd = connector.getClientSocket();
                _fds[_nfds].events = POLLIN | POLLPRI;
                _listen_map.insert(std::make_pair(_fds[_nfds].fd, &(*it)));
                _client_map.insert(std::make_pair(_fds[_nfds].fd, connector.getClient()));
                ++_nfds;
                if (_nfds == MAX_CLIENTS)
                    throw (max_clients_reached());
            }
        }
        else
		{
			std::cout << std::endl << B_GREEN << "Descriptor " << _fds[i].fd << " is readable. Refers to listening socket " << _listen_map[_fds[i].fd]->getFd() << " on "  << inet_ntoa(_listen_map[_fds[i].fd]->getAddress().sin_addr) << ":" << ntohs(_listen_map[_fds[i].fd]->getAddress().sin_port)  << std::endl;
            Connector connector(*_listen_map[_fds[i].fd]);
            connector.setClient(_client_map[_fds[i].fd]);
			connector.setClientSocket(_fds[i].fd);
			if (connector.handle(servs))
			{
                std::cout << "   Closing descriptor " << _fds[i].fd << std::endl;
                shutdown(_fds[i].fd, SHUT_RDWR);
                close(_fds[i].fd);
                _listen_map.erase(_fds[i].fd);
                _client_map.erase(_fds[i].fd);
				_fds[i].fd = -1;
                compress = true;
			}
		}
	}

    if (compress)
        for (nfds_t i = 0; i < _nfds; ++i)
        {
            if (_fds[i].fd == -1)
            {
                for (nfds_t j = i; j < _nfds; ++j)
                    _fds[j].fd = _fds[j + 1].fd;
                --_nfds;
                --i;
            }
        }
}
