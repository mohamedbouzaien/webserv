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

	int i = 0;
	int pos = getWordEnd(&buffer[i]);
	std::string keyword(&buffer[i], pos);

	if (keyword == "GET")
		_header.setMethod(GET);
	else if (keyword == "POST")
		_header.setMethod(POST);
	else if (keyword == "DELETE")
		_header.setMethod(DELETE);
	else
		_header.setMethod(BAD_REQUEST);
	i += pos;
	while (buffer[i] == ' ')
		i++;
	pos = getWordEnd(&buffer[i]);
	_header.setPath(std::string(&buffer[i], pos));
	i += pos;
	while (buffer[i] == ' ')
		i++;
	pos = getWordEnd(&buffer[i]);
	if (pos == 0)
		_header.setMethod(BAD_REQUEST);
	_header.setProtocol(std::string(&buffer[i], pos));
	i += pos;
	while (buffer[i] == ' ')
		i++;
	if (((buffer[i] != '\r' && buffer[i] != '\n') || (buffer[i] == '\r' && buffer[i + 1] != 10)) || ! _header.getPath().size() || ! _header.getProtocol().size())
		_header.setMethod(BAD_REQUEST);
	while (buffer[i] && buffer[i] != '\n')
		i++;
	return (i);
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

std::list<std::pair<std::string, std::string> > Request::setAcceptParams(char *buffer) {
	std::list<std::pair<std::string, std::string> > accept_params;
	int pos = 0;
	std::string keyword;

	while ((((*buffer != '\r' && *buffer != '\n') || (*buffer == '\r' && *(buffer + 1) != 10))) && *buffer != ',')
	{
		//get keyword
		//skip spaces
		while (*buffer == ' ')
			buffer++;
		if (*buffer == ',' || *buffer == '=')
			return (accept_params);
		//get keyword len
		while (buffer[pos] && buffer[pos] != ' ' && buffer[pos] != '=') {
			if (buffer[pos] == '(')
				while (buffer[pos] && buffer[pos] != ')')
					pos++;
			else
				pos++;
		}
		keyword = std::string(buffer, pos);
		buffer += pos;
		//get value
		//skip "  =   "
		while (*buffer == ' ')
			buffer++;
		if (*buffer == '=')
		
		while (*buffer == ' ')
			buffer++;
		pos = 0;
		// get value lenght
		while (buffer[pos] && buffer[pos] != '\r' && buffer[pos] != '\n' && buffer[pos] != ';' && buffer[pos] != ',')
		{
			if (buffer[pos] == '(')
				while (buffer[pos] && buffer[pos] != ')')
					pos++;
			else
				pos++;
		}
		//create pair key - value
		accept_params.push_back(std::make_pair<std::string, std::string>(keyword, std::string(buffer, pos)));
		buffer += pos ;
		if (*buffer == ';')
			buffer++;
		pos = 0;
	}
	return (accept_params);
}

std::list<std::pair<std::string, std::list<std::pair<std::string, std::string> > > > Request::setListField(char *buffer) {

	std::list<std::pair<std::string, std::list<std::pair<std::string, std::string> > > > list;
	std::list<std::pair<std::string, std::string> > accept_params;

	int pos = 0;
	int is_q = -1;
	std::string quality;

	while ((((*buffer != '\r' && *buffer != '\n') || (*buffer == '\r' && *(buffer + 1) != 10))))
	{
		while (*buffer == ' ')
			buffer++;
		while (buffer[pos] && buffer[pos] != '\r' && buffer[pos] != '\n' && buffer[pos] != ',' )
		{
			if (buffer[pos] == ';' && is_q == -1) {
				is_q = pos;
				accept_params = setAcceptParams(buffer + pos + 1);
			}
			if (buffer[pos] == '(')
				while (buffer[pos] && buffer[pos] != ')')
					pos++;
			else
				pos++;
		}
		if (is_q == -1)
			is_q = pos;
		list.push_back(std::make_pair<std::string, std::list<std::pair<std::string, std::string> > >(std::string(buffer, is_q), accept_params));
		buffer += pos ;
		if (*buffer == ',')
			buffer++;
		quality = std::string();
		is_q = -1;
		pos = 0;
	}
	return (list);
}

int Request::setRequestField(char *buffer) {
	if (*buffer == ' ' || *buffer == '	')
	{
		_header.setMethod(BAD_REQUEST);
		return (1);
	}
	char *check_key = strchr(buffer, ':');
	if (!check_key)
		return (1);
	int pos = check_key - buffer;
	std::string keyword(buffer, pos);
	pos++;
	for (int i = 0; keyword[i]; i++)
		keyword[i] = std::tolower(keyword[i]);
	if (keyword == "host")
		setHostField(buffer + pos);
	else
		_header.insertHeaderField(keyword, setListField(buffer + pos));
	return (1);
}

void Request::parseRequest(char *buffer) {
	buffer += this->setRequestLine(buffer);
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
	printHeader();
}

//Printer

void Request::printHeader() {
	_header.show();
}

//Setters

void Request::setHeader(Header header) {
	_header = header;
}

//Getters

Header Request::getHeader() const {
	return (_header);
}
