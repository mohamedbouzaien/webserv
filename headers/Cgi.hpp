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
		char *_cgi_path;
		char _output[CGI_BUFFER_SIZE];
		int _body_pipe[2]; // PARENT -> CHILD, SEND BODY
		int _output_pipe[2]; // CHILD -> PARENT, SEND CGI OUTPUT
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
		char *getCgiPath() const;
		char *getOutput() const;
		int getStatusCode() const;
		void setStatusCode(std::string buffer);
		void setCgiEnv(Request &request);
		void setCgiEnvVar(const char *var, int pos);
		class MallocFailedException : public std::exception
		{
			public:
				virtual const char* what() const throw();
		};
};
