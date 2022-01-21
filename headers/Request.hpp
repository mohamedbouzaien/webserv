#pragma once
#include <iostream>
#include <map>
#define GET 1
#define POST 2
#define DELETE 3
#define BAD_REQUEST 4

class Request {
	private:
		int _method;
		std::string _path;
		std::string _query_string;
		std::string _protocol;
		std::pair<std::string, std::string> _host;
		std::map<std::string, std::string> _header_fields;
		std::string _body;
		const char *_cgi_env[23];

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
		void convertToCgiEnv();

				//Setters
		void setMethod(int method);
		void setPath(std::string path);
		void setProtocol(std::string protocol);
		void setHost(std::pair<std::string, std::string> host);
		void setHeaderFields(std::map<std::string, std::string > header_fields);
		//Getters
		int  getMethod() const ;
		std::string getPath() const ;
		std::string getProtocol() const ;
		std::pair<std::string, std::string> getHost() const ;
		std::map<std::string, std::string> getHeaderFields() const;
};
