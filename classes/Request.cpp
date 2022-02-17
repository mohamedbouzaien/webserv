#include "../headers/Request.hpp"
#include <cstring>

const char* Request::MallocFailedException::what() const throw() {
	return ("Malloc failed");
}

Request::Request() : _is_body(0) {}

Request::Request(int socket) : _client_socket(socket), _is_body(0) {}

Request::Request(const Request &other) {
	*this = other;
}

Request::~Request() {
	_body.clear();
}

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
	if (!buffer || !*buffer)
		return;
	buffer++;
	while (buffer && *buffer && *buffer != '\n' && *buffer != '\r')
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
}

int		Request::isHeaderEnded(std::string &request, char *buffer) {
	char *body = strstr(buffer, "\r\n\r\n");
	std::string request_end;

	if (_is_body == 1)
		return (0);

	if (request.size() > 3)
		request_end = request.substr(request.size() - 3);
	else
		request_end = request;
	if (body != NULL) {
		return (body - buffer + 4);
	}
	else if (request_end == "\r\n\r" && *buffer == '\n')
		return (1);
	else if (request_end.find("\r\n") != std::string::npos && (request_end.find("\r\n") + 2 == request_end.size()) && strstr(buffer, "\r\n") == buffer) 
		return (2);
	else if (request_end.find("\r") != std::string::npos && (request_end.find("\r") + 1 == request_end.size()) && strstr(buffer, "\n\r\n") == buffer)
		return (3);
	return (0);
}

int		Request::recvSocket(std::string &request) {
	char buffer[BUFFER_SIZE + 1];
	int	bytesRead;
	int to_skip = 0;

	memset(buffer, 0, BUFFER_SIZE + 1);
	bytesRead = recv(_client_socket, buffer, BUFFER_SIZE, 0);
	if ((to_skip = isHeaderEnded(request, buffer)) > 0)
		_is_body = 1;
	if (_is_body)
		_body.insert(_body.end(), buffer + to_skip, buffer + bytesRead);
	request += buffer;
	memset(buffer, 0, BUFFER_SIZE + 1);
	return (bytesRead);
}

int		Request::readSocket(std::string &request, std::string pattern) {
	int status;

	while (request.find(pattern) == std::string::npos) {
		status = recvSocket(request);
		if (status <= 0)
			return (-1);
		if (status != BUFFER_SIZE)
			break;
	}
	return (1);
}

int Request::unchunkBody(std::string &chunked_body) {
	int chunk_size;

	while (chunked_body.find("\r\n") != std::string::npos) {
		chunk_size = strtol(chunked_body.c_str(), NULL, 16);
		if (chunk_size == 0)
			return (1);
		chunked_body.erase(0, chunked_body.find("\r\n") + 2);
		_body.insert(_body.end(), chunked_body.begin(), chunked_body.begin() + chunk_size);
		chunked_body.erase(0, chunk_size);
		if (chunked_body.find("\r\n") != 0)
			return (1);
		chunked_body.erase(0, 2);
	}
	return (1);
}

int Request::readChunkedBody() {
	std::string chunked_body(_body.begin(), _body.end());
	int status;
	_body.clear();
	if (chunked_body.find("0\r\n\r\n") != 0) {
		while (chunked_body.find("\r\n0\r\n\r\n") == std::string::npos) {
			status = recvSocket(chunked_body);
			if (status <= 0)
				return (-1);
			if (status != BUFFER_SIZE)
				break;
		}
	}
	unchunkBody(chunked_body);
	return (1);

}

int Request::readBody(size_t len) {
	char buffer[BUFFER_SIZE + 1];
	int to_read;
	int	bytesRead;

	if (_body.size() >= len) {
		_body.erase(_body.begin() + len, _body.end());
		return (1);
	}
	while (_body.size() < len) {
		memset(buffer, 0, BUFFER_SIZE + 1);
		if (_body.size() + BUFFER_SIZE < len)
			to_read = BUFFER_SIZE;
		else
			to_read = len - _body.size();
		bytesRead = recv(_client_socket, buffer, to_read, 0);
		_body.insert(_body.end(), buffer, buffer + bytesRead);
		if (bytesRead < 1)
			return (bytesRead);
	}
	return (bytesRead);
}

int Request::handle() {
	std::string header;
	int status;

	status = readSocket(header, "\r\n\r\n");
	if (status < 1)
		return (status);
	parseRequest((char *)header.c_str());
	if (_header_fields.find("Transfer-Encoding") != _header_fields.end() && _header_fields["Transfer-Encoding"] == "chunked")
		status = readChunkedBody();
	else if (_header_fields.find("Content-Length") != _header_fields.end())
		status = readBody(stoi(_header_fields["Content-Length"]));
	if (status < 1)
		return (status);
	return (1);
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

void Request::setUriLength(int len) {
	_uri_length = len;
}

void Request::setBody(std::vector<char> vbody) {
	_body = vbody;
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

std::vector<char> Request::getBody() const {
	return (_body);
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
	for(std::vector<char>::const_iterator it = request._body.begin(); it != request._body.end(); it++)
		std::cout << *it << std::endl;

	std::cout << std::endl << "<------ END ----->" << std::endl;
	return (os);
}
