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

void Server_t::add_location(const Location_t *loc)
{
    _locations.push_back(*loc);
}

// Getters ----------------------------------
const std::set<std::string>   &Server_t::get_names() const{
    return _names;
}
const std::set<Server_t::listen_pair_t> &Server_t::get_listen() const {
    return _listen;
}
std::vector<Location_t> &Server_t::get_locations() {
    return _locations;
}

std::pair<bool, Location_t>  Server_t::get_best_location_block(std::string path) const {
	std::string path_tried;
	std::string uri;
	size_t pos;

	path_tried = path;
	if (_locations.begin() == _locations.end())
		return (std::make_pair<bool, Location_t>(false, Location_t()));
	while (1) {
		for(std::vector<Location_t>::const_iterator it = _locations.begin(); it != _locations.end(); it++) {
			uri = it->get_uri();
			if (uri == path_tried || uri == path_tried + "/") {
				return (std::make_pair<bool, Location_t>(true, it->get_best_location_block(path)));
			}
		}
		if (path_tried.empty())
			break;
		pos = path_tried.find_last_of('/');
		if (pos == std::string::npos)
			pos = 0;
		path_tried.erase(pos);
	}
	return (std::make_pair<bool, Location_t>(false, Location_t()));
}

unsigned long Server_t::get_best_client_max_body_size(std::string path) const {
	std::pair<bool, Location_t> best_location;

	best_location = get_best_location_block(path);
	if (best_location.first == false)
		return (this->get_client_max_body_size());
	else
		return (best_location.second.get_client_max_body_size());
}

std::pair<std::string, std::string> Server_t::get_best_cgi(std::string path) const {
	std::pair<bool, Location_t> best_location;

	best_location = get_best_location_block(path);
	if (best_location.first == false)
		return (std::make_pair<std::string, std::string>(get_cgi_path(), get_cgi_type()));
	else
		return (std::make_pair<std::string, std::string>(best_location.second.get_cgi_path(), best_location.second.get_cgi_type()));
}

// checks --------------------
bool Server_t::has_name(const std::string &name) const
{
    return _names.count(name);
}

bool Server_t::has_listen(const listen_pair_t &listen_pair) const
{
    //TODO Add "*" support
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
void Server_t::print() const{
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
    for (std::list<std::string>::const_iterator it = _index.begin();
            it != _index.end(); ++it)
        std::cout << "   -" << *it << '\n';

    std::cout << "  error_pages:\n";
    for (std::map<int, std::string>::const_iterator it = _error_pages.begin(); it != _error_pages.end(); ++it)
        std::cout << "     -" << it->first << " / " << it->second << '\n';

    std::cout << "  _autoindex: " << _autoindex << '\n';
    std::cout << "  _client_max_body_size: " << _client_max_body_size << '\n';

    std::cout << "  _allow_methods: ";
    if (is_allowed_get())
        std::cout << "GET ";
    if (is_allowed_post())
        std::cout << "POST ";
    if (is_allowed_delete())
        std::cout << "DELETE ";
    std::cout << "\n";

    std::cout << "  _cgi_path: " << get_cgi_path() << '\n';
    std::cout << "  _cgi_type: " << get_cgi_type() << '\n';


    if  (_locations.size())
        std::cout << "  locations: \n";
    for (std::vector<Location_t>::const_iterator it = _locations.begin();
            it != _locations.end(); ++it){
        std::cout << "---- Location " << it->get_uri() << " ----\n";
        it->print();
        std::cout << "---- End of " << it->get_uri() << " ----\n";
    }


}
