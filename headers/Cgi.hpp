#pragma once
#include "Request.hpp"
#include <unistd.h>
#define SIDE_IN 0
#define SIDE_OUT 1
#define CGI_BUFFER_SIZE 96
#define INTERNAL_SERVER_ERROR 500
#define OK 200

class Cgi {
	private:

		std::string _cgi_path;
		std::string _translated_path;
		std::string _output;
		int _body_pipe[2]; // PARENT -> CHILD, SEND BODY
		int _output_pipe[2]; // CHILD -> PARENT, SEND CGI OUTPUT
		char *_body;
		int _body_size;
		int _status_code;
		char **_cgi_env;
		std::map<std::string, std::string> _response_header;

	public:
		Cgi(std::string cgi_path, std::string t_path, Request &request);
		Cgi(const Cgi &other);
		~Cgi();
		Cgi &operator=(const Cgi &other);

		void runCgi();
		void processOutput();
		void readHeader();
		void parseHeader(std::string &);
		void readBody();
		// Setters
		void setCgiPath(char *path);
		void setTranslatedPath(std::string t_path);
		void setResponseHeader(std::map<std::string, std::string>);
		void setStatusCode(int code);
		void setCgiEnv(Request &request);

		//Getters
		std::string getTranslatedPath() const ;
		std::string getCgiPath() const;
		std::string getOutput() const;
		int getStatusCode() const;
		std::map<std::string, std::string> getResponseHeader() const;
		std::string upper_key(std::string key) const;
		class MallocFailedException : public std::exception
		{
			public:
				virtual const char* what() const throw();
		};
};
