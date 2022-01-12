#include "Request.hpp"
#
Request::Request() {}

Request::Request(const Request &other) {
	*this = other;
}

Request::~Request() {}

Request &Request::operator=(const Request &other) {
	if (this != &other)
	{
		_method = other._method;
		_path = other._path;
		_protocol = other._protocol;
		_host = other._host;
		_accept = other._accept;
		_user_agent = other._user_agent;
	}
	return (*this);
}

void Request::setRequest(std::string header) {}

//Setters

void Request::setMethod(int method) {
	_method = method;
}

void Request::setPath(std::string path) {
	_path = path;
}

void Request::setProtocol(std::string protocol) {
	_protocol = protocol;
}

void Request::setHost(std::string host) {
	_host = host;
}

void Request::setAccept(std::string accept) {
	_accept = accept;
}

void Request::setUserAgent(std::string user_agent) {
	_user_agent = user_agent;
}

//Getters

int Request::getMethod() const {
	return (_method);
}

std::string Request::getPath() const {
	return (_path);
}

std::string Request::getProtocol() const {
	return (_protocol);
}

std::string Request::getHost() const {
	return (_host);
}

std::string Request::getAccpet() const {
	return (_accept);
}

std::string Request::getUserAgent() const {
	return (_user_agent);
}
