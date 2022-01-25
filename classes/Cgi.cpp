#include "../headers/Cgi.hpp"

const char* Cgi::MallocFailedException::what() const throw() {
	return ("Malloc failed");
}


Cgi::Cgi(char *path) : _cgi_path(path) {}

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
	int pid;
	int pfd[2];

	if (pipe(pfd) < 0) {
		std::cout << "Pipe error" << std::endl;
		return ;
	}
	if ((pid = fork()) < 0) {
		std::cout << "Fork error" << std::endl;
		return ;
	}
	if (pid == 0) {
		close(pfd[SIDE_OUT]);
		dup2(pfd[SIDE_IN], STDIN_FILENO);
		close(pfd[SIDE_IN]);
		execve(_cgi_path, NULL, request.getCgiEnv());
		exit(1);
	}
	else {
		close(pfd[SIDE_IN]);
		dup2(pfd[SIDE_OUT], STDOUT_FILENO);
		close(pfd[SIDE_OUT]);
		write(1, request.getBody().c_str(), request.getBody().size());
		wait(NULL);
	}
}

//Setter

void Cgi::setCgiPath(char *path) {
	_cgi_path = path;
}

//Getter

char *Cgi::getCgiPath() const {
	return (_cgi_path);
}
