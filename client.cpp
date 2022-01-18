#include "header.hpp"

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
	servaddr.sin_port = htons(80);
	if (inet_pton(AF_INET, av[1], &servaddr.sin_addr) <= 0) {
		std::cout << "Cant convert arg to IP address" << std::endl;
		exit(1);
	}
	if (connect(sockfd, (sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
		std::cout << "Cant connect to server" << std::endl;
		exit(1);
	}
	std::string request = "GET /e HTTP/1.1\re\nhost: e:80\r\nLOl: e\r\n\n";
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
		}
		else {
			std::cout << "Socket closed" << std::endl;
			break;
		}
		memset(response, 0, MAXLINE);
	}
	close(sockfd);
}
