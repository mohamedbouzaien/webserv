/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouzaie <mbouzaie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/25 15:09:59 by mbouzaie          #+#    #+#             */
/*   Updated: 2022/01/26 00:24:13 by mbouzaie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../headers/Response.hpp"

Response::Response()
{

}

Response::Response(const Response &copy)
{
	(void)copy;
}

Response	&Response::operator=(const Response &other)
{
	(void)other;
	return (*this);
}

Response::~Response()
{

}

bool		Response::endsWith(std::string const &str, std::string const &end)
{
	if (str.length() >= end.length()) {
		return (0 == str.compare(str.length() - end.length(), end.length(), end));
	} else {
		return false;
	}
}

void		Response::prepare(Request &request)
{
	std::ifstream   	indata;
	std::ostringstream	sstr;

	indata.open(request.getPath().substr(1), std::ifstream::in);
	if(!indata)
	{
		std::cerr << "Error: file could not be opened" << std::endl;
		exit(1);
	}
	if (this->endsWith(request.getPath(), ".css"))
		this->_header_fields.insert(std::make_pair<std::string, std::string>("Content-Type", "text/css"));
	else if (this->endsWith(request.getPath(), ".html"))
		this->_header_fields.insert(std::make_pair<std::string, std::string>("Content-Type", "text/html; charset=UTF-8"));
	else if (this->endsWith(request.getPath(), ".js"))
		this->_header_fields.insert(std::make_pair<std::string, std::string>("Content-Type", "application/x-javascript"));
	else if (this->endsWith(request.getPath(), ".jpg"))
		this->_header_fields.insert(std::make_pair<std::string, std::string>("Content-Type", "image/jpeg"));
	else if (this->endsWith(request.getPath(), ".jpg"))
		this->_header_fields.insert(std::make_pair<std::string, std::string>("Content-Type", "image/x-icon"));	
	sstr << indata.rdbuf();
	this->_body = sstr.str();
	this->_code = 200;
	indata.close();
}

std::string Response::parse(void)
{
	std::string hello = "HTTP/1.1" + std::to_string(this->_code) + " OK\n" + this->_header_fields.begin()->first + ": " +  this->_header_fields.begin()->second + "\nContent-Length: " + std::to_string(this->_body.size()) + "\n\n" + this->_body;
	return (hello);
}
