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
		_params = other._params;
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

std::list<std::pair<std::string, std::string> > Request::setAcceptParams(char *header) {
	std::list<std::pair<std::string, std::string> > accept_params;
	int pos = 0;
	std::string keyword;

	while ((((*header != '\r' && *header != '\n') || (*header == '\r' && *(header + 1) != 10))) && *header != ',')
	{
		//get keyword
		//skip spaces
		while (*header == ' ')
			header++;
		if (*header == ',' || *header == '=')
			return (accept_params);
		//get keyword len
		while (header[pos] && header[pos] != ' ' && header[pos] != '=') {
			if (header[pos] == '(')
				while (header[pos] && header[pos] != ')')
					pos++;
			else
				pos++;
		}
		keyword = std::string(header, pos);
		header += pos;
		//get value
		//skip "  =   "
		while (*header == ' ')
			header++;
		if (*header == '=')
			header++;
		while (*header == ' ')
			header++;
		pos = 0;
		// get value lenght
		while (header[pos] && header[pos] != '\r' && header[pos] != '\n' && header[pos] != ';' && header[pos] != ',')
		{
			if (header[pos] == '(')
				while (header[pos] && header[pos] != ')')
					pos++;
			else
				pos++;
		}
		//create pair key - value
		accept_params.push_back(std::make_pair<std::string, std::string>(keyword, std::string(header, pos)));
		header += pos ;
		if (*header == ';')
			header++;
		pos = 0;
	}

	return (accept_params);
}

std::list<std::pair<std::string, std::list<std::pair<std::string, std::string> > > > Request::setListField(char *header) {

	std::list<std::pair<std::string, std::list<std::pair<std::string, std::string> > > > list;
	std::list<std::pair<std::string, std::string> > accept_params;

	int pos = 0;
	int is_q = -1;
	std::string quality;

	while ((((*header != '\r' && *header != '\n') || (*header == '\r' && *(header + 1) != 10))))
	{
		while (*header == ' ')
			header++;
		while (header[pos] && header[pos] != '\r' && header[pos] != '\n' && header[pos] != ',' )
		{
			if (header[pos] == ';' && is_q == -1) {
				is_q = pos;
				accept_params = setAcceptParams(header + pos + 1);
			}
			if (header[pos] == '(')
				while (header[pos] && header[pos] != ')')
					pos++;
			else
				pos++;
		}
		if (is_q == -1)
			is_q = pos;
		list.push_back(std::make_pair<std::string, std::list<std::pair<std::string, std::string> > >(std::string(header, is_q), accept_params));
		header += pos ;
		if (*header == ',')
			header++;
		quality = std::string();
		is_q = -1;
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
	else
		_params[keyword] = setListField(header + pos);
	return (1);
}

void Request::parseRequest(char *header) {
	header += this->setRequestLine(header);
	if (_method == BAD_REQUEST || !*header)
		return;
	header++;
	while (*header && *header != '\n')
	{
		setRequestField(header);
		header = strchr(header, '\n');
		if (header == NULL)
		{
			_method = BAD_REQUEST;
			break;
		}
		header++;
	}

	std::map<std::string, std::list<std::pair<std::string, std::list<std::pair<std::string, std::string> > > > >::iterator it = _params.begin();
	std::map<std::string, std::list<std::pair<std::string, std::list<std::pair<std::string, std::string> > > > >::iterator ite = _params.end();

	std::cout << "PARAMS:" << std::endl;
	for (;it != ite;it++) {
		std::cout << it->first << ": " << std::endl;
		std::list<std::pair<std::string, std::list<std::pair<std::string, std::string> > > > l = it->second;
		for (std::list<std::pair<std::string, std::list<std::pair<std::string, std::string> > > >::iterator lit = l.begin(); lit != l.end(); lit++)
		{
			std::cout << "     " << lit->first << ": " << std::endl;
			std::list<std::pair<std::string, std::string> > accept_params = lit->second;
			for(std::list<std::pair<std::string, std::string> >::iterator nit = accept_params.begin(); nit != accept_params.end(); nit++)
				std::cout << "           " << nit->first << " = " << nit->second << std::endl;
		}
	}
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
