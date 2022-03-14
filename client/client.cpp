#include "header.hpp"
#define SERVER_PORT 8080

int main(int ac, char **av) {
	int sockfd;
	int n;
	sockaddr_in servaddr;
	std::string sendline;
	std::string recvline;

	if (ac != 2) {
		std::cout << "usage : " << av[0] << "<server address>" << std::endl;
		exit(1);
	}
	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		std::cout << "Cant create socket" << std::endl;
		exit(1);
	}
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(SERVER_PORT);
	if (inet_pton(AF_INET, av[1], &servaddr.sin_addr) <= 0) {
		std::cout << "Cant convert arg to IP address" << std::endl;
		exit(1);
	}
	if (connect(sockfd, (sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
		std::cout << "Cant connect to server" << std::endl;
		exit(1);
	}
	std::string request = "GET / HTTP/1.1\r\nHost: z:1\r\nContent-Type: application/x-www-form-urlencoded\r\nContent-Length: 2\r\n\r\n11";

	std::cout << request.size() << std::endl;
	send(sockfd, request.c_str(), request.size(), 0);

	char response[MAXLINE];
	memset(response, 0, MAXLINE);
	pollfd pfds[1];
	pfds[0].fd = sockfd;
	pfds[0].events = POLLIN;
	poll(pfds, 1, -1);
	while (1)
	{
		pfds[0].fd = sockfd;
		pfds[0].events = POLLIN;
		poll(pfds, 1, -1);
		if (pfds[0].revents & POLLIN) {
			read(sockfd, response, MAXLINE - 1);
			std::cout << response;
			close(pfds[0].fd);
			break;
		}
		else {
			std::cout << "Socket closed" << std::endl;
			break;
		}
		memset(response, 0, MAXLINE);
	}
	close(sockfd);
}
