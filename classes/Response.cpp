/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouzaie <mbouzaie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/25 15:09:59 by mbouzaie          #+#    #+#             */
/*   Updated: 2022/01/27 12:44:50 by mbouzaie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../headers/Response.hpp"

Response::Response()
{
	this->initMime();
}

Response::Response(const Response &copy)
{
	(void)copy;
	this->initMime();
}

Response	&Response::operator=(const Response &other)
{
	(void)other;
	return (*this);
}

Response::~Response()
{

}

void		Response::addHeader(std::string key, std::string value)
{
	this->_header.insert(std::make_pair<std::string, std::string>(key, value));
}

void		Response::initMime()
{
	this->_mime.insert(std::make_pair<std::string, std::string>(".aac", "audio/aac"));
	this->_mime.insert(std::make_pair<std::string, std::string>("abw", "application/x-abiword"));
	this->_mime.insert(std::make_pair<std::string, std::string>(".arc", "application/x-freearc"));
	this->_mime.insert(std::make_pair<std::string, std::string>(".avif", "image/avif"));
	this->_mime.insert(std::make_pair<std::string, std::string>(".avi", "video/x-msvideo"));
	this->_mime.insert(std::make_pair<std::string, std::string>(".azw", "application/vnd.amazon.ebook"));
	this->_mime.insert(std::make_pair<std::string, std::string>(".bin", "application/octet-stream"));
	this->_mime.insert(std::make_pair<std::string, std::string>(".bmp", "image/bmp"));
	this->_mime.insert(std::make_pair<std::string, std::string>(".bz", "application/x-bzip"));
	this->_mime.insert(std::make_pair<std::string, std::string>(".bz2", "application/x-bzip2"));
	this->_mime.insert(std::make_pair<std::string, std::string>(".cda", "application/x-cdf"));
	this->_mime.insert(std::make_pair<std::string, std::string>(".csh", "application/x-csh"));
	this->_mime.insert(std::make_pair<std::string, std::string>(".css", "text/css"));
	this->_mime.insert(std::make_pair<std::string, std::string>(".csv", "text/csv"));
	this->_mime.insert(std::make_pair<std::string, std::string>(".doc", "application/msword"));
	this->_mime.insert(std::make_pair<std::string, std::string>(".docx", "application/vnd.openxmlformats-officedocument.wordprocessingml.document"));
	this->_mime.insert(std::make_pair<std::string, std::string>(".eot", "application/vnd.ms-fontobject"));
	this->_mime.insert(std::make_pair<std::string, std::string>(".epub", "application/epub+zip"));
	this->_mime.insert(std::make_pair<std::string, std::string>(".gz", "application/gzip"));
	this->_mime.insert(std::make_pair<std::string, std::string>(".gif", "image/gif"));
	this->_mime.insert(std::make_pair<std::string, std::string>(".htm", "text/html"));
	this->_mime.insert(std::make_pair<std::string, std::string>(".html", "text/html"));
	this->_mime.insert(std::make_pair<std::string, std::string>(".ico", "image/vnd.microsoft.icon"));
	this->_mime.insert(std::make_pair<std::string, std::string>(".ics", "audio/aac"));
	this->_mime.insert(std::make_pair<std::string, std::string>(".jar", "application/java-archive"));
	this->_mime.insert(std::make_pair<std::string, std::string>(".jpeg", "image/jpeg"));
	this->_mime.insert(std::make_pair<std::string, std::string>(".jpg", "image/jpeg"));
	this->_mime.insert(std::make_pair<std::string, std::string>(".js", "text/javascript"));
	this->_mime.insert(std::make_pair<std::string, std::string>(".json", "application/json"));
	this->_mime.insert(std::make_pair<std::string, std::string>(".jsonld", "application/ld+json"));
	this->_mime.insert(std::make_pair<std::string, std::string>(".mid", "audio/x-midi"));
	this->_mime.insert(std::make_pair<std::string, std::string>(".midi", "audio/x-midi"));
	this->_mime.insert(std::make_pair<std::string, std::string>(".mjs", "text/javascript"));
	this->_mime.insert(std::make_pair<std::string, std::string>(".mp3", "audio/mpeg"));
	this->_mime.insert(std::make_pair<std::string, std::string>(".mp4", "video/mp4"));
	this->_mime.insert(std::make_pair<std::string, std::string>(".mpeg", "video/mpeg"));
	this->_mime.insert(std::make_pair<std::string, std::string>(".mpkg", "application/vnd.apple.installer+xml"));
	this->_mime.insert(std::make_pair<std::string, std::string>(".odp", "application/vnd.oasis.opendocument.presentation"));
	this->_mime.insert(std::make_pair<std::string, std::string>(".ods", "application/vnd.oasis.opendocument.spreadsheet"));
	this->_mime.insert(std::make_pair<std::string, std::string>(".odt", "application/vnd.oasis.opendocument.text"));
	this->_mime.insert(std::make_pair<std::string, std::string>(".oga", "audio/ogg"));
	this->_mime.insert(std::make_pair<std::string, std::string>(".ogv", "video/ogg"));
	this->_mime.insert(std::make_pair<std::string, std::string>(".ogx", "application/ogg"));
	this->_mime.insert(std::make_pair<std::string, std::string>(".opus", "audio/opus"));
	this->_mime.insert(std::make_pair<std::string, std::string>(".otf", "font/otf"));
	this->_mime.insert(std::make_pair<std::string, std::string>(".png", "image/png"));
	this->_mime.insert(std::make_pair<std::string, std::string>(".pdf", "application/pdf"));
	this->_mime.insert(std::make_pair<std::string, std::string>(".php", "application/x-httpd-php"));
	this->_mime.insert(std::make_pair<std::string, std::string>(".ppt", "application/vnd.ms-powerpoint"));
	this->_mime.insert(std::make_pair<std::string, std::string>(".pptx", "application/vnd.openxmlformats-officedocument.presentationml.presentation"));
	this->_mime.insert(std::make_pair<std::string, std::string>(".rar", "application/vnd.rar"));
	this->_mime.insert(std::make_pair<std::string, std::string>(".rtf", "application/rtf"));
	this->_mime.insert(std::make_pair<std::string, std::string>(".sh", "application/x-sh"));
	this->_mime.insert(std::make_pair<std::string, std::string>(".svg", "image/svg+xml"));
	this->_mime.insert(std::make_pair<std::string, std::string>(".swf", "application/x-shockwave-flash"));
	this->_mime.insert(std::make_pair<std::string, std::string>(".tar", "application/x-tar"));
	this->_mime.insert(std::make_pair<std::string, std::string>(".tif", "image/tiff"));
	this->_mime.insert(std::make_pair<std::string, std::string>(".tiff", "image/tiff"));
	this->_mime.insert(std::make_pair<std::string, std::string>(".ts", "video/mp2t"));
	this->_mime.insert(std::make_pair<std::string, std::string>(".ttf", "font/ttf"));
	this->_mime.insert(std::make_pair<std::string, std::string>(".txt", "text/plain"));
	this->_mime.insert(std::make_pair<std::string, std::string>(".vsd", "application/vnd.visio"));
	this->_mime.insert(std::make_pair<std::string, std::string>(".wav", "audio/wav"));
	this->_mime.insert(std::make_pair<std::string, std::string>(".weba", "audio/webm"));
	this->_mime.insert(std::make_pair<std::string, std::string>(".webm", "video/webm"));
	this->_mime.insert(std::make_pair<std::string, std::string>(".webp", "image/webp"));
	this->_mime.insert(std::make_pair<std::string, std::string>(".woff", "font/woff"));
	this->_mime.insert(std::make_pair<std::string, std::string>(".woff2", "font/woff2"));
	this->_mime.insert(std::make_pair<std::string, std::string>(".xhtml", "application/xhtml+xml"));
	this->_mime.insert(std::make_pair<std::string, std::string>(".xls", "application/vnd.ms-excel"));
	this->_mime.insert(std::make_pair<std::string, std::string>(".xlsx", "application/vnd.openxmlformats-officedocument.spreadsheetml.sheet"));
	this->_mime.insert(std::make_pair<std::string, std::string>(".xml", "application/xml"));
	this->_mime.insert(std::make_pair<std::string, std::string>(".xul", "application/vnd.mozilla.xul+xml"));
	this->_mime.insert(std::make_pair<std::string, std::string>(".zip", "application/zip"));
	this->_mime.insert(std::make_pair<std::string, std::string>(".3gp", "video/3gpp"));
	this->_mime.insert(std::make_pair<std::string, std::string>(".3g2", "video/3gpp2"));
	this->_mime.insert(std::make_pair<std::string, std::string>(".7z", "application/x-7z-compressed"));
}

void		Response::handleHeader(std::string path)
{
	size_t	index = path.find_last_of('.');
	bool	cond = false;

	if (index != std::string::npos)
	{
		std::cout << path << std::endl;
		std::map<std::string, std::string>::iterator	it = this->_mime.find(path.substr(index, path.size()));
		if (it != this->_mime.end())
		{
			this->addHeader("Content-Type", it->second);
			cond = true;
		}
	}
	if (!cond)
	{
		this->_mime.erase(this->_mime.find(path.substr(index, path.size()))->first);
		this->addHeader("Content-Type", "text/plain");
	}
}

void		Response::prepare(Request &request)
{
	std::ifstream   	indata;
	std::ostringstream	sstr;

	indata.open(request.getPath().substr(1), std::ifstream::in);
	if(!indata)
	{
		std::cerr << "Error: file could not be opened =>" << request.getPath().substr(1) << std::endl;
	}
	else
	{
		this->handleHeader(request.getPath());
		sstr << indata.rdbuf();
		this->_body = sstr.str();
		this->_code = 200;
		indata.close();
	}
}

std::string Response::parse(void)
{
	std::string hello = "HTTP/1.1 " + std::to_string(this->_code) + " OK\n";
	for (std::map<std::string, std::string>::iterator it = this->_header.begin(); it != this->_header.end(); it++)
		hello += it->first + ": " +  it->second + "\n";
	hello += "Content-Length: " + std::to_string(this->_body.size()) + "\n\n" + this->_body;
	return (hello);
}
