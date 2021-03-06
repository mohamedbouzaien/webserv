#include "../headers/Location_t.hpp"

Location_t::Location_t()
{
}

Location_t::Location_t(const std::string uri): _uri(uri),
                                               _locations(std::vector<Location_t>()),
                                               _alias(DEFAULT_ALIAS)
{
}

Location_t::Location_t(const Location_t &copy): Context_t(copy),
                                                _uri(copy._uri),
                                                _locations(copy._locations),
                                                _alias(copy._alias)
{
}


Location_t::~Location_t()
{}

Location_t	&Location_t::operator=(const Location_t &other)
{
    _uri = other._uri;
    _locations = other._locations;
    _alias = other._alias;
    Context_t::operator=(other);
	return *this;
}

// Setters ---------------------------------

void Location_t::add_location(const Location_t *loc)
{
    _locations.push_back(*loc);
}

void Location_t::set_alias(const std::string &alias)
{
    _alias = alias;
}

// Getters ----------------------------------

std::vector<Location_t> &Location_t::get_locations(){
    return _locations;
}

const std::string &Location_t::get_uri() const
{
    return _uri;
}

const std::string &Location_t::get_alias() const
{
    return _alias;
}

Location_t Location_t::get_best_location_block(std::string path) const {
	std::string path_tried;
	std::string uri;
	size_t pos;

	path_tried = path;
	if (_locations.begin() == _locations.end())
		return (*this);
	while (1) {
		for(std::vector<Location_t>::const_iterator it = _locations.begin(); it != _locations.end(); it++) {
			uri = it->get_uri();
			if (uri == path_tried || uri == path_tried + "/") {
				return (it->get_best_location_block(path));
			}
		}
		if (path_tried.empty())
			break;
		pos = path_tried.find_last_of('/');
		if (pos == std::string::npos)
			pos = 0;
		path_tried.erase(pos);
	}
	return (*this);
}


#include <iostream>
void Location_t::print() const {
    std::cout << "  Location content:\n";
    std::cout << "    uri: " << _uri << "\n";
    std::cout << "    root: " << _root << "\n";
    std::cout << "    alias: " << _alias << "\n";
    std::cout << "    _autoindex: " << _autoindex << '\n';
    std::cout << "    _client_max_body_size: " << _client_max_body_size << '\n';

    std::cout << "    _allow_methods: ";
    if (is_allowed_get())
        std::cout << "GET ";
    if (is_allowed_post())
        std::cout << "POST ";
    if (is_allowed_delete())
        std::cout << "DELETE ";
    std::cout << "\n";

    std::cout << "    _cgi_path: " << get_cgi_path() << '\n';
    std::cout << "    _cgi_type: " << get_cgi_type() << '\n';
    std::cout << "    _upload_to: " << get_upload_to() << '\n';
    std::cout << "    _redir: " << get_redir() << '\n';

    std::cout << "    index:\n";
    for (std::list<std::string>::const_iterator it = _index.begin();
            it != _index.end(); ++it)
        std::cout << "     -" << *it << '\n';

    std::cout << "    error_pages:\n";
    for (std::map<int, std::string>::const_iterator it = _error_pages.begin(); it != _error_pages.end(); ++it)
        std::cout << "     -" << it->first << " / " << it->second << '\n';


    for (std::vector<Location_t>::const_iterator it = _locations.begin();
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
