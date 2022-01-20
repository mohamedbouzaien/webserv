#include "../headers/Request.hpp"
#include <cstring>

Request::Request() : _header() {}

Request::Request(const Request &other) {
	*this = other;
}

Request::~Request() {}

Request &Request::operator=(const Request &other) {
	if (this != &other)
		_header = other._header;
	return (*this);
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
		_header.setMethod(GET);
	else if (keyword == "POST")
		_header.setMethod(POST);
	else if (keyword == "DELETE")
		_header.setMethod(DELETE);
	else
		_header.setMethod(BAD_REQUEST);
	buffer += pos;
	while (*buffer == ' ')
		buffer++;
	if (*buffer != '/')
		_header.setMethod(BAD_REQUEST);

	pos = 0;
	while (buffer[pos] && buffer[pos] != '?' && buffer[pos] != ' ' && buffer[pos] != '	' && buffer[pos] != '\r' && buffer[pos] != '\n')
		pos++;
	_header.setPath(std::string(buffer, pos));
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
		_header.setMethod(BAD_REQUEST);
	_header.setProtocol(std::string(buffer, pos));
	buffer += pos;
	while (*buffer == ' ')
		buffer++;
	if (((*buffer != '\r' && *buffer != '\n') || (*buffer == '\r' && *(buffer + 1)  != '\n')) || ! _header.getPath().size() || ! _header.getProtocol().size())
		_header.setMethod(BAD_REQUEST);
	while (*buffer && *buffer != '\n')
		buffer++;
	return (1);
}

int Request::setHostField(char *buffer) {
	int pos = 0;
	bool is_port = false;
	std::string host_name;
	std::string host_port;

	if (_header.getHost().first.size())
	{
		_header.setMethod(BAD_REQUEST);
		return (BAD_REQUEST);
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
	_header.setHost(std::make_pair(host_name, host_port));
	while (*buffer == ' ')
		buffer++;
	if (((*buffer != '\r' && *buffer != '\n') || (*buffer == '\r' && *(buffer + 1) != 10)) )
		_header.setMethod(BAD_REQUEST);
	return (_header.getMethod());
}

void Request::setHeaderField(std::string keyword, char *buffer) {
	while (*buffer == ' ')
		buffer++;
	int pos = 0;
	while (buffer[pos] && buffer[pos] != '\r' && buffer[pos] != '\n')
		pos++;
	if (buffer[pos] == '\r' && buffer[pos + 1] != '\n') {
		_header.setMethod(BAD_REQUEST);
		return ;
	}
	pos--;
	while (buffer[pos] == ' ')
		pos--;
	_hf[keyword] = std::string(buffer, pos + 1);
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
			_header.setMethod(BAD_REQUEST);
		return (1);
	}
	pos++;
	for (int i = 0; keyword[i]; i++)
		keyword[i] = std::tolower(keyword[i]);
	if (keyword == "host")
		setHostField(buffer + pos);
	else
		setHeaderField(keyword, buffer + pos);
	return (1);
}

void Request::parseRequest(char *buffer) {
	this->setRequestLine(buffer);
	buffer = strchr(buffer, '\n');
	if (_header.getMethod() == BAD_REQUEST || !*buffer)
		return;
	buffer++;
	while (*buffer && *buffer != '\n')
	{
		setRequestField(buffer);
		buffer = strchr(buffer, '\n');
		if (buffer == NULL || _header.getMethod() == BAD_REQUEST)
		{
			_header.setMethod(BAD_REQUEST);
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
	std::cout << "Query_string : " << _query_string << std::endl;
	std::map<std::string, std::string>::iterator it = _hf.begin();
	std::map<std::string, std::string>::iterator ite = _hf.end();
	for (; it != ite; it++)
		std::cout << it->first << " : " << it->second << std::endl;
	std::cout << "Body : " << _body << std::endl;
}

//Setters

void Request::setHeader(Header header) {
	_header = header;
}

//Getters

Header Request::getHeader() const {
	return (_header);
}
