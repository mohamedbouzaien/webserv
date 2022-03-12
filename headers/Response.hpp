/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouzaie <mbouzaie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/25 15:10:06 by mbouzaie          #+#    #+#             */
/*   Updated: 2022/03/12 14:19:53 by mbouzaie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

# include <iostream>
# include <string>
# include <map>
# include <cstdio>
# include <fstream>
# include <sstream>

# include <time.h>
# include <dirent.h>
# include <sys/stat.h>

# include "Request.hpp"
# include "Config.hpp"
# include "Cgi.hpp"

#define URI_MAX_LEN 1000000

class Response
{
	private:
		std::map<int, std::string>			_codes;
		std::map<std::string, std::string>	_header;		
		std::string							_body;
		std::map<std::string, std::string>	_mime;
		std::vector<std::string>			_allowed_methods;
		Server_t							_conf;
		Location_t							_loc;
		std::string							_host;
		int									_port;
		Context_t							*_context;
		std::string							_old_path;
		

		void		addHeader(std::string key, std::string value);
		void		initMime();
		void		initCodes();
		void		handleHeader(std::string path, int code);
		void		retreiveBody(std::string path, int code);
		bool 		endsWith(std::string const & value, std::string const & ending);
		int			pathIsFile(std::string const &path);
		int			setLocationBlock(std::string const &path);
		void		getMethod(Request &request, std::string &real_path);
		void		deleteMethod(std::string const &path);
		void		postMethod(Request &request, std::string &real_path);
		void		putMethod(Request &request, std::string &real_path);
		void		listDirectory(std::vector<std::string> dir_cont);
		std::string	findIndex(std::vector<std::string> dir_cont);
		std::vector<std::string>	getDirContents(std::string const &path);
	public:
		Response(const Server_t &conf);
		Response(const Response &copy);
		Response	&operator=(const Response &other);
		~Response();
		void		prepare(Request &request);
		std::string parse(void);
};
