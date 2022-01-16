#include "../headers/Server_t.hpp"


Server_t::Server_t(): _names(std::set<std::string>()),
                      _ports(std::set<in_port_t>())
{
}

Server_t::Server_t(const Server_t &copy): _names(copy._names),
                                          _ports(copy._ports)
{
}


Server_t::~Server_t()
{}

Server_t	&Server_t::operator=(const Server_t &other)
{
    _names = other._names;
    _ports = other._ports;
	return *this;
}

void Server_t::add_name(const std::string name)
{
    _names.insert(name);
}

void Server_t::add_port(const in_port_t port)
{
    _ports.insert(port);
}

bool Server_t::has_name(const std::string name) const
{
    return _names.count(name);
}

bool Server_t::has_port(const in_port_t port) const
{
    return _ports.count(port);
}
