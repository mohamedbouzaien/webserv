/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouzaie <mbouzaie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/25 15:10:06 by mbouzaie          #+#    #+#             */
/*   Updated: 2022/01/26 14:56:26 by mbouzaie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

# include <iostream>
# include <string>
# include <map>
# include <cstdio>
# include <fstream>
# include <sstream>

# include "Request.hpp"


class Response
{
	private:
		int									_code;
		std::map<std::string, std::string>	_header;		
		std::string							_body;
		std::map<std::string, std::string>	_mime;

		void		addHeader(std::string key, std::string value);
		void		initMime();
		void		handleHeader(std::string path);
	public:
		Response();
		Response(const Response &copy);
		Response	&operator=(const Response &other);
		~Response();
		void		prepare(Request &request);
		std::string parse(void);
};
