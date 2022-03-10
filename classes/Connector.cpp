/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Connector.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouzaie <mbouzaie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/11 16:37:13 by mbouzaie          #+#    #+#             */
/*   Updated: 2022/03/10 09:49:18 by acastelb         ###   ########.fr       */
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

bool    Connector::accept_c()
{
	socklen_t	addrlen = sizeof(this->_listener.getAddress());
	_client_socket = accept(this->_listener.getFd(), (struct sockaddr*)&(_listener.getAddress()), &addrlen);
	if (_client_socket < 0)
    {
        if (errno == EAGAIN)
            return false;
        else
            throw Connector::ConnectionFailedException();
    }
    return true;
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
	Request request(_client_socket);
	int status;

	std::cout << "\033[1;31m--- Exchange Started ---\033[0m\n";
	if ((status = request.readAndParseHeader()) < 1) {
		std::cout << "\033[1;31m--- Exchange Ended ---\033[0m\n";
		return (status);
	}
	const Server_t &current_serv = (choose_serv(servs, request.getHost().first));
	if ((status = request.readAndParseBody(status, current_serv.get_best_client_max_body_size(request.getPath()))) < 1) {
		std::cout << "\033[1;31m--- Exchange Ended ---\033[0m\n";
		return (status);
	}
	std::cout << request << std::endl;

	Response	response(choose_serv(servs, request.getHost().first));
	response.prepare(request);
	std::string hello = response.parse();
	send(_client_socket, hello.c_str(), hello.size(), 0);
	std::cout << "\033[1;31m--- Exchange Ended ---\033[0m\n";
	if (request.search("Connection") == "keep-alive")
		return (0);
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
