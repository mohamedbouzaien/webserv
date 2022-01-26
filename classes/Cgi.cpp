#include "../headers/Cgi.hpp"

const char* Cgi::MallocFailedException::what() const throw() {
	return ("Malloc failed");
}


Cgi::Cgi(char *path) : _cgi_path(path) {
	memset((char *)_buffer, 0, CGI_BUFFER_SIZE);
}

Cgi::Cgi(const Cgi &other) {
	*this = other;
}

Cgi::~Cgi() {}

Cgi &Cgi::operator=(const Cgi &other) {
	if (this != &other)
		_cgi_path = other._cgi_path;
	return (*this);
}

void Cgi::runCgi(Request &request) const {

	int body_size;
	int pid;
	int stdin_cp = dup(STDIN_FILENO);

	body_size = atoi(request.getHeaderFields()["CONTENT-LENGTH"].c_str());
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
		execve(_cgi_path, NULL, request.getCgiEnv());
		exit(1);
	}
	else {
		close(_body_pipe[SIDE_IN]);
		write(_body_pipe[SIDE_OUT], request.getBody().c_str(), body_size);
		close(_body_pipe[SIDE_OUT]);
		close(_output_pipe[SIDE_OUT]);
		wait(NULL);
		read(_output_pipe[SIDE_IN], (char *)_buffer, CGI_BUFFER_SIZE - 1);
		close(_output_pipe[SIDE_IN]);
	}
	std::cout << "BUFFER:" << std::endl << _buffer << std::endl;
	dup2(STDIN_FILENO, stdin_cp);
}

//Setter

void Cgi::setCgiPath(char *path) {
	_cgi_path = path;
}

//Getter

char *Cgi::getCgiPath() const {
	return (_cgi_path);
}
