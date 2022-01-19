#pragma once
#include "Header.hpp"

class Request {
	private:
		Header _header;

	public:
		Request();
		Request (const Request &other);
		~Request();
		Request &operator=(const Request &other);

		int  getLine(const char *s) const;
		int getWordEnd(const char *s) const; 
		int setRequestLine(char *buffer);
		int setRequestField(char *buffer);
		int setHostField(char *buffer);
		std::list<std::pair<std::string, std::string> > setAcceptParams(char *buffer);
		std::list<std::pair<std::string, std::list<std::pair<std::string, std::string> > > > setListField(char *buffer);
		void parseRequest(char *buffer);

		//Printer
		void printHeader();
		//Setters
		void setHeader(Header header);
		//Getters
		Header  getHeader() const ;
};
