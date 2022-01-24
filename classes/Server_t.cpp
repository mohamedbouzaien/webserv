#include "../headers/Server_t.hpp"


Server_t::Server_t(): _names(std::set<std::string>()),
                      _listen(std::set<listen_pair_t>()),
                      _locations(std::vector<Location_t>())
{
}

Server_t::Server_t(const Server_t &copy): Context_t(copy),
                                          _names(copy._names),
                                          _listen(copy._listen),
                                          _locations(copy._locations)
{
}


Server_t::~Server_t()
{}

Server_t	&Server_t::operator=(const Server_t &other)
{
    _names = other._names;
    _listen = other._listen;
    _locations = other._locations;
	return *this;
}

// Setters ---------------------------------
void Server_t::add_name(const std::string &name)
{
    _names.insert(name);
}

void Server_t::add_listen(const listen_pair_t &listen_pair)
{
    _listen.insert(listen_pair);
}

void Server_t::add_location(const Location_t &loc)
{
    _locations.push_back(loc);
}

// Getters ----------------------------------
std::set<std::string>   &Server_t::get_names(){
    return _names;
}
std::set<Server_t::listen_pair_t> &Server_t::get_listen(){
    return _listen;
}
std::vector<Location_t> &Server_t::get_locations(){
    return _locations;
}


// checks --------------------
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

    std::cout << "  root: " << _root << "\n";

    std::cout << "  index:\n";
    for (std::list<std::string>::iterator it = _index.begin();
            it != _index.end(); ++it)
        std::cout << "   -" << *it << '\n';

    std::cout << "  _auto_index: " << _auto_index << '\n';
    std::cout << "  _client_max_body_size: " << _client_max_body_size << '\n';


    if  (_locations.size())
        std::cout << "  locations: \n";
    for (std::vector<Location_t>::iterator it = _locations.begin();
            it != _locations.end(); ++it)
        it->print();


}
