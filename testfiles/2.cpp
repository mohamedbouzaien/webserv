#include "header.hpp"

int main(int ac, char **av) {
	int sockfd;
	int n;
	sockaddr_in servaddr;
	std::string sendline;
	std::string recvline;

	if (ac != 3) {
		std::cout << "usage : " << av[0] << "<server address> <request>" << std::endl;
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
	send(sockfd, av[2], strlen(av[2]), 0);
	char response[MAXLINE];
	memset(response, 0, MAXLINE);
	read(sockfd, response, MAXLINE - 1);
	std::cout << response;
	close(sockfd);
}
