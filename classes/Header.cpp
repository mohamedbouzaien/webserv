#include "../headers/Header.hpp"
#include <cstring>

Header::Header() : _method(BAD_REQUEST) {}

Header::Header(const Header &other) {
	*this = other;
}

Header::~Header() {}

Header &Header::operator=(const Header &other) {
	if (this != &other)
	{
		_method = other._method;
		_path = other._path;
		_protocol = other._protocol;
		_host = other._host;
		_header_fields = other._header_fields;
	}
	return (*this);
}

//Printer

void Header::show() {
	std::cout << "<----- HEADER ----->" << std::endl;
	std::cout << "Method: " << _method << ", Path: " << _path << ", Protocol: " << _protocol << std::endl;
	std::cout << "Host: " << _host.first << ", Port: " ;
	if (_host.second.size())
		std::cout << _host.second << std::endl;
	else
		std::cout << "Non Specified" << std::endl;
	std::cout << "Header fields:" << std::endl;
		std::map<std::string, std::list<std::pair<std::string, std::list<std::pair<std::string, std::string> > > > >::iterator it = _header_fields.begin();
	std::map<std::string, std::list<std::pair<std::string, std::list<std::pair<std::string, std::string> > > > >::iterator ite = _header_fields.end();
	for (;it != ite;it++) {
		std::cout << it->first << ": " << std::endl;
		std::list<std::pair<std::string, std::list<std::pair<std::string, std::string> > > > l = it->second;
		for (std::list<std::pair<std::string, std::list<std::pair<std::string, std::string> > > >::iterator lit = l.begin(); lit != l.end(); lit++)
		{
			std::cout << "     " << lit->first << ": " << std::endl;
			std::list<std::pair<std::string, std::string> > accept_params = lit->second;
			for(std::list<std::pair<std::string, std::string> >::iterator nit = accept_params.begin(); nit != accept_params.end(); nit++)
				std::cout << "           " << nit->first << " = " << nit->second << std::endl;
		}
	}
}

//Setters

void Header::setMethod(int method) {
	_method = method;
}

void Header::setPath(std::string path) {
	_path = path;
}

void Header::setProtocol(std::string protocol) {
	_protocol = protocol;
}

void Header::setHost(std::pair<std::string, std::string> host) {
	_host = host;
}

void Header::setHeaderFields(std::map<std::string, std::list<std::pair<std::string, std::list<std::pair<std::string, std::string> > > > > header_fields) {
	_header_fields = header_fields;
}

void Header::insertHeaderField(std::string key, std::list<std::pair<std::string, std::list<std::pair<std::string, std::string> > > > value) {
	_header_fields[key] = value;
}

//Getters

int Header::getMethod() const {
	return (_method);
}

std::string Header::getPath() const {
	return (_path);
}

std::string Header::getProtocol() const {
	return (_protocol);
}

std::pair<std::string, std::string> Header::getHost() const {
	return (_host);
}
