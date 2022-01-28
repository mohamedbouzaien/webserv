#include "../headers/Location_t.hpp"


Location_t::Location_t(const std::string uri): _uri(uri),
                                               _locations(std::vector<Location_t>())
{
}

Location_t::Location_t(const Location_t &copy): Context_t(copy),
                                                _uri(copy._uri),
                                                _locations(copy._locations)
{
}


Location_t::~Location_t()
{}

Location_t	&Location_t::operator=(const Location_t &other)
{
    _uri = other._uri;
    _locations = other._locations;
	return *this;
}

// Setters ---------------------------------

void Location_t::add_location(const Location_t *loc)
{
    _locations.push_back(*loc);
}

// Getters ----------------------------------

std::vector<Location_t> &Location_t::get_locations(){
    return _locations;
}

const std::string &Location_t::get_uri() const
{
    return _uri;
}

#include <iostream>
void Location_t::print(){
    std::cout << "  Location content:\n";
    std::cout << "    uri: " << _uri << "\n";
    std::cout << "    root: " << _root << "\n";
    std::cout << "    _auto_index: " << _auto_index << '\n';
    std::cout << "    _client_max_body_size: " << _client_max_body_size << '\n';

    std::cout << "    _allow_methods: ";
    if (is_allowed_get())
        std::cout << "GET ";
    if (is_allowed_post())
        std::cout << "POST ";
    if (is_allowed_delete())
        std::cout << "DELETE ";
    std::cout << "\n";

    std::cout << "    index:\n";
    for (std::list<std::string>::iterator it = _index.begin();
            it != _index.end(); ++it)
        std::cout << "     -" << *it << '\n';

    std::cout << "    error_pages:\n";
    for (std::map<std::string, std::string>::iterator it = _error_pages.begin(); it != _error_pages.end(); ++it)
        std::cout << "     -" << it->first << " / " << it->second << '\n';


    for (std::vector<Location_t>::iterator it = _locations.begin();
            it != _locations.end(); ++it) {
        std::cout << "---- SubLocation " << it->_uri << "----\n";
        it->print();
        std::cout << "---- End of " << it->_uri << "----\n";
    }

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
