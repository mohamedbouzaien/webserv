#pragma once
#include <iostream>
#include <list>
#include <map>
#define BAD_REQUEST 0
#define GET 1
#define POST 2
#define DELETE 3

class Request {
	private:
		int _method;
		std::string _path;
		std::string _protocol;
		std::string _host;
		std::map<std::string, std::list<std::pair<std::string, std::list<std::pair<std::string, std::string> > > > > _params;

	public:
		Request();
		Request (const Request &other);
		~Request();
		Request &operator=(const Request &other);

		int  getLine(const char *s) const;
		int getWordEnd(const char *s) const; 
		int setRequestLine(char *header);
		int setRequestField(char *header);
		int setHostField(char *header);
		std::list<std::pair<std::string, std::string> > setAcceptParams(char *header);
		std::list<std::pair<std::string, std::list<std::pair<std::string, std::string> > > > setListField(char *header);
		void parseRequest(char *header);
		//Setters
		void setMethod(int method);
		void setPath(std::string path);
		void setProtocol(std::string protocol);
		void setHost(std::string host);
		//Getters
		int  getMethod() const ;
		std::string getPath() const ;
		std::string getProtocol() const ;
		std::string getHost() const ;
};
