#pragma once
#include "Request.hpp"

class Cgi {
	private:
		Request *_request;

	public:
		Cgi(Request *);
		Cgi(const Cgi &other);
		~Cgi();
		Cgi &operator=(const Cgi &other);

		//Setter
		void setRequest(Request *request);

		//Getter
		Request *getRequest() const;

};
