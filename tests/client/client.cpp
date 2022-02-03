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
	servaddr.sin_port = htons(SERVER_PORT);
	if (inet_pton(AF_INET, av[1], &servaddr.sin_addr) <= 0) {
		std::cout << "Cant convert arg to IP address" << std::endl;
		exit(1);
	}
	if (connect(sockfd, (sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
		std::cout << "Cant connect to server" << std::endl;
		exit(1);
	}

	std::cout << "ok" << std::endl;
	std::string request = "POST /secretary_response.php HTTP/1.1\nhost: e\nContent-Type: application/x-www-form-urlencoded\nAccept: text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.9\nAccept-Language: fr-FR,fr;q=0.9,en-US;q=0.8,en;q=0.7\nUser-Agent: Mozilla/5.0 (Macintosh; Intel Mac OS X 10_15_7) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/97.0.4692.71 Safari/537.36\nTransfer-Encoding: chunked\n\n";

	std::cout << "ok" << std::endl;
	send(sockfd, request.c_str(), request.size(), 0);
	request = "C\r\nnom=ad&age=2\r\n0\r\n\r\n";
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
