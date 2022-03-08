#pragma once
#include <sys/socket.h>
#include <iostream>
#include <cstdlib>
#include <map>
#include <vector>
#include <algorithm>
#define GET "GET"
#define POST "POST"
#define DELETE "DELETE"
#define PUT	"PUT"
#define BUFFER_SIZE 30000
#define MAX_MALLOC_SIZE 16711568
#define MAX_HEADER_SIZE 16000
#define MAX_URI_SIZE 2048

class Request {
	private:
		int _client_socket;
		std::string _method;
		std::string _path;
		std::string _query_string;
		std::string _protocol;
		std::pair<std::string, std::string> _host;
		std::map<std::string, std::string> _header_fields;
		std::vector<char> _body;
		int _is_body;
		int _status_code;

	public:
		Request();
		Request(int socket);
		Request (const Request &other);
		~Request();
		Request &operator=(const Request &other);

		void clear();
		int getWordEnd(const char *s) const; 
		int setRequestLine(std::string buffer);
		int setRequestField(std::string field);
		int setHostField(std::string host);
		void parseHeader(std::string header);
		void setHeaderField(std::string, std::string);
		int recvHeader(std::string& header);
		int readHeader(std::string& header);
		int readChunkedBody(int status, size_t max_body_size);
		int searchEndline(std::vector<char> &vector) const;
		int getChunkSize(std::vector<char> &vector) const;
		int unchunkBody(std::vector<char> &body_buffer);
		int readBody(size_t len, size_t max_body_size);
		int isHeaderEnded(std::string &request, char *buffer) ;
		int readAndParseHeader();
		int readAndParseBody(int status, size_t max_body_size);
		//Setters
		void setClientSocket(int socket);
		void setMethod(std::string method);
		void setPath(std::string path);
		void setProtocol(std::string protocol);
		void setHost(std::pair<std::string, std::string> host);
		void setHeaderFields(std::map<std::string, std::string > header_fields);
		void setBody(std::vector<char> vbody);
		void setStatusCode(int status_code);
		//Getters
		std::string search(std::string) const;
		int getClientSocket() const;
		int getStatusCode() const;
		std::string getMethod() const ;
		std::string getPath() const ;
		std::string getQueryString() const ;
		std::string getProtocol() const ;
		std::pair<std::string, std::string> getHost() const ;
		std::map<std::string, std::string> getHeaderFields() const;
		std::vector<char> getBody() const;
		//EXCEPTIONS

		class MallocFailedException : public std::exception
		{
			public:
				virtual const char* what() const throw();
		};

		// << Overload
		friend std::ostream& operator<<(std::ostream& os, const Request& request);
};
