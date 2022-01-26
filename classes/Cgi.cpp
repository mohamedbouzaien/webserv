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

	int body_size;
	int pid;
	int pfd1[2]; // PARENT -> CHILD, SEND BODY
	int pfd2[2]; // CHILD -> PARENT, SEND CGI OUTPUT
	char buffer[CGI_BUFFER_SIZE];

	memset(buffer, 0, CGI_BUFFER_SIZE);
	int stdin_cp = dup(STDIN_FILENO);
	body_size = atoi(request.getHeaderFields()["CONTENT-LENGTH"].c_str());
	if (pipe(pfd1) < 0 || pipe(pfd2)) {
		std::cout << "Pipe error" << std::endl;
		return ;
	}
	if ((pid = fork()) < 0) {
		std::cout << "Fork error" << std::endl;
		return ;
	}
	if (pid == 0) {
		close(pfd1[SIDE_OUT]);
		dup2(pfd1[SIDE_IN], STDIN_FILENO);
		close(pfd1[SIDE_IN]);
		dup2(pfd2[SIDE_OUT], STDOUT_FILENO);
		close(pfd2[SIDE_OUT]);
		execve(_cgi_path, NULL, request.getCgiEnv());
		exit(1);
	}
	else {
		close(pfd1[SIDE_IN]);
		write(pfd1[SIDE_OUT], request.getBody().c_str(), body_size);
		close(pfd1[SIDE_OUT]);
		close(pfd2[SIDE_OUT]);
		wait(NULL);
		read(pfd2[SIDE_IN], buffer, CGI_BUFFER_SIZE - 1);
		close(pfd2[SIDE_IN]);
	}
	std::cout << "BUFFER:" << std::endl << buffer << std::endl;
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
