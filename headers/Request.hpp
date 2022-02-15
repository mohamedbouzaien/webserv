#pragma once
#include "Connector.hpp"
#include <iostream>
#include <cstdlib>
#include <map>
#define GET "GET"
#define POST "POST"
#define DELETE "DELETE"
#define BAD_REQUEST "BAD REQUEST"
#define BUFFER_SIZE 30000

class Request {
	private:
		int _client_socket;
		std::string _method;
		std::string _path;
		std::string _query_string;
		std::string _protocol;
		std::pair<std::string, std::string> _host;
		std::map<std::string, std::string> _header_fields;
		std::string _body;
		int _uri_length;
		int _body_length;


	public:
		Request();
		Request(int socket);
		Request (const Request &other);
		~Request();
		Request &operator=(const Request &other);

		void clear();
		int getWordEnd(const char *s) const; 
		int setRequestLine(char *buffer);
		int setRequestField(char *buffer);
		int setHostField(char *buffer);
		void parseRequest(char *buffer);
		void setHeaderField(std::string, char *);
		int recvSocket(std::string& request);
		int readSocket(std::string& request, std::string pattern);
		int readSocket(std::string& request, size_t len);
		int handle();
		//Setters
		void setClientSocket(int socket);
		void setMethod(std::string method);
		void setPath(std::string path);
		void setProtocol(std::string protocol);
		void setHost(std::pair<std::string, std::string> host);
		void setHeaderFields(std::map<std::string, std::string > header_fields);
		void setBody(std::string body);
		void setUriLength(int len);
		void setBodyLength(int len);
		//Getters
		int getClientSocket() const;
		std::string getMethod() const ;
		std::string getPath() const ;
		std::string getQueryString() const ;
		std::string getProtocol() const ;
		std::pair<std::string, std::string> getHost() const ;
		std::map<std::string, std::string> getHeaderFields() const;
		std::string getBody() const;
		int getUriLength() const;
		int getBodyLength() const;
		//EXCEPTIONS

		class MallocFailedException : public std::exception
		{
			public:
				virtual const char* what() const throw();
		};

		// << Overload
		friend std::ostream& operator<<(std::ostream& os, const Request& request);
};
