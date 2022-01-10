#include "header.hpp"

void	error_n_die(std::string s) {
	std::cout << s << std::endl;
	exit(1);
}

void generate_and_send_cgi(int new_socket) {
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
	send(new_socket, response.c_str(), response.size(), 0);
	free(s);
	free(env);
}

int		main(int ac, char **av) {
	int server_fd;
	int new_socket;
	int n;
	sockaddr_in servaddr;
	int addrlen = sizeof(servaddr);

	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		error_n_die("socket error");
	bzero(&servaddr, addrlen);
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(SERVER_PORT);
	if ((bind(server_fd, (sockaddr *) &servaddr, addrlen)) < 0)
		error_n_die("bind error");
	if ((listen(server_fd, 10)) < 0)
		error_n_die("listen error");
	while (1) {
		std::cout << std::endl << "+++++++ Waiting for new connection ++++++++" << std::endl << std::endl;
		if ((new_socket = accept(server_fd, (sockaddr *) NULL, NULL)) < 0)
			error_n_die("accept error");
		char request[MAXLINE] = {0};
		n = read(new_socket, request, MAXLINE -1);
		std::cout << request << std::endl;
		if (n < 0)
			error_n_die("read error");
		generate_and_send_cgi(new_socket);
		//std::string response("HTTP/1.1 200 OK\n\nHello world");
		//send(new_socket, response.c_str(), response.size(), 0);
		close(new_socket);
	}
}
