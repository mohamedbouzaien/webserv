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
	if (*buffer == ' ')
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
	if (*buffer == ' ')
		buffer++;
	pos = getWordEnd(buffer);
	if (pos == 0)
		_method = BAD_REQUEST;
	_protocol = std::string(buffer, pos);
	buffer += pos;
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
		buffer++;
		pos = getWordEnd(buffer);
		host_port = std::string(buffer, pos);
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
		_method = BAD_REQUEST;
		return (1);
	}
	pos++;
	if (keyword == "Host")
		setHostField(buffer + pos);
	else
		setHeaderField(keyword, buffer + pos);
	return (1);
}

void Request::parseRequest(char *buffer) {
	this->setRequestLine(buffer);
	buffer = strchr(buffer, '\n');
	_uri_length = _path.size();
	if (!*buffer)
		return;
	buffer++;
	while (*buffer && *buffer != '\n' && *buffer != '\r')
	{
		setRequestField(buffer);
		buffer = strchr(buffer, '\n');
		if (buffer == NULL)
		{
			_method = BAD_REQUEST;
			break;
		}
		buffer++;
	}
	_uri_length += _host.first.size();
	if (*buffer == '\r')
		buffer++;
	if (*buffer == '\n')
		buffer++;
	if (_header_fields.find("Transfer-Encoding") != _header_fields.end() && _header_fields["Transfer-Encoding"] == "chunked")
	{
		std::string r_body(buffer);
		int chunk_size;
		while (r_body.find("\r\n")  != std::string::npos) {
			chunk_size = strtol(r_body.c_str(), &buffer, 16);
			if (chunk_size == 0)
				return ;
			r_body.erase(0, r_body.find("\r\n") + 2);
			_body += std::string(r_body, 0, chunk_size);
			r_body.erase(0, chunk_size);
			if (r_body.find("\r\n") != 0)
				return ;
			r_body.erase(0, 2);
		}
	}
	else if (_header_fields.find("Content-Length") != _header_fields.end())
		_body = std::string(buffer, stoi(_header_fields["Content-Length"]));

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

void Request::setUriLength(int len) {
	_uri_length = len;
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

int Request::getUriLength() const {
	return (_uri_length);
}

// << OVERLOAD

std::ostream& operator<<(std::ostream& os, const Request& request) {
	std::cout << "<----- HEADER ----->" << std::endl;
	std::cout << "Method: " << request._method << ", Path: " << request._path << ", Protocol: " << request._protocol << std::endl;
	std::cout << "Query_string : ";
	if (request._query_string.size())
		std::cout << request._query_string << std::endl;
	else
		std::cout << "Non Specifield" << std::endl;
	std::cout << "Host: " << request._host.first << ", Port: " ;
	if (request._host.second.size())
		std::cout << request._host.second << std::endl;
	else
		std::cout << "Non Specified" << std::endl;
	std::cout << "URI Length: " << request._uri_length << std::endl;
	std::cout << "Header fields:" << std::endl;
	std::map<std::string, std::string>::const_iterator it = request._header_fields.begin();
	std::map<std::string, std::string>::const_iterator ite = request._header_fields.end();
	for (; it != ite; it++)
		std::cout << it->first << " : " << it->second << std::endl;
	std::cout << "<------ BODY ----->" << std::endl;
	std::cout << "Body : " << request._body << std::endl;
	std::cout << "<------ END ----->" << std::endl;
	return (os);
}
