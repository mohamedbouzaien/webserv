#include "../headers/Request.hpp"
#include <cstring>

Request::Request() : _method(BAD_REQUEST) {}

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

int	Request::getWordEnd(const char *s) const {
	int i = 0;
	while (s[i] && s[i] != ' ' && s[i] != '	' && s[i] != '\r')
		i++;
	return (i);
}

void Request::setRequest(char *header) {

	int i = 0;
	int pos = getWordEnd(&header[i]);
	std::string keyword(&header[i], pos);

	if (keyword == "GET")
		_method = GET;
	else if (keyword == "POST")
		_method = POST;
	else if (keyword == "DELETE")
		_method = DELETE;
	else
			_method = BAD_REQUEST;
	i += pos;
	while (header[i] == ' ')
		i++;
	pos = getWordEnd(&header[i]);
	_path = std::string(&header[i], pos);
	i += pos;
	while (header[i] == ' ')
		i++;
	pos = getWordEnd(&header[i]);
	if (pos == 0)
		_method = BAD_REQUEST;
	_protocol = std::string(&header[i], pos);
	i += pos;
	while (header[i] == ' ')
		i++;
	if (((header[i] != '\r' && header[i] != '\n') || (header[i] == '\r' && header[i + 1] != 10)) || !_path.size() || !_protocol.size())
		_method = BAD_REQUEST;
}

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
