#pragma once
#include "Request.hpp"
#include <unistd.h>
#define SIDE_IN 0
#define SIDE_OUT 1
#define CGI_BUFFER_SIZE 4096
#define INTERNAL_SERVER_ERROR 500
#define OK 200
class Cgi {
	private:

		std::string _cgi_path;
		char _output[CGI_BUFFER_SIZE];
		int _body_pipe[2]; // PARENT -> CHILD, SEND BODY
		int _output_pipe[2]; // CHILD -> PARENT, SEND CGI OUTPUT
		char *_body;
		int _body_size;
		int _status_code;
		char **_cgi_env;

	public:
		Cgi(char *path, Request &request);
		Cgi(const Cgi &other);
		~Cgi();
		Cgi &operator=(const Cgi &other);

		void runCgi(Request &request);
		void setCgiPath(char *path);
		std::string getCgiPath() const;
		char *getOutput() const;
		int getStatusCode() const;
		void setStatusCode(std::string buffer);
		void setCgiEnv(Request &request);
		std::string upper_key(std::string key) const;
		class MallocFailedException : public std::exception
		{
			public:
				virtual const char* what() const throw();
		};
};
