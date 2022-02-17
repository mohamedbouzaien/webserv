/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouzaie <mbouzaie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/25 15:10:06 by mbouzaie          #+#    #+#             */
/*   Updated: 2022/02/17 14:44:25 by acastelb         ###   ########.fr       */
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

		void		addHeader(std::string key, std::string value);
		void		initMime();
		void		initCodes();
		void		handleHeader(std::string path, int code, const Server_t &serv_conf);
		void		retreiveBody(std::string path, int code, const Server_t &serv_conf);
		bool 		endsWith(std::string const & value, std::string const & ending);
	public:
		Response();
		Response(const Response &copy);
		Response	&operator=(const Response &other);
		~Response();
		void		prepare(Request &request, const Server_t &serv_conf);
		std::string parse(void);
};
