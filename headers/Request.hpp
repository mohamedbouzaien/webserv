#pragma once
#include <iostream>
#include <map>
#define GET "GET"
#define POST "POST"
#define DELETE "DELETE"
#define BAD_REQUEST "BAD REQUEST"
#define CGI_ENV_SIZE 24

class Request {
	private:
		std::string _method;
		std::string _path;
		std::string _query_string;
		std::string _protocol;
		std::pair<std::string, std::string> _host;
		std::map<std::string, std::string> _header_fields;
		std::string _body;

	public:
		Request();
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
		//Printer
		void printRequest();

				//Setters
		void setMethod(std::string method);
		void setPath(std::string path);
		void setProtocol(std::string protocol);
		void setHost(std::pair<std::string, std::string> host);
		void setHeaderFields(std::map<std::string, std::string > header_fields);
		void setBody(std::string body);
		//Getters
		std::string getMethod() const ;
		std::string getPath() const ;
		std::string getQueryString() const ;
		std::string getProtocol() const ;
		std::pair<std::string, std::string> getHost() const ;
		std::map<std::string, std::string> getHeaderFields() const;
		std::string getBody() const;
		//EXCEPTIONS

		class MallocFailedException : public std::exception
		{
			public:
				virtual const char* what() const throw();
		};
};
