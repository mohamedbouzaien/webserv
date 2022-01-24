#include "../headers/Cgi.hpp"

Cgi::Cgi(Request *request) : _request(request) {}

Cgi::Cgi(const Cgi &other) {
	*this = other;
}

Cgi::~Cgi() {}

Cgi &Cgi::operator=(const Cgi &other) {
	if (this != &other)
		_request = other._request;
	return (*this);
}

//Setter

void Cgi::setRequest(Request *request) {
	_request = request;
}

//Getter

Request *Cgi::getRequest() const {
	return (_request);
}
