#pragma once
#include "Request.hpp"
#include "Server_t.hpp"
#include "common.hpp"
#define SIDE_IN 0
#define SIDE_OUT 1
#define CGI_BUFFER_SIZE 30000
#define INTERNAL_SERVER_ERROR 500
#define OK 200

class Cgi {
	private:

		Server_t _conf;
		std::string _cgi_path;
		std::string _translated_path;
		std::string _output;
		std::string _upload_to;
		int _body_pipe[2]; // PARENT -> CHILD, SEND BODY
		int _output_pipe[2]; // CHILD -> PARENT, SEND CGI OUTPUT
		char *_body;
		size_t _body_size;
		int _status_code;
		char **_cgi_env;
		std::map<std::string, std::string> _response_header;

	public:
		Cgi();
		Cgi(Server_t &conf, std::string t_path, Request &request);
		Cgi(const Cgi &other);
		~Cgi();
		Cgi &operator=(const Cgi &other);

		void runCgi();
		void processOutput();
		void readHeader();
		void parseHeader(std::string &);
		void readBody();
		// Setters
		void setConf(Server_t);
		void setCgiPath(std::string path);
		void setTranslatedPath(std::string t_path);
		void setOutput(std::string output);
		void setUploadTo(std::string location);
		void setBody(const std::vector<char> vbody);
		void setBodySize(size_t size);
		void setResponseHeader(std::map<std::string, std::string>);
		void setStatusCode(int code);
		void setCgiEnv(Request &request);

		//Getters
		Server_t getConf() const;
		std::string getTranslatedPath() const ;
		std::string getCgiPath() const;
		std::string getOutput() const;
		std::string getUploadTo() const;
		char *getBody() const;
		int getBodySize() const;
		int getStatusCode() const;
		std::map<std::string, std::string> getResponseHeader() const;
		char **getCgiEnv() const;
		std::string upper_key(std::string key) const;
		class MallocFailedException : public std::exception
		{
			public:
				virtual const char* what() const throw();
		};

		friend std::ostream& operator<<(std::ostream& os, const Cgi& cgi);

		void free_cgi_env();
};
