#include "../headers/Cgi.hpp"

const char* Cgi::MallocFailedException::what() const throw() {
	return ("Malloc failed");
}


Cgi::Cgi(char *path, Request &request) : _cgi_path(path), _status_code() {
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
	if (this != &other) {

		_cgi_path = other._cgi_path;
		strcpy(_output, other._output);
		_body_size = other._body_size;
		_status_code = other._status_code;
	}
	return (*this);
}

void Cgi::runCgi(Request &request) {
	int pid;
	char *argv[3];
	argv[0] = const_cast<char *>(_cgi_path.c_str());
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
	mapped_cgi_env["PATH_INFO"] = "tests/www" + request.getPath();
	mapped_cgi_env["PATH_TRANSLATED"] = "tests/www" + request.getPath();
	mapped_cgi_env["SCRIPT_NAME"] = request.getPath();
	if (mapped_cgi_env["REQUEST_METHOD"] == "GET")
		mapped_cgi_env["QUERY_STRING"] = request.getQueryString();

	mapped_cgi_env["REMOTE_HOST"] = "127.0.0.1";
	mapped_cgi_env["REMOTE_ADDR"] = "127.0.0.1";
	it = header_fields.find("Authorization");
	if (it != header_fields.end() && it->second.size()) {
		std::string auth_str = std::string(it->second, it->second.find_first_not_of(" "));
		mapped_cgi_env["AUTH_TYPE"] = std::string(auth_str, 0, auth_str.find(" "));
	}

	mapped_cgi_env["REMOTE_USER"] = "";
	mapped_cgi_env["REMOTE_IDENT"] = "";

	if (mapped_cgi_env["REQUEST_METHOD"] == "POST") {
		mapped_cgi_env["CONTENT_TYPE"] = header_fields["Content-Type"];
		mapped_cgi_env["CONTENT_LENGTH"] = std::to_string(request.getBody().size());
	}

	mapped_cgi_env["REDIRECT_STATUS"] = "200";

	it = header_fields.begin();
	ite = header_fields.end();
	for(;it != ite; it++)
		mapped_cgi_env["HTTP_" + upper_key(it->first)] = it->second;
	int size = mapped_cgi_env.size();
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

std::string Cgi::getCgiPath() const {
	return (_cgi_path);
}

int Cgi::getStatusCode() const {
	return (_status_code);
}
