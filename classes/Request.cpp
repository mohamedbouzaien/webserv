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

int Request::setRequestLine(char *header) {

	int i = 0;
	int pos = getWordEnd(&header[i]);
	std::string keyword(&header[i], pos);

	if (keyword == "GET")
		_header.setMethod(GET);
	else if (keyword == "POST")
		_header.setMethod(POST);
	else if (keyword == "DELETE")
		_header.setMethod(DELETE);
	else
		_header.setMethod(BAD_REQUEST);
	i += pos;
	while (header[i] == ' ')
		i++;
	pos = getWordEnd(&header[i]);
	_header.setPath(std::string(&header[i], pos));
	i += pos;
	while (header[i] == ' ')
		i++;
	pos = getWordEnd(&header[i]);
	if (pos == 0)
		_header.setMethod(BAD_REQUEST);
	_header.setProtocol(std::string(&header[i], pos));
	i += pos;
	while (header[i] == ' ')
		i++;
	if (((header[i] != '\r' && header[i] != '\n') || (header[i] == '\r' && header[i + 1] != 10)) || ! _header.getPath().size() || ! _header.getProtocol().size())
		_header.setMethod(BAD_REQUEST);
	while (header[i] && header[i] != '\n')
		i++;
	return (i);
}

int Request::setHostField(char *header) {
	int pos = 0;
	bool is_port = false;
	std::string host_name;
	std::string host_port;

	if (_header.getHost().first.size())
	{
		_header.setMethod(BAD_REQUEST);
		return (BAD_REQUEST);
	}
	while (*header == ' ')
		header++;
	while (header[pos] && header[pos] != ' ' && header[pos] != '	' && header[pos] != '\r' && header[pos] != '\n' && header[pos] != ':')
		pos++;
	if (header[pos] == ':')
		is_port = true;
	host_name = std::string(header, pos);
	//_host.first = std::string(header, pos);
	header += pos;
	if (is_port)
	{
		pos = getWordEnd(header);
		host_name = std::string(header, pos);
		//_host.second = std::string(header, pos);
		header += pos;
	}
	_header.setHost(std::make_pair(host_name, host_port));
	while (*header == ' ')
		header++;
	if (((*header != '\r' && *header != '\n') || (*header == '\r' && *(header + 1) != 10)) )
		_header.setMethod(BAD_REQUEST);
	return (_header.getMethod());
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
	if (*header == ' ' || *header == '	')
	{
		_header.setMethod(BAD_REQUEST);
		return (1);
	}
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
		_header.insertHeaderField(keyword, setListField(header + pos));
		//_params[keyword] = setListField(header + pos);
	return (1);
}

void Request::parseRequest(char *header) {
	header += this->setRequestLine(header);
	if (_header.getMethod() == BAD_REQUEST || !*header)
		return;
	header++;
	while (*header && *header != '\n')
	{
		setRequestField(header);
		header = strchr(header, '\n');
		if (header == NULL || _header.getMethod() == BAD_REQUEST)
		{
			_header.setMethod(BAD_REQUEST);
			break;
		}
		header++;
	}

/*
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
	}*/
}

//Setters

void Request::setHeader(Header header) {
	_header = header;
}

//Getters

Header Request::getHeader() const {
	return (_header);
}
