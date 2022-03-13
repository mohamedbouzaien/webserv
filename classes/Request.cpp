#include "../headers/Request.hpp"

const char* Request::MallocFailedException::what() const throw() {
	return ("Malloc failed");
}

Request::Request() :  _is_body(0) , _status_code(200){}

Request::Request(int socket, sockaddr_in client) : _client_socket(socket), _client(client), _is_body(0), _status_code(200) {}

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


int Request::setRequestLine(std::string line) {
	size_t pos;

	pos = line.find(" ");
	std::string word = line.substr(0, pos);
	if (word == "GET" || word == "POST" || word == "DELETE" || word == "PUT")
		_method = word;
	else
		_status_code = 400;
	line.erase(0, pos + (pos != std::string::npos ? 1 : 0));
	if (line.size() == 0)
		return (0);
	if (line[0] != '/')
		_status_code = 400;
	pos = line.find(" ");
	word = line.substr(0, pos);
	_path = word.substr(0, word.find("?"));
	if (word.find("?") != std::string::npos)
		_query_string = word.substr(word.find("?") + 1);
	line.erase(0, pos + (pos != std::string::npos ? 1 : 0));
	if (line.size() == 0)
		return (0);
	_protocol = line;
	if (_protocol != "HTTP/1.1" && _protocol != "HTTP/1.0")
		_status_code = 505;
	return (1);
}

int Request::setHostField(std::string host) {
	size_t pos;

	if (_host.first.size())
	{
		_status_code = 400;
		return (0);
	}
	pos = host.find_first_not_of(' ');
	host.erase(0, pos);
	pos = host.find(':');
	_host.first = host.substr(0, pos);
	if (pos != std::string::npos)
		_host.second = host.substr(pos + 1);
	else
		_host.second = "80";
	if (_host.first.find('	') != std::string::npos || _host.second.find_first_not_of("0123456789") != std::string::npos)
		_status_code = 400;
	return (1);
}

void Request::setHeaderField(std::string keyword, std::string value) {
	size_t pos;

	pos = value.find_first_not_of(' ');
	value.erase(0, pos);
	value.erase(value.find_last_not_of(' ') + 1);
	_header_fields[keyword] = value;
}

int Request::setRequestField(std::string buffer) {
	size_t pos;
	std::string keyword;

	pos = buffer.find(":");
	if (pos == std::string::npos) {
		_status_code = 400;
		return (1);
	}
	keyword = buffer.substr(0, pos);
	if (keyword.find('	') != std::string::npos || keyword.find(' ') != std::string::npos)
	{
		_status_code = 400;
		return (1);
	}
	if (keyword == "Host")
		setHostField(buffer.substr(pos + 1));
	else
		setHeaderField(keyword, buffer.substr(pos + 1));
	return (1);
}

void Request::parseHeader(std::string header) {
	size_t pos;

	pos = header.find("\r\n");
	this->setRequestLine(header.substr(0, pos));
	if (pos == std::string::npos) {
		_status_code = 400;
		return ;
	}
	header.erase(0, pos + 2);
	while (header.size())
	{
		pos = header.find("\r\n");
		if (pos == 0)
			break;
		this->setRequestField(header.substr(0, pos));
		if (pos == std::string::npos) {
			_status_code = 400;
			break ;
		}
		header.erase(0, pos + 2);
	}
	if (_path.size() + _host.first.size() > MAX_URI_SIZE)
		_status_code = 414;
	std::map<std::string, std::string>::iterator  content_length = _header_fields.find("Content-Length");
	if (content_length != _header_fields.end() && std::all_of(content_length->second.begin(), content_length->second.end(), ::isdigit) == false)
		_status_code = 400;
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

int		Request::recvHeader(std::string &header) {
	char buffer[BUFFER_SIZE + 1];
	int	bytesRead;
	int to_skip = 0;

	memset(buffer, 0, BUFFER_SIZE + 1);
	bytesRead = recv(_client_socket, buffer, BUFFER_SIZE, 0);
	if (bytesRead == -1)
		return (bytesRead);
	if ((to_skip = isHeaderEnded(header, buffer)) > 0)
		_is_body = 1;
	else
		to_skip = bytesRead;
	if (_is_body)
		_body.insert(_body.end(), buffer + to_skip, buffer + bytesRead);
	header += std::string(buffer, to_skip);
	memset(buffer, 0, BUFFER_SIZE + 1);
	return (bytesRead);
}

int		Request::readHeader(std::string &header) {
	int status;

	while (header.find("\r\n\r\n") == std::string::npos) {
		status = recvHeader(header);
		if (status != BUFFER_SIZE || header.size() > MAX_HEADER_SIZE) {
			if (header.size() > MAX_HEADER_SIZE)
				_status_code = 431;
			break;
		}
	}
	return (status);
}


int Request::readAndParseHeader() {
	std::string header;
	int status;

	status = readHeader(header);
	if (status == -1)
		return (status);
	parseHeader(header);
	return (status);
}

int Request::searchEndline(std::vector<char> &vector) const {
	std::vector<char>::iterator it = vector.begin();
	std::vector<char>::iterator ite = vector.end();

	for(int i = 0; it != ite; i++) {
		if (*it == '\r') {
			if (*(++it) == '\n')
				return (i);
		}
		else
			it++;
	}
	return (-1);
}

long int Request::getChunkSize(std::vector<char> &vector) const {
	int endline = searchEndline(vector);
	if (endline < 0)
		return (-2);
	std::string size(vector.begin(), vector.begin() + endline);
	if (size.find_first_not_of("0123456789ABCDEFabcdef") != std::string::npos || size.empty())
		return (-1);
	return (strtol(size.c_str(), NULL, 16));
}

long int Request::unchunkBody(std::vector<char> &body_buffer) {
	long int chunk_size;
	std::vector<char> chunk;
	std::vector<char> body_copy;

	body_copy = body_buffer;
	while (1) {
		chunk_size = getChunkSize(body_copy);
		if (chunk_size < 0)
			return (chunk_size);
		if (chunk_size > MAX_MALLOC_SIZE)
			return (-1);
		body_copy.erase(body_copy.begin(), body_copy.begin() + searchEndline(body_copy) + 2);
		if (body_copy.size() < (size_t)chunk_size)
			return (-2);
		chunk.insert(chunk.end(), body_copy.begin(), body_copy.begin() + chunk_size);
		body_copy.erase(body_copy.begin(), body_copy.begin() + chunk_size);
		if (searchEndline(body_copy) > 0)
			return (-1);
		else if (searchEndline(body_copy) == -1)
			return (-2);
		_body.insert(_body.end(), chunk.begin(), chunk.end());
		body_copy.erase(body_copy.begin(), body_copy.begin() + searchEndline(body_copy) + 2);
		body_buffer = body_copy;
		chunk.clear();
		if (chunk_size == 0)
			return (0);
	}
	return (chunk_size);
}

int Request::readChunkedBody(int status, size_t max_body_size) {
	char buffer[BUFFER_SIZE + 1];
	size_t client_body_size;
	long int chunk_size;
	std::vector<char> body_buffer = _body;
	bool is_limit = true;

	_body.clear();
	memset(buffer, 0, BUFFER_SIZE + 1);
	client_body_size = body_buffer.size();
	if (max_body_size == 0) {
		is_limit = false;
		max_body_size = client_body_size + 1;
	}
	while (client_body_size < max_body_size) {
		chunk_size = unchunkBody(body_buffer);
		if (chunk_size == 0 || chunk_size == -1 || status != BUFFER_SIZE)
			break;
		memset(buffer, 0, BUFFER_SIZE + 1);
		status = recv(_client_socket, buffer, BUFFER_SIZE, 0);
		if (status <= 0)
			return (status);
		body_buffer.insert(body_buffer.end(), buffer, buffer + status);
		client_body_size += status;
		if (is_limit == false)
			max_body_size = client_body_size + 1;
	}
	if (chunk_size != 0)
		_status_code = 400;
	if (client_body_size > max_body_size)
		_status_code = 413;
	return (1);

}

int Request::readBody(size_t len, size_t max_body_size) {
	char buffer[BUFFER_SIZE + 1];
	int to_read;
	int	bytesRead;

	if ( max_body_size > 0 && len > max_body_size) {
		_status_code = 413;
		return (1);
	}
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
		if (bytesRead < 1)
			return (bytesRead);
		_body.insert(_body.end(), buffer, buffer + bytesRead);
	}
	return (bytesRead);
}

int Request::readAndParseBody(int status, size_t max_body_size) {
	if (_header_fields.find("Transfer-Encoding") != _header_fields.end() && _header_fields["Transfer-Encoding"] == "chunked")
		status = readChunkedBody(status, max_body_size);
	else if (_header_fields.find("Content-Length") != _header_fields.end())
		status = readBody(stoi(_header_fields["Content-Length"]), max_body_size);
	return (status);
}

//Setters

void Request::setClientSocket(int socket) {
	_client_socket = socket;
}

void Request::setClient(sockaddr_in client) {
	_client = client;
}

void Request::setMethod(std::string method) {
	_method = method;
}

void Request::setPath(std::string path) {
	_path = path;
}

void Request::setQueryString(std::string s) {
	_query_string = s;
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

void Request::setBody(std::vector<char> vbody) {
	_body = vbody;
}

void Request::setIsBody(int i) {
	_is_body = i;
}

void Request::setStatusCode(int status_code) {
	_status_code = status_code;
}

//Getters

std::string Request::search(std::string s) const {
	if (_header_fields.find(s) != _header_fields.end())
		return (_header_fields.find(s)->second);
	else
		return ("");
}

int Request::getClientSocket() const {
	return (_client_socket);
}

sockaddr_in Request::getClient() const {
	return (_client);
}

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

int Request::getIsBody() const {
	return(_is_body);
}

int Request::getStatusCode() const {
	return (_status_code);
}

// << OVERLOAD

std::ostream& operator<<(std::ostream& os, const Request& request) {
	std::cout << "Client IP address is: " << inet_ntoa(request._client.sin_addr) << std::endl;
	std::cout << "Client port is: " << (int) ntohs(request._client.sin_port) << std::endl;

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
	std::cout << "Header fields:" << std::endl;
	std::map<std::string, std::string>::const_iterator it = request._header_fields.begin();
	std::map<std::string, std::string>::const_iterator ite = request._header_fields.end();
	for (; it != ite; it++)
		std::cout << it->first << " : " << it->second << std::endl;
	std::cout << "<------ BODY ----->" << std::endl;
	for(std::vector<char>::const_iterator it = request._body.begin(); it != request._body.end(); it++)
		std::cout << *it;
	std::cout << std::endl;

	std::cout << "<------ ---- ----->" << std::endl;
	std::cout << "Status code : " << request._status_code << std::endl;
	std::cout << "<------ END  ----->" << std::endl;
	return (os);
}
