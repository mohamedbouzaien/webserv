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

void Cgi::runGetMethod(Request &request) const {
	int pid;

	if ((pid = fork()) < 0) {
		std::cout << "Fork error" << std::endl;
		return ;
	}
	if (pid == 0) // Child
	{
		execve(_cgi_path, NULL, request.getCgiEnv());
		exit(1);
	}
	else
		wait(NULL);
}

void Cgi::runCgi(Request &request) const {
	if (request.getMethod() == GET)
		runGetMethod(request);
}

//Setter

void Cgi::setCgiPath(char *path) {
	_cgi_path = path;
}

//Getter

char *Cgi::getCgiPath() const {
	return (_cgi_path);
}
