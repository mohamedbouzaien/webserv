#include "../headers/Cgi.hpp"

const char* Cgi::MallocFailedException::what() const throw() {
	return ("Malloc failed");
}


Cgi::Cgi(char *path) {
	if (!(_cgi_path = static_cast<char **>(malloc(sizeof(char *) * (1 + 1)))))
		throw Cgi::MallocFailedException();
	_cgi_path[0] = path;
	_cgi_path[1] = NULL;
}

Cgi::Cgi(const Cgi &other) {
	*this = other;
}

Cgi::~Cgi() {
	free(_cgi_path);
	_cgi_path = NULL;
}

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
		execve(_cgi_path[0], _cgi_path, request.getCgiEnv());
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
	*_cgi_path = path;
}

//Getter

char *Cgi::getCgiPath() const {
	return (*_cgi_path);
}
