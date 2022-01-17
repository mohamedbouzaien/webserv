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
		int setRequestLine(char *header);
		int setRequestField(char *header);
		int setHostField(char *header);
		std::list<std::pair<std::string, std::string> > setAcceptParams(char *header);
		std::list<std::pair<std::string, std::list<std::pair<std::string, std::string> > > > setListField(char *header);
		void parseRequest(char *header);

		//Printer
		void printHeader();
		//Setters
		void setHeader(Header header);
		//Getters
		Header  getHeader() const ;
};
