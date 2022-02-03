#include "../headers/Cgi.hpp"

const char* Cgi::MallocFailedException::what() const throw() {
	return ("Malloc failed");
}


Cgi::Cgi(char *path, Request &request) : _cgi_path(path), _status_code() {
	int size = CGI_ENV_SIZE + request.getHeaderFields().size();
		if (!(_cgi_env = static_cast<char **>(malloc(sizeof(char *) * (size + 1))))) {
			_status_code = INTERNAL_SERVER_ERROR;
			throw Cgi::MallocFailedException();
		}
	for(int i = 0; i < size; i++)
		_cgi_env[i] = NULL;
	setCgiEnv(request);
	_body_size = request.getBody().size();
	memset((char *)_output, 0, CGI_BUFFER_SIZE);
}

Cgi::Cgi(const Cgi &other) {
	*this = other;
}

Cgi::~Cgi() {
	if (!_cgi_env)
		return ;
	for (int i = 0; _cgi_env[i] != NULL; i++)
		free(_cgi_env[i]);
	free(_cgi_env);
}

Cgi &Cgi::operator=(const Cgi &other) {
	if (this != &other)
		_cgi_path = other._cgi_path;
	return (*this);
}

void Cgi::runCgi(Request &request) {
	int pid;
	char *argv[3];
	argv[0] = _cgi_path;
	argv[1] = (char *)request.getPath().c_str();
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
		write(_body_pipe[SIDE_OUT], request.getBody().c_str(), _body_size);
		close(_body_pipe[SIDE_OUT]);
		close(_output_pipe[SIDE_OUT]);
		waitpid(pid, &_status_code, 0);

		if (WIFEXITED(_status_code) && WEXITSTATUS(_status_code) != 0)
			_status_code = INTERNAL_SERVER_ERROR;
		read(_output_pipe[SIDE_IN], (char *)_output, CGI_BUFFER_SIZE - 1);
		close(_output_pipe[SIDE_IN]);
		setStatusCode(_output);
	}
}

void Cgi::setCgiEnvVar(const char *var, int pos) {
	if (_cgi_env[pos])
		free(_cgi_env[pos]);
	if (!(_cgi_env[pos] = static_cast<char *>(malloc(sizeof(char *) * (strlen(var) + 1))))) {
		_status_code = INTERNAL_SERVER_ERROR;
		throw Request::MallocFailedException();
	}
	strcpy(_cgi_env[pos], var);
}

void Cgi::setCgiEnv(Request &request) {
	std::map<std::string, std::string> header_fields;
	header_fields = request.getHeaderFields();
	//SERVER VAR NEED TO BE COMPLETED W/ CONF_FILE VALUE

	setCgiEnvVar(std::string("SERVER_SOFTWARE=webserv/1.0").c_str(), 0);

	setCgiEnvVar(std::string("SERVER_NAME=localhost").c_str(), 1);

	setCgiEnvVar(std::string("GATEWAY_INTERFACE=CGI/1.1").c_str(), 2);
	// REQUEST VAR
	setCgiEnvVar((std::string("SERVER_PROTOCOL=") + request.getProtocol()).c_str(), 3);
	setCgiEnvVar((std::string("SERVER_PORT=") + request.getHost().second).c_str(), 4);
	setCgiEnvVar((std::string("REQUEST_METHOD=") + request.getMethod()).c_str(), 5);
	setCgiEnvVar((std::string("PATH_INFO=")).c_str(), 6);
	setCgiEnvVar((std::string("PATH_TRANSLATED=") + "tests/www").c_str(), 7);
	setCgiEnvVar((std::string("SCRIPT_NAME=") + request.getPath()).c_str(), 8);
	setCgiEnvVar((std::string("QUERY_STRING=") + request.getQueryString()).c_str(), 9);
	setCgiEnvVar((std::string("REMOTE_HOST=") + "127.0.0.1").c_str(), 10);
	setCgiEnvVar((std::string("REMOTE_ADDR=") + header_fields["X-HTTP-FORWARDED-FOR"]).c_str(), 11);
	setCgiEnvVar((std::string("AUTH_TYPE=")).c_str(), 12);
	setCgiEnvVar((std::string("REMOTE_USER=")).c_str(), 13);
	setCgiEnvVar((std::string("REMOTE_IDENT=")).c_str(), 14);
	setCgiEnvVar((std::string("CONTENT_TYPE=") + header_fields["Content-Type"]).c_str(), 15);
	setCgiEnvVar((std::string("CONTENT_LENGTH=") + std::to_string(request.getBody().size())).c_str(), 16);

	//FROM CLIENT VAR
	setCgiEnvVar((std::string("REDIRECT_STATUS=200").c_str()), 17);
	setCgiEnvVar((std::string("SCRIPT_FILENAME=") + std::string("tests/www") + request.getPath()).c_str(), 18);
	//FROM CLIENT VAR
	int i = 19;
	int size = CGI_ENV_SIZE + request.getHeaderFields().size();
	std::map<std::string, std::string> hf = request.getHeaderFields();
	std::map<std::string, std::string>::iterator it = hf.begin();
	std::map<std::string, std::string>::iterator ite = hf.end();
	for (;(i < size && it != ite); i++) {
		setCgiEnvVar(std::string("HTTP_" + upper_key(it->first) + "=" + it->second).c_str(), i);
		it++;
	}
	_cgi_env[i] = NULL;
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

void Cgi::setCgiPath(char *path) {
	_cgi_path = path;
}

void Cgi::setStatusCode(std::string buffer) {
	if (buffer.find("Status:") == 0)
		_status_code = stoi(buffer.erase(0, 8));
	else 
		_status_code = OK;
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

//Getter

char* Cgi::getOutput() const {
	return ((char *)_output);
} 

char* Cgi::getCgiPath() const {
	return (_cgi_path);
}

int Cgi::getStatusCode() const {
	return (_status_code);
}
