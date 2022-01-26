#include "../headers/Cgi.hpp"

const char* Cgi::MallocFailedException::what() const throw() {
	return ("Malloc failed");
}


Cgi::Cgi(char *path, Request &request) : _cgi_path(path) {
		if (!(_cgi_env = static_cast<char **>(malloc(sizeof(char *) * (CGI_ENV_SIZE + 1)))))
		throw Cgi::MallocFailedException();
	for(int i = 0; i < CGI_ENV_SIZE; i++)
		_cgi_env[i] = NULL;
	setCgiEnv(request);
	_body_size = atoi(request.getHeaderFields()["CONTENT-LENGTH"].c_str());
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

void Cgi::runCgi(Request &request) const {
	int pid;

	if (pipe((int *)_body_pipe) < 0 || pipe((int *)_output_pipe)) {
		std::cout << "Pipe error" << std::endl;
		return ;
	}
	if ((pid = fork()) < 0) {
		std::cout << "Fork error" << std::endl;
		return ;
	}
	if (pid == 0) {
		close(_body_pipe[SIDE_OUT]);
		dup2(_body_pipe[SIDE_IN], STDIN_FILENO);
		close(_body_pipe[SIDE_IN]);
		dup2(_output_pipe[SIDE_OUT], STDOUT_FILENO);
		close(_output_pipe[SIDE_OUT]);
		execve(_cgi_path, NULL, _cgi_env);
		exit(1);
	}
	else {
		close(_body_pipe[SIDE_IN]);
		write(_body_pipe[SIDE_OUT], request.getBody().c_str(), _body_size);
		close(_body_pipe[SIDE_OUT]);
		close(_output_pipe[SIDE_OUT]);
		wait(NULL);
		read(_output_pipe[SIDE_IN], (char *)_output, CGI_BUFFER_SIZE - 1);
		close(_output_pipe[SIDE_IN]);
	}
	std::cout << "BUFFER:" << std::endl << _output << std::endl;
}

void Cgi::setCgiEnvVar(const char *var, int pos) {
	if (_cgi_env[pos])
		free(_cgi_env[pos]);
	if (!(_cgi_env[pos] = static_cast<char *>(malloc(sizeof(char *) * (strlen(var) + 1)))))
		throw Request::MallocFailedException();
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
	setCgiEnvVar((std::string("PATH_TRANSLATED=") + "www").c_str(), 7);
	setCgiEnvVar((std::string("SCRIPT_NAME=") + request.getPath()).c_str(), 8);
	setCgiEnvVar((std::string("QUERY_STRING=") + request.getQueryString()).c_str(), 9);
	setCgiEnvVar((std::string("REMOTE_HOST=") + "127.0.0.1").c_str(), 10);
	setCgiEnvVar((std::string("REMOTE_ADDR=") + header_fields["X-HTTP-FORWARDED-FOR"]).c_str(), 11);
	setCgiEnvVar((std::string("AUTH_TYPE=")).c_str(), 12);
	setCgiEnvVar((std::string("REMOTE_USER=")).c_str(), 13);
	setCgiEnvVar((std::string("REMOTE_IDENT=")).c_str(), 14);
	setCgiEnvVar((std::string("CONTENT_TYPE=") + header_fields["CONTENT-TYPE"]).c_str(), 15);
	setCgiEnvVar((std::string("CONTENT_LENGTH=") + header_fields["CONTENT-LENGTH"]).c_str(), 16);

	//FROM CLIENT VAR
	setCgiEnvVar((std::string("HTTP_ACCEPT=") + header_fields["ACCEPT"]).c_str(), 17);
	setCgiEnvVar((std::string("HTTP_ACCEPT_LANGUAGE=") + header_fields["ACCEPT-LANGUAGE"]).c_str(), 18);
	setCgiEnvVar((std::string("HTTP_USER_AGENTT=") + header_fields["USER-AGENT"]).c_str(), 19);
	setCgiEnvVar((std::string("HTTP_COOKIE=")).c_str(), 20);
	setCgiEnvVar((std::string("HTTP_REFERER=")).c_str(), 21);
	setCgiEnvVar((std::string("REDIRECT_STATUS=200").c_str()), 22);
	setCgiEnvVar((std::string("SCRIPT_FILENAME=") + std::string("www") + request.getPath()).c_str(), 23);
	_cgi_env[24] = NULL;
}

//Setter

void Cgi::setCgiPath(char *path) {
	_cgi_path = path;
}

//Getter

char* Cgi::getOutput() const {
	return ((char *)_output);
} 

char* Cgi::getCgiPath() const {
	return (_cgi_path);
}
