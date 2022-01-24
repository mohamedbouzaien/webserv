#include "../headers/Request.hpp"
#include <cstring>

const char* Request::MallocFailedException::what() const throw() {
	return ("Malloc failed");
}

Request::Request() {
	if (!(_cgi_env = static_cast<char **>(malloc(sizeof(char *) * (CGI_ENV_SIZE + 1)))))
		throw Request::MallocFailedException();
	for(int i = 0; i < CGI_ENV_SIZE; i++)
		_cgi_env[i] = NULL;
}

Request::Request(const Request &other) {
	*this = other;
}

Request::~Request() {
	for (int i = 0; _cgi_env[i] != NULL; i++)
		free(_cgi_env[i]);
	free(_cgi_env);
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
	std::cout << "<------ CGI_ENV ------>" << std::endl;
	for (int i = 0; _cgi_env[i] != NULL; i++)
		std::cout << _cgi_env[i] << std::endl;
	std::cout << "<------ END ------>" << std::endl;
}

void Request::setCgiEnvVar(const char *var, int pos) {
	if (_cgi_env[pos])
		free(_cgi_env[pos]);
	if (!(_cgi_env[pos] = static_cast<char *>(malloc(sizeof(char *) * (strlen(var) + 1)))))
		throw Request::MallocFailedException();
	strcpy(_cgi_env[pos], var);

}

void Request::convertToCgiEnv() {
	//SERVER VAR NEED TO BE COMPLETED W/ CONF_FILE VALUE
	
	setCgiEnvVar(std::string("SERVER_SOFTWARE=webserv/1.0").c_str(), 0);

	setCgiEnvVar(std::string("SERVER_NAME=localhost").c_str(), 1);

	setCgiEnvVar(std::string("GATEWAY_INTERFACE=CGI/1.1").c_str(), 2);
	// REQUEST VAR
	setCgiEnvVar((std::string("SERVER_PROTOCOL=") + _protocol).c_str(), 3);
	setCgiEnvVar((std::string("SERVER_PORT=") + _host.second).c_str(), 4);
	setCgiEnvVar((std::string("REQUEST_METHOD=") + _method).c_str(), 5);
	setCgiEnvVar((std::string("PATH_INFO=")).c_str(), 6);
	setCgiEnvVar((std::string("PATH_TRANSLATED=") + "/Users/adriencastelbou/Desktop/42/webserv").c_str(), 7);
	setCgiEnvVar((std::string("SCRIPT_NAME=") + _path).c_str(), 8);
	setCgiEnvVar((std::string("QUERY_STRING=") + _query_string).c_str(), 9);
	setCgiEnvVar((std::string("REMOTE_HOST=") + _header_fields["X-FORWARDED-HOST"]).c_str(), 10);
	setCgiEnvVar((std::string("REMOTE_ADDR=") + _header_fields["X-HTTP-FORWARDED-FOR"]).c_str(), 11);
	setCgiEnvVar((std::string("AUTH_TYPE=")).c_str(), 12);
	setCgiEnvVar((std::string("REMOTE_USER=")).c_str(), 13);
	setCgiEnvVar((std::string("REMOTE_IDENT=")).c_str(), 14);
	setCgiEnvVar((std::string("CONTENT_TYPE=") + _header_fields["CONTENT_TYPE"]).c_str(), 15);
	setCgiEnvVar((std::string("CONTENT_LENGHT=") + _header_fields["CONTENT_LENGTH"]).c_str(), 16);

	//FROM CLIENT VAR
	setCgiEnvVar((std::string("HTTP_ACCEPT=") + _header_fields["ACCEPT"]).c_str(), 17);
	setCgiEnvVar((std::string("HTTP_ACCEPT_LANGUAGE=") + _header_fields["ACCEPT-LANGUAGE"]).c_str(), 18);
	setCgiEnvVar((std::string("HTTP_USER_AGENTT=") + _header_fields["USER-AGENT"]).c_str(), 19);
	setCgiEnvVar((std::string("HTTP_COOKIE=")).c_str(), 20);
	setCgiEnvVar((std::string("HTTP_REFERER=")).c_str(), 21);
	_cgi_env[22] = NULL;
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
