#pragma once
#include "Request.hpp"
#include <unistd.h>
class Cgi {
	private:
		char *_cgi_path;
	public:
		Cgi(char *path);
		Cgi(const Cgi &other);
		~Cgi();
		Cgi &operator=(const Cgi &other);

		void runGetMethod(Request &request) const;
		void runCgi(Request &request) const;
		//Setter
		void setCgiPath(char *path);

		//Getter
		char *getCgiPath() const;
		class MallocFailedException : public std::exception
		{
			public:
				virtual const char* what() const throw();
		};
};
