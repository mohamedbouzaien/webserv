/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouzaie <mbouzaie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/25 15:10:06 by mbouzaie          #+#    #+#             */
/*   Updated: 2022/02/22 11:58:41 by mbouzaie         ###   ########.fr       */
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
# include <sys/stat.h>

# include "Request.hpp"
# include "Config.hpp"
# include "Cgi.hpp"

#define URI_MAX_LEN 280000

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
		std::map<int, std::string>			_error_pages;

		void		addHeader(std::string key, std::string value);
		void		initMime();
		void		initCodes();
		void		handleHeader(std::string path, int code);
		void		retreiveBody(std::string path, int code);
		bool 		endsWith(std::string const & value, std::string const & ending);
		int			pathIsFile(std::string const &path);
		int			setLocationBlock(std::string const &path);
		void		getMethod(Request &request);
		void		deleteMethod(std::string const &path);
		void		postMethod(Request &request);
	public:
		Response(const Server_t &conf);
		Response(const Response &copy);
		Response	&operator=(const Response &other);
		~Response();
		void		prepare(Request &request);
		std::string parse(void);
};
