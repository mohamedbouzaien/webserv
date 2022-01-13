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
	while (s[i] && s[i] != ' ' && s[i] != '	' && s[i] != '\r' && s[i] != '\n')
		i++;
	return (i);
}

int Request::setRequestLine(char *header) {

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
	while (header[i] && header[i] != '\n')
		i++;
	return (i);
}

int Request::setHostField(char *header) {
	if (_host.size())
	{
		_method = BAD_REQUEST;
		return (_method);
	}
	while (*header == ' ')
		header++;
	int pos = getWordEnd(header);
	_host = std::string(header, pos);
	header += pos;
	while (*header == ' ')
		header++;
	if (((*header != '\r' && *header != '\n') || (*header == '\r' && *(header + 1) != 10)) )
		_method = BAD_REQUEST;
	return (_method);
}

std::list<std::string> Request::setListField(char *header) {
	std::list<std::string> list;
	int pos = 0;

	while ((((*header != '\r' && *header != '\n') || (*header == '\r' && *(header + 1) != 10))))
	{
		while (*header == ' ')
			header++;
		while (header[pos] && header[pos] != ' ' && header[pos] != '	' && header[pos] != '\r' && header[pos] != '\n' && header[pos] != ',' )
			pos++;
		list.push_back(std::string(header, pos));
		header += pos;
		if (*header == ',')
			header++;
		pos = 0;
	}
	return (list);
}

int Request::setRequestField(char *header) {
	char *check_key = strchr(header, ':');
	if (!check_key)
		return (1);
	int pos = check_key - header;
	std::string keyword(header, pos);
	pos++;
	for (int i = 0; keyword[i]; i++)
		keyword[i] = std::tolower(keyword[i]);
	if (keyword == "host")
		setHostField(header + pos);
	else if (keyword == "accept")
		_accept = setListField(header + pos);
std::list<std::string>::iterator it = _accept.begin();
std::list<std::string>::iterator ite = _accept.end();
for (;it != ite;it++)
	std::cout << *it << std::endl;
	return (1);
}

void Request::parseRequest(char *header) {
	header += this->setRequestLine(header);
	std::cout << "|" << header << "|" << std::endl;
	std::cout << "method: " << _method << std::endl;
	std::cout << "path: " << _path << std::endl;
	std::cout << "protocol: " << _protocol << std::endl;
	if (_method == BAD_REQUEST || !*header)
		return;
	header++;
	//while (*header && *header != '\n')
	//{
		header += setRequestField(header);
		header++;

	std::cout << "method: " << _method << std::endl;
	//}
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

void Request::setAccept(std::list<std::string> accept) {
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

std::list<std::string> Request::getAccpet() const {
	return (_accept);
}

std::string Request::getUserAgent() const {
	return (_user_agent);
}
