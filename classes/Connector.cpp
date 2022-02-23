/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Connector.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouzaie <mbouzaie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/11 16:37:13 by mbouzaie          #+#    #+#             */
/*   Updated: 2022/02/15 22:20:53 by mbouzaie         ###   ########.fr       */
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

const Server_t &Connector::choose_serv(const std::vector<Server_t> &servs, const std::string host) const
{
    std::list<const Server_t*> possible;

    if (_listener.getAddress().sin_addr.s_addr != INADDR_ANY) // Find every server w/ exact ip:port correspondance
    {
        for (std::vector<Server_t>::const_iterator serv_it = servs.begin(); serv_it != servs.end(); ++serv_it)
            for (std::set<Server_t::listen_pair_t>::iterator lstn_it = serv_it->get_listen().begin(); lstn_it != serv_it->get_listen().end(); ++lstn_it)
                if (htons(lstn_it->second) == _listener.getAddress().sin_port
                        && inet_addr(lstn_it->first.c_str()) == _listener.getAddress().sin_addr.s_addr
                        && std::find(possible.begin(), possible.end(), &(*serv_it)) == possible.end())
                    possible.push_back(&(*serv_it));
    }
    if (possible.empty()) // if no matching listen or listening on 0.0.0.0, search every 0.0.0.0 with same port
    {
        for (std::vector<Server_t>::const_iterator serv_it = servs.begin(); serv_it != servs.end(); ++serv_it)
            for (std::set<Server_t::listen_pair_t>::iterator lstn_it = serv_it->get_listen().begin(); lstn_it != serv_it->get_listen().end(); ++lstn_it)
                if (htons(lstn_it->second) == _listener.getAddress().sin_port
                        && std::find(possible.begin(), possible.end(), &(*serv_it)) == possible.end())
                    possible.push_back(&(*serv_it));
    }

    /* TESTING
    // if (!possible.size())
    //    std::cout << "IL Y A UN GROOOOOS PROBLEME DANS LA FONCTION CHOOSE_SERVER" << std::endl;
    for (std::list<const Server_t*>::const_iterator serv_it = possible.begin(); serv_it != possible.end(); ++serv_it)
        (*serv_it)->print(); // TESTING
    */

    // If only one possibility, return it
    if (possible.size() == 1)
        return **possible.begin();
    // Multiple servers with same priority, analyze server_name
    for (std::list<const Server_t*>::const_iterator serv_it = possible.begin(); serv_it != possible.end(); ++serv_it)
        if ((*serv_it)->has_name(host))
            return **serv_it;
    // return first possible if no matching name found
    return *possible.front();
}

int    Connector::handle(const std::vector<Server_t> &servs)
{
	Request		request;

	char buffer[30000];
	int	bytesRead = recv(_client_socket, buffer, 30000, 0);
	if (bytesRead < 0)
		throw Connector::RecvFailedException();
	if (bytesRead == 0)
		return (-1);

	std::cout << buffer << std::endl;
	request.parseRequest(buffer);
	std::cout << request << std::endl;

	Response	response(choose_serv(servs, request.getHost().first));

	response.prepare(request);
	std::string hello = response.parse();
	send(_client_socket, hello.c_str(), hello.size(), 0);
	request.clear();
	memset(buffer, 0, 30000);
	return (0);
}

void	Connector::setClientSocket(int client_socket)
{
	this->_client_socket = client_socket;
}

int		Connector::getClientSocket()	const
{
	return (this->_client_socket);
}
