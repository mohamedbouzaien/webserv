#include "../headers/Server_t.hpp"


Server_t::Server_t(): _names(std::set<std::string>()),
                      _listen(std::set<listen_pair_t>())
{
}

Server_t::Server_t(const Server_t &copy): _names(copy._names),
                                          _listen(copy._listen)
{
}


Server_t::~Server_t()
{}

Server_t	&Server_t::operator=(const Server_t &other)
{
    _names = other._names;
    _listen = other._listen;
	return *this;
}

void Server_t::add_name(const std::string &name)
{
    _names.insert(name);
}

void Server_t::add_listen(const listen_pair_t &listen_pair)
{
    _listen.insert(listen_pair);
}

bool Server_t::has_name(const std::string name) const
{
    return _names.count(name);
}

bool Server_t::has_listen(const listen_pair_t listen_pair) const
{
    return _listen.count(listen_pair);
}

bool Server_t::names_empty() const
{
    return _names.empty();
}

bool Server_t::listen_empty() const
{
    return _listen.empty();
}

#include <iostream>
void Server_t::print(){
    std::cout << "Server content:\n";
    std::cout << "  server names:\n";
    for (std::set<std::string>::iterator it = _names.begin();
            it != _names.end(); ++it)
        std::cout << "   -" << *it << '\n';
    std::cout << "  listen ip:port pairs:\n";
    for (std::set<listen_pair_t>::iterator it = _listen.begin();
            it != _listen.end(); ++it)
        std::cout << "   -" << it->first << ':' << it->second << '\n';

}
