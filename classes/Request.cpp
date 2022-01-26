#include "../headers/Request.hpp"
#include <cstring>

const char* Request::MallocFailedException::what() const throw() {
	return ("Malloc failed");
}

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
		_query_string = other._query_string;
		_protocol = other._protocol;
		_host = other._host;
		_header_fields = other._header_fields;
		_body = other._body;
	}
	return (*this);
}

void Request::clear() {
	_method.clear();
	_path.clear();
	_query_string.clear();
	_protocol.clear();
	_host.first.clear();
	_host.second.clear();
	_header_fields.clear();
}

int	Request::getWordEnd(const char *s) const {
	int i = 0;
	while (s[i] && s[i] != ' ' && s[i] != '	' && s[i] != '\r' && s[i] != '\n')
		i++;
	return (i);
}

int Request::setRequestLine(char *buffer) {
	int pos = getWordEnd(buffer);
	std::string keyword(buffer, pos);

	if (keyword == "GET")
		_method = GET;
	else if (keyword == "POST")
		_method = POST;
	else if (keyword == "DELETE")
		_method = DELETE;
	else
		_method = BAD_REQUEST;
	buffer += pos;
	while (*buffer == ' ')
		buffer++;
	if (*buffer != '/')
		_method = BAD_REQUEST;

	pos = 0;
	while (buffer[pos] && buffer[pos] != '?' && buffer[pos] != ' ' && buffer[pos] != '	' && buffer[pos] != '\r' && buffer[pos] != '\n')
		pos++;
	_path = std::string(buffer, pos);
	buffer += pos;
	if (*buffer == '?') {
		buffer++;
		pos = getWordEnd(buffer);
		_query_string = std::string(buffer, pos);
		buffer += pos;
	}
	while (*buffer == ' ')
		buffer++;
	pos = getWordEnd(buffer);
	if (pos == 0)
		_method = BAD_REQUEST;
	_protocol = std::string(buffer, pos);
	buffer += pos;
	while (*buffer == ' ')
		buffer++;
	if (((*buffer != '\r' && *buffer != '\n') || (*buffer == '\r' && *(buffer + 1)  != '\n')) || ! _path.size() || ! _protocol.size())
		_method = BAD_REQUEST;
	while (*buffer && *buffer != '\n')
		buffer++;
	return (1);
}

int Request::setHostField(char *buffer) {
	int pos = 0;
	bool is_port = false;
	std::string host_name;
	std::string host_port;

	if (_host.first.size())
	{
		_method = BAD_REQUEST;
		return (0);
	}
	while (*buffer == ' ')
		buffer++;
	while (buffer[pos] && buffer[pos] != ' ' && buffer[pos] != '	' && buffer[pos] != '\r' && buffer[pos] != '\n' && buffer[pos] != ':')
		pos++;
	if (buffer[pos] == ':')
		is_port = true;
	host_name = std::string(buffer, pos);
	buffer += pos;
	if (is_port)
	{
		pos = getWordEnd(buffer);
		host_name = std::string(buffer, pos);
		buffer += pos;
	}
	_host = std::make_pair(host_name, host_port);
	while (*buffer == ' ')
		buffer++;
	if (((*buffer != '\r' && *buffer != '\n') || (*buffer == '\r' && *(buffer + 1) != 10)) )
		_method = BAD_REQUEST;
	return (1);
}

void Request::setHeaderField(std::string keyword, char *buffer) {
	while (*buffer == ' ')
		buffer++;
	int pos = 0;
	while (buffer[pos] && buffer[pos] != '\r' && buffer[pos] != '\n')
		pos++;
	if (buffer[pos] == '\r' && buffer[pos + 1] != '\n') {
		_method = BAD_REQUEST;
		return ;
	}
	pos--;
	while (buffer[pos] == ' ')
		pos--;
	_header_fields[keyword] = std::string(buffer, pos + 1);
}

int Request::setRequestField(char *buffer) {
	int pos = 0;
	while (buffer[pos] && buffer[pos] != ' ' && buffer[pos] != '	' && buffer[pos] != '\r' && buffer[pos] != '\n' && buffer[pos] != ':')
		pos++;
	std::string keyword(buffer, pos);
	if (buffer[pos] != ':')
	{
		while (buffer[pos] == ' ')
			pos++;
		if ((buffer[pos] != '\r' && buffer[pos] != '\n') || (buffer[pos] == '\r' && buffer[pos + 1] != '\n'))
			_method = BAD_REQUEST;
		return (1);
	}
	pos++;
	for (int i = 0; keyword[i]; i++)
		keyword[i] = std::toupper(keyword[i]);
	if (keyword == "HOST")
		setHostField(buffer + pos);
	else
		setHeaderField(keyword, buffer + pos);
	return (1);
}

void Request::parseRequest(char *buffer) {
	this->setRequestLine(buffer);
	buffer = strchr(buffer, '\n');
	if (_method == BAD_REQUEST || !*buffer)
		return;
	buffer++;
	while (*buffer && *buffer != '\n')
	{
		setRequestField(buffer);
		buffer = strchr(buffer, '\n');
		if (buffer == NULL || _method == BAD_REQUEST)
		{
			_method = BAD_REQUEST;
			break;
		}
		buffer++;
	}
	if (*buffer == '\n')
	{
		buffer++;
		_body = std::string(buffer, strlen(buffer));
	}
}

//Printer

void Request::printRequest() {
	std::cout << "<----- HEADER ----->" << std::endl;
	std::cout << "Method: " << _method << ", Path: " << _path << ", Protocol: " << _protocol << std::endl;
	std::cout << "Query_string : ";
	if (_query_string.size())
		std::cout << _query_string << std::endl;
	else
		std::cout << "Non Specifield" << std::endl;
	std::cout << "Host: " << _host.first << ", Port: " ;
	if (_host.second.size())
		std::cout << _host.second << std::endl;
	else
		std::cout << "Non Specified" << std::endl;
	std::cout << "Header fields:" << std::endl;
	std::map<std::string, std::string>::iterator it = _header_fields.begin();
	std::map<std::string, std::string>::iterator ite = _header_fields.end();
	for (; it != ite; it++)
		std::cout << it->first << " : " << it->second << std::endl;
	std::cout << "<------ BODY ----->" << std::endl;
	std::cout << "Body : " << _body << std::endl;
}

//Setters

void Request::setMethod(std::string method) {
	_method = method;
}

void Request::setPath(std::string path) {
	_path = path;
}

void Request::setProtocol(std::string protocol) {
	_protocol = protocol;
}

void Request::setHost(std::pair<std::string, std::string> host) {
	_host = host;
}

void Request::setHeaderFields(std::map<std::string, std::string > header_fields) {
	_header_fields = header_fields;
}

void Request::setBody(std::string body) {
	_body = body;
}

//Getters

std::string Request::getMethod() const {
	return (_method);
}

std::string Request::getPath() const {
	return (_path);
}

std::string Request::getProtocol() const {
	return (_protocol);
}

std::pair<std::string, std::string> Request::getHost() const {
	return (_host);
}

std::map<std::string, std::string> Request::getHeaderFields() const {
	return (_header_fields);
}

std::string Request::getQueryString() const {
	return (_query_string);
}

std::string Request::getBody() const {
	return (_body);
}
