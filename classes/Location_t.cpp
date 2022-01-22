#include "../headers/Location_t.hpp"


Location_t::Location_t(const std::string uri): _uri(uri)
{
}

Location_t::Location_t(const Location_t &copy): Context_t(copy),
                                                _uri(copy._uri)
{
}


Location_t::~Location_t()
{}

Location_t	&Location_t::operator=(const Location_t &other)
{
    _uri = other._uri;
	return *this;
}


#include <iostream>
void Location_t::print(){
    std::cout << "  Location content:\n";
    std::cout << "    uri: " << _uri << "\n";
    std::cout << "    root: " << _root << "\n";
    /*
    for (std::set<std::string>::iterator it = _names.begin();
            it != _names.end(); ++it)
        std::cout << "   -" << *it << '\n';
    std::cout << "  listen ip:port pairs:\n";
    for (std::set<listen_pair_t>::iterator it = _listen.begin();
            it != _listen.end(); ++it)
        std::cout << "   -" << it->first << ':' << it->second << '\n';
    */

}
