#include "../headers/Cgi.hpp"

const char* Cgi::MallocFailedException::what() const throw() {
	return ("Malloc failed");
}

Cgi::Cgi() : _conf(), _cgi_path(), _translated_path(), _output(), _upload_to(), _body_size(), _status_code() {
	_body = NULL;
	_cgi_env = NULL;
}

Cgi::Cgi(Server_t &conf, std::string t_path, Request &request) : _translated_path(t_path), _status_code() {
	_conf = conf;
	_cgi_path = _conf.get_best_cgi(request.getPath()).first;
	_body = NULL;
	_cgi_env = NULL;
	setUploadTo(_conf.get_best_upload_to(request.getPath()));
	setBody(request.getBody());
	_body_size = request.getBody().size();
	setCgiEnv(request);
}

Cgi::Cgi(const Cgi &other) {
	*this = other;
}

Cgi::~Cgi() {
	free_cgi_env();
	if (_body)
		free(_body);
	_body = NULL;
}

Cgi &Cgi::operator=(const Cgi &other) {
	if (this != &other) {
		_conf = other._conf;
		_cgi_path = other._cgi_path;
		_translated_path = other._translated_path;
		_output = other._output;
		_upload_to = other._upload_to;
		if (_body) {
			free(_body);
			_body = NULL;
		}
		_body_size = other._body_size;
		if (!(_body = static_cast<char *>(malloc(sizeof(char) * (_body_size + 1))))) {
			_status_code = INTERNAL_SERVER_ERROR;
			throw Cgi::MallocFailedException();
		}
		for (size_t i = 0; i < _body_size; i++)
			_body[i] = other._body[i];
		free_cgi_env();
		if (other._cgi_env) {
			int size = 0;
			for (;other._cgi_env[size]; size++)
				;
			if (!(_cgi_env = static_cast<char **>(malloc(sizeof(char *) * (size + 1))))) {
				_status_code = INTERNAL_SERVER_ERROR;
				throw Cgi::MallocFailedException();
			}
			for (int i = 0; i < size; i++)
				_cgi_env[i] = strdup(other._cgi_env[i]);
			_cgi_env[size] = NULL;
		}
		_body[_body_size] = 0;
		_status_code = other._status_code;
	}
	return (*this);
}

void Cgi::free_cgi_env() {
	if (!_cgi_env)
		return;
	for (int i = 0; _cgi_env[i] != NULL; i++)
		free(_cgi_env[i]);
	free(_cgi_env);
	_cgi_env = NULL;
}

void Cgi::runCgi() {
	int pid;
	char *argv[3];

	argv[0] = const_cast<char *>(_cgi_path.c_str());
	argv[1] = const_cast<char *>(_translated_path.c_str());
	argv[2] = NULL;
	if (pipe((int *)_body_pipe) < 0 || pipe((int *)_output_pipe)) {
		std::cout << "Pipe error" << std::endl;
		_status_code = INTERNAL_SERVER_ERROR;
		return ;
	}
	if ((pid = fork()) < 0) {
		std::cout << "Fork error" << std::endl;
		_status_code = INTERNAL_SERVER_ERROR;
		return ;
	}
	if (pid == 0) {
		close(_body_pipe[SIDE_OUT]);
		dup2(_body_pipe[SIDE_IN], STDIN_FILENO);
		close(_body_pipe[SIDE_IN]);
		dup2(_output_pipe[SIDE_OUT], STDOUT_FILENO);
		close(_output_pipe[SIDE_OUT]);
		execve(*argv, argv, _cgi_env);
		exit(1);
	}
	else {
		close(_body_pipe[SIDE_IN]);
		write(_body_pipe[SIDE_OUT], _body, _body_size);
		close(_body_pipe[SIDE_OUT]);
		close(_output_pipe[SIDE_OUT]);
		waitpid(pid, &_status_code, 0);
		if (WIFEXITED(_status_code) && WEXITSTATUS(_status_code) != 0)
			_status_code = INTERNAL_SERVER_ERROR;
		processOutput();
		close(_output_pipe[SIDE_IN]);
	}
}

void Cgi::parseHeader(std::string &header) {
	std::string field;
	std::string key;
	std::string value;
	size_t endline;
	size_t colon;

	while (header.size()) {
		endline = header.find("\r\n");
		field = header.substr(0, endline);
		if ((colon = field.find(": ")) != std::string::npos) {
			key = field.substr(0, colon);
			value = field.substr(colon + 2);
			if (key == "Status")
				setStatusCode(stoi(value));
			else
				_response_header[key] =  value;
		}
		if (endline == std::string::npos)
			break;
		header.erase(0, endline + 2);
	}
}

void Cgi::readHeader() {
	char buffer[CGI_BUFFER_SIZE + 1];
	int readed;
	size_t pos;
	std::string header;

	memset(buffer, 0, CGI_BUFFER_SIZE + 1);
	while ((pos = header.find("\r\n\r\n")) == std::string::npos && (readed = read(_output_pipe[SIDE_IN], buffer, CGI_BUFFER_SIZE)) > 0){
		header += buffer;
		memset(buffer, 0, CGI_BUFFER_SIZE + 1);
	}
	if (pos != std::string::npos) {
		_output = header.substr(pos + 4);
		header.erase(pos);
	}
	parseHeader(header);
}

void Cgi::processOutput() {
	if (_status_code == 0)
		_status_code = 200;
	readHeader();
	readBody();
}

void Cgi::readBody() {
	char buffer[CGI_BUFFER_SIZE + 1];
	int to_read;
	int size;

	memset(buffer, 0, CGI_BUFFER_SIZE + 1);
	std::map<std::string, std::string>::iterator content_length = _response_header.find("Content-Length");
	if (content_length == _response_header.end())
		size = -1;
	else
		size = stoi(content_length->second);
	if (size != -1 && (int)_output.size() >= size) {
		_output.erase(size);
		return;
	}
	if (size != -1 && size - _output.size() <= CGI_BUFFER_SIZE)
		to_read = size - _output.size();
	else
		to_read = CGI_BUFFER_SIZE;
	while (read(_output_pipe[SIDE_IN], buffer, to_read) > 0) {
		_output += buffer;
		memset(buffer, 0, CGI_BUFFER_SIZE + 1);
		if (size != -1 && size - _output.size() <= CGI_BUFFER_SIZE)
			to_read = size - _output.size();
		else
			to_read = CGI_BUFFER_SIZE;
	}
	_response_header.erase("Content-Length");
}

void Cgi::setCgiEnv(Request &request) {
	std::map<std::string, std::string> header_fields;
	std::map<std::string, std::string> mapped_cgi_env;
	std::map<std::string, std::string>::iterator it = header_fields.begin();
	std::map<std::string, std::string>::iterator ite = header_fields.end();

	header_fields = request.getHeaderFields();
	mapped_cgi_env["SERVER_SOFTWARE"] = "webserv/1.0";
	mapped_cgi_env["SERVER_NAME"] = "localhost";
	mapped_cgi_env["GATEWAY_INTERFACE"] = "CGI/1.1";

	mapped_cgi_env["SERVER_PROTOCOL"] = request.getProtocol();
	mapped_cgi_env["SERVER_PORT"] = request.getHost().second;

	mapped_cgi_env["REQUEST_METHOD"] = request.getMethod();
	mapped_cgi_env["PATH_INFO"] = request.getPath();
	mapped_cgi_env["PATH_TRANSLATED"] = _translated_path;
	mapped_cgi_env["SCRIPT_NAME"] = request.getPath();
	mapped_cgi_env["REQUEST_URI"] = request.getPath();
	if (mapped_cgi_env["REQUEST_METHOD"] == "GET")
		mapped_cgi_env["QUERY_STRING"] = request.getQueryString();

	mapped_cgi_env["REMOTE_HOST"] = "";
	mapped_cgi_env["REMOTE_ADDR"] = inet_ntoa(request.getClient().sin_addr);
	it = header_fields.find("Authorization");
	if (it != header_fields.end() && it->second.size()) {
		std::string auth_str = std::string(it->second, it->second.find_first_not_of(" "));
		mapped_cgi_env["AUTH_TYPE"] = std::string(auth_str, 0, auth_str.find(" "));
	}

	mapped_cgi_env["REMOTE_USER"] = "";
	mapped_cgi_env["REMOTE_IDENT"] = "";

	if (mapped_cgi_env["REQUEST_METHOD"] == "POST") {
		mapped_cgi_env["CONTENT_TYPE"] = header_fields["Content-Type"];
		mapped_cgi_env["CONTENT_LENGTH"] = std::to_string(_body_size);
	}

	mapped_cgi_env["REDIRECT_STATUS"] = "200";
	mapped_cgi_env["UPLOADS_DIR"] = _upload_to;
	it = header_fields.begin();
	ite = header_fields.end();
	for(;it != ite; it++)
		mapped_cgi_env["HTTP_" + upper_key(it->first)] = it->second;
	int size = mapped_cgi_env.size();
	free_cgi_env();
	if (!(_cgi_env = static_cast<char **>(malloc(sizeof(char *) * (size + 1))))) {
			_status_code = INTERNAL_SERVER_ERROR;
			throw Cgi::MallocFailedException();
		}
	it = mapped_cgi_env.begin();
	ite = mapped_cgi_env.end();
	for(int i = 0; i < size; i++) {
		_cgi_env[i] = strdup(std::string(it->first + "=" + it->second).c_str());
		it++;
	}
	_cgi_env[size] = NULL;
}

std::string Cgi::upper_key(std::string key) const {
	int size = key.size();
	for (int i = 0; i < size; i++) {
		if (key[i] == '-')
			key[i] = '_';
		else
			key[i] = std::toupper(key[i]);
	}
	return (key);
}

//Setter

void Cgi::setConf(Server_t conf) {
	_conf = conf;
}

void Cgi::setCgiPath(std::string path) {
	_cgi_path = path;
}

void Cgi::setTranslatedPath(std::string path) {
	_translated_path = path;
}

void Cgi::setOutput(std::string output) {
	_output = output;
}

void Cgi::setUploadTo(std::string location) {
	char buffer[CGI_BUFFER_SIZE + 1];

	memset(buffer, 0, CGI_BUFFER_SIZE + 1);
	if (location.empty() || location[0] == '/')
		_upload_to = location;
	else {
		if (getcwd(buffer, CGI_BUFFER_SIZE) == NULL)
			return;
		_upload_to = std::string(buffer) + "/" + location;
	}
	if (!_upload_to.empty() && _upload_to[_upload_to.size() - 1] != '/')
		_upload_to += "/";
}

void Cgi::setBody(const std::vector<char> vbody) {
	if (_body)
		free(_body);
	_body_size = vbody.size();
	if (!(_body = static_cast<char *>(malloc(sizeof(char) * (_body_size + 1))))) {
		_status_code = INTERNAL_SERVER_ERROR;
		throw Cgi::MallocFailedException();
	}
	int i = 0;
	for (std::vector<char>::const_iterator it = vbody.begin(); it !=  vbody.end(); it++)
		_body[i++] = *it;
	_body[i] = 0;
}

void Cgi::setBodySize(size_t size) {
	if (size > _body_size)
		return ;
	_body[size] = 0;
	_body_size = size;
}

void Cgi::setStatusCode(int code) {
	_status_code = code;
	if (!(_status_code >= 100 && _status_code <= 103) &&
			!(_status_code >= 200 && _status_code <= 208) &&
			_status_code != 210 && _status_code != 226 &&
			!(_status_code >= 300 && _status_code <= 308) &&
			_status_code != 310 && 
			!(_status_code >= 400 && _status_code <= 418) && 
			!(_status_code >= 421 && _status_code <= 429) &&
			_status_code != 431 &&
			!(_status_code >= 449 && _status_code <= 451) &&
			_status_code != 456 && 
			!(_status_code >= 500 && _status_code <= 511))
		_status_code = INTERNAL_SERVER_ERROR;
	std::cout << "Status: " << _status_code << std::endl;
}

void Cgi::setResponseHeader(std::map<std::string, std::string> header) {
	_response_header = header;
}

//Getter

Server_t Cgi::getConf() const {
	return (_conf);
}

std::string Cgi::getCgiPath() const {
	return (_cgi_path);
}

std::string Cgi::getTranslatedPath() const {
	return (_translated_path);
}

std::string Cgi::getOutput() const {
	return (_output);
}

std::string Cgi::getUploadTo() const {
	return (_upload_to);
}

char *Cgi::getBody() const {
	return (_body);
}

int Cgi::getBodySize() const {
	return (_body_size);
}

int Cgi::getStatusCode() const {
	return (_status_code);
}

std::map<std::string, std::string> Cgi::getResponseHeader() const {
	return (_response_header);
}

char **Cgi::getCgiEnv() const {
	return (_cgi_env);
}

// << OVERLOAD

std::ostream& operator<<(std::ostream& os, const Cgi& cgi) {
	os << "--- Cgi ---" << std::endl;
	os << "_cgi_path : " << cgi._cgi_path << std::endl;
	os << "_translated_path : " << cgi._translated_path << std::endl;
	os << "_upload_to : " << cgi._upload_to << std::endl;
	os << "--- RESPONSE ELEMENTS ---" << std::endl;
	os << "_response_header : " << std::endl;
	for(std::map<std::string, std::string>::const_iterator it = cgi._response_header.begin(); it != cgi._response_header.end(); it++)
		std::cout << it->first << " : " << it->second << std::endl;
	os << "_output : " << cgi._output << std::endl;
	os << "--- END OF RESPONSE ELEMENTS ---" << std::endl;
	os << "_body_size : " << cgi._body_size << std::endl;
	os << "_body : " << std::endl;
	for (size_t i = 0; i < cgi._body_size; i++)
		os << cgi._body[i];
	os << std::endl;
	os << "_status_code  : " << cgi._status_code << std::endl;
	os << "_cgi_env : " << std::endl;
	if (cgi._cgi_env) {
		for (int i = 0; cgi._cgi_env[i]; i++)
			os << cgi._cgi_env[i] << std::endl;
	}
	else
		os << "No _cgi_env" << std::endl;
	os << "--- End ---" << std::endl;
	return (os);
}
