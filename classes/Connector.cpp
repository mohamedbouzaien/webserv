/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Connector.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouzaie <mbouzaie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/11 16:37:13 by mbouzaie          #+#    #+#             */
/*   Updated: 2022/02/14 14:44:36 by acastelb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/Connector.hpp"
#include "../headers/Request.hpp"
#include "../headers/Cgi.hpp"

Connector::Connector(Listener &listener): _listener(listener)
{
	_client_socket = 0;
}

Connector::Connector(Connector &copy): _client_socket(copy._client_socket), _listener(copy._listener)
{
	
}

Connector	&Connector::operator=(Connector &other)
{
	if (this == &other)
		return (*this);
	this->_listener = other._listener;
	this->_client_socket = other._client_socket;
	return (*this);
}

const char* Connector::ConnectionFailedException::what() const throw()
{
	return ("Failed to grab connection");
}

const char* Connector::RecvFailedException::what() const throw()
{
	return ("Recv error");
}

void    Connector::accept_c()
{
	socklen_t	addrlen = sizeof(this->_listener.getAddress());
	_client_socket = accept(this->_listener.getFd(), (struct sockaddr*)&(_listener.getAddress()), &addrlen);
	if (_client_socket < 0)
		throw Connector::ConnectionFailedException();
}

int		Connector::readSocket(std::string &request) {
	char buffer[BUFFER_SIZE + 1];
	int	bytesRead;

	memset(buffer, 0, BUFFER_SIZE + 1);
	bytesRead = recv(_client_socket, buffer, BUFFER_SIZE, 0);
	request += buffer;
	memset(buffer, 0, BUFFER_SIZE + 1);
	return (bytesRead);
}

int		Connector::recvRequest(std::string &request, std::string patern) {
	int status;

	while (request.find(patern) == std::string::npos) {
		status = readSocket(request);
		if (status <= 0)
			return (-1);
		if (status != BUFFER_SIZE)
			break;
	}
	return (1);
}

int		Connector::recvRequest(std::string &request, size_t len) {
	int status;

	while (request.size() < len) {
		status = readSocket(request);
		if (status <= 0)
			return (-1);
		if (status != BUFFER_SIZE)
			break;
	}
	return (1);
}

int    Connector::handle()
{
	std::cout << "\033[1;31m--- Exchange Started ---\033[0m\n";
	Request request;
	std::string s_buffer;
	int status;
	int s_buffer_len;

	status = recvRequest(s_buffer, "\r\n\r\n");
	if (status < 1)
		return (status);
	request.parseRequest((char *)s_buffer.c_str());
	s_buffer_len = s_buffer.size();
	s_buffer.erase(0, s_buffer.find("\r\n\r\n") + 4);
	std::map<std::string, std::string> header_fields = request.getHeaderFields();
	if (header_fields.find("Transfer-Encoding") != header_fields.end() && header_fields["Transfer-Encoding"] == "chunked" && s_buffer_len >= BUFFER_SIZE)
		status = recvRequest(s_buffer, "0\r\n\r\n");
	else if (header_fields.find("Content-Length") != header_fields.end() && s_buffer_len >= BUFFER_SIZE)
		status = recvRequest(s_buffer, stoi(header_fields["Content-Length"]) );
	if (status < 1)
		return (status);
	request.setBody(s_buffer);
	std::string s("bin/php-cgi"); // Path to cgi binary
	Cgi cgi((char *)s.c_str(), request); // Cgi constr.
	cgi.runCgi(request); // run Cgi
	char *output = cgi.getOutput(); // get Cgi result, use getStatusCode for status code (int)
	char *body = strstr(output, "\r\n\r\n"); // get output body
	body += 4; // skip \r\n\r\n
	std::string result = ("HTTP/1.1 200 OK\nContent-Length: " + std::to_string(strlen(body)) + "\n" + output); // build test response
	send(_client_socket, result.c_str(), result.size(), 0);
	request.clear();
	std::cout << "\033[1;31m--- Exchange Ended ---\033[0m\n";
	return (1);
}

void	Connector::setClientSocket(int client_socket)
{
	this->_client_socket = client_socket;
}

int		Connector::getClientSocket()	const
{
	return (this->_client_socket);
}
