#include "header.hpp"
#include <fcntl.h>
void	error_n_die(std::string s) {
	std::cout << s << std::endl;
	exit(1);
}

void generate_and_send_cgi(int client_socket) {
	int pid;
	int pfd[2];
	char **s ;
	char cgi_res[MAXLINE] = {0};
	s = (char **)malloc(sizeof(char *) * 2);
	s[0] = "/usr/local/Cellar/php/8.1.1/bin/php-cgi";
	s[1] = NULL;

	char **env;
	env = (char **)malloc(sizeof(char *) * 5);

	env[0] = "SERVER_PROTOCOL=HTTP/1.1";
	env[1] = "SCRIPT_FILENAME=/Users/adriencastelbou/Desktop/42_sandbox/client-server-test/test.php";
	env[2] = "SCRIPT_NAME=test.php";
	env[3] = "REDIRECT_STATUS=200";
	pipe(pfd);
	pid = fork();
	if (pid == 0) // Child
	{
		close(pfd[0]);
		dup2(pfd[1], 1);
		close(pfd[1]);
		execve(s[0], s, env);
		exit(1);
	}
	else
	{
		close(pfd[1]);
		wait(NULL);
		read(pfd[0], cgi_res, MAXLINE -1);
		close (pfd[0]);
	}
	std::string response = "HTTP/1.1 200 OK\n";
	response = response + cgi_res;
	std::cout << response << std::endl;
	send(client_socket, response.c_str(), response.size(), 0);
	free(s);
	free(env);
}

int setup_server() {
	int server_socket;
	int client_socket;
	int n;
	sockaddr_in servaddr;
	int addrlen = sizeof(servaddr);

	if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		error_n_die("socket error");
	bzero(&servaddr, addrlen);
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(SERVER_PORT);
	if ((bind(server_socket, (sockaddr *) &servaddr, addrlen)) < 0)
		error_n_die("bind error");
	if ((listen(server_socket, 10)) < 0)
		error_n_die("listen error");
	return (server_socket);
}

int		accept_connection(int server_socket) {
	int client_socket;
	sockaddr_in client_addr;
	int addrlen = sizeof(client_addr);
		if ((client_socket = accept(server_socket, (sockaddr *) &client_addr, (socklen_t *)&addrlen)) < 0)
			error_n_die("accept error");
	return (client_socket);
}

void send_error(int client_socket) {

	std::string response = "HTTP/1.1 404 Not Found\n\nPage Not found\n";
	send(client_socket, response.c_str(), response.size(), 0);
}


void *handle_connection(int client_socket) {
	char buffer[MAXLINE] = {0};
	int bytes_read;
	int readed = 0;

	bytes_read = read(client_socket, buffer, MAXLINE - 1);
	if (bytes_read < 0)
		error_n_die("read error");
	std::cout << "REQUEST : " << buffer << std::endl;
	int file  = open(buffer, O_RDONLY);
	if (file < 0)
	{
		std::cout << "File error: " << buffer << std::endl;
		send_error(client_socket);
		close(file);
		close(client_socket);
		std::cout << "Closing connection" << std::endl;
		return (NULL);
	}
	while ((bytes_read = read(file, buffer, MAXLINE)) > 0) {
		std::cout << "Sending " << bytes_read << " bytes..." << std::endl;
		send(client_socket, buffer, bytes_read, 0);
	}
	//generate_and_send_cgi(client_socket);
	close(file);
	close(client_socket);
	std::cout << "Closing connection" << std::endl;
	return (NULL);
}

int		main(int ac, char **av) {
	int server_socket;
	int client_socket;

	server_socket = setup_server();
	while (true) {
		std::cout << std::endl << "+++++++ Waiting for new connection ++++++++" << std::endl << std::endl;
		client_socket = accept_connection(server_socket);
		handle_connection(client_socket);
	}
}
