#pragma once
#include <iostream>
#include <list>
#include <map>
#define BAD_REQUEST 0
#define GET 1
#define POST 2
#define DELETE 3

class Header {
	protected:
		int _method;
		std::string _path;
		std::string _protocol;
		std::pair<std::string, std::string> _host;
		std::map<std::string, std::list<std::pair<std::string, std::list<std::pair<std::string, std::string> > > > > _header_fields;

	public:
		Header();
		Header (const Header &other);
		virtual ~Header();
		Header &operator=(const Header &other);

		//Printer
		void show();
		//Setters
		void setMethod(int method);
		void setPath(std::string path);
		void setProtocol(std::string protocol);
		void setHost(std::pair<std::string, std::string> host);
		void setHeaderFields(std::map<std::string, std::list<std::pair<std::string, std::list<std::pair<std::string, std::string> > > > > header_fields);
		void insertHeaderField(std::string, std::list<std::pair<std::string, std::list<std::pair<std::string, std::string> > > > header_field);
		//Getters
		int  getMethod() const ;
		std::string getPath() const ;
		std::string getProtocol() const ;
		std::pair<std::string, std::string> getHost() const ;
};
