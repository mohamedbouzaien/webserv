#include "../headers/Request.hpp"
#
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

std::string Request::getLine(const std::string& s) const {
	std::string::size_type pos = s.find('\n');
	if (pos != std::string::npos)
        return ((s.substr(0, pos)));
    else
        return (std::string(NULL));
}

int	Request::getWordEnd(const std::string& s) const {
	int i = 0;
	while (s[i] && s[i] != ' ' && s[i] != '	' && s[i] != '\r')
		i++;
	return (i);
}

void Request::setRequest(std::string& header) {
	std::string s = getLine(header);
	std::string keyword;
	std::string::size_type pos = getWordEnd(s);
	keyword = s.substr(0, pos);
	if (keyword == "GET")
		_method = GET;
	else if (keyword == "POST")
		_method = POST;
	else if (keyword == "DELETE")
		_method = DELETE;
	else
		{
			_method = BAD_REQUEST;
			return ;
		}
	pos += 1;
	while (s[pos] == ' ')
		pos++;
	s = s.erase(0, pos);
	pos = getWordEnd(s);
	keyword = s.substr(0, pos);
	_path = keyword;
	pos += 1;
	while (s[pos] == ' ')
		pos++;
	s = s.erase(0, pos);
	pos = getWordEnd(s);
	if (!pos)
		_method = BAD_REQUEST;
	keyword = s.substr(0, pos);
	_protocol = keyword;
	pos += 1;
	while (s[pos] == ' ')
		pos++;
	if (s[pos] && (s[pos] != '\r' || (s[pos] == '\r' && s[pos + 1] != 10)))
		_method = BAD_REQUEST;
	std::cout << "method: " << _method << "|" << std::endl;
	std::cout << "path: " << _path << "|" << std::endl;
	std::cout << "protocol: " << _protocol << "|" << std::endl;
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
