/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouzaie <mbouzaie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/25 15:09:59 by mbouzaie          #+#    #+#             */
/*   Updated: 2022/03/01 17:04:04 by acastelb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../headers/Response.hpp"

Response::Response(const Server_t &conf) : _conf(conf), _autoindex(false)
{
	this->initMime();
	this->initCodes();
}

Response::Response(const Response &copy)
{
	(void)copy;
	this->initMime();
	this->initCodes();
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
	this->_mime.insert(std::make_pair<std::string, std::string>(".php", "text/html"));
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

void		Response::initCodes()
{
	this->_codes.insert(std::make_pair<int, std::string>(100, "Continue"));
	this->_codes.insert(std::make_pair<int, std::string>(200, "OK"));
	this->_codes.insert(std::make_pair<int, std::string>(201, "Created"));
	this->_codes.insert(std::make_pair<int, std::string>(204, "No content"));
	this->_codes.insert(std::make_pair<int, std::string>(400, "Bad Request"));
	this->_codes.insert(std::make_pair<int, std::string>(403, "Forbidden"));
	this->_codes.insert(std::make_pair<int, std::string>(404, "Not Found"));
	this->_codes.insert(std::make_pair<int, std::string>(405, "Method Not Alloud"));
	this->_codes.insert(std::make_pair<int, std::string>(413, "Payload Too Large"));
	this->_codes.insert(std::make_pair<int, std::string>(414, "URI Too Long"));
	this->_codes.insert(std::make_pair<int, std::string>(500, "Internal Server Error"));
}

void		Response::handleHeader(std::string path, int code)
{
	size_t	index = path.find_last_of('.');
	bool	cond = false;

	this->addHeader(std::to_string(code) + " ", _codes[code]);
	if (index != std::string::npos)
	{
		std::cout << "Fetched => " << path << std::endl;
		std::map<std::string, std::string>::iterator	it = this->_mime.find(path.substr(index, path.size()));
		if (it != this->_mime.end())
		{
			this->addHeader("Content-type: ", it->second);
			cond = true;
		}
	}
	if (!cond)
	{
		this->_mime.erase(this->_mime.find(path.substr(index, path.size()))->first);
		this->addHeader("Content-type: ", "text/plain");
	}
	if (code == 405 || code == 413)
	{
		std::string	alloud;
		for (std::vector<std::string>::iterator it = _allowed_methods.begin(); it != _allowed_methods.end(); it++)
		{
			alloud += *it;
			if (it != _allowed_methods.end() - 1)
				alloud += ", ";
		}
		this->addHeader("Allow: ", alloud);
	}
	time_t		rawtime;
	struct	tm	*timeinfo;
	char 		buffer[80];

	time(&rawtime);
	timeinfo = localtime(&rawtime);
	strftime(buffer, 80, "%a, %d %b %Y %H:%M:%S GMT", timeinfo);
	this->addHeader("Date: ", std::string(buffer));
}

void		Response::retreiveBody(std::string path, int code)
{
	std::ifstream   	indata;
	std::ostringstream	sstr;
	int					path_type;

	path_type = pathIsFile(path.substr(1));
	if (path_type == 1)
	{
		indata.open(path.substr(1), std::ifstream::in);
		if (!indata.is_open())
			this->retreiveBody(_error_pages[403], 403);
		else
		{
			this->handleHeader(path, code);
			sstr << indata.rdbuf();
			this->_body = sstr.str();
			indata.close();
		}
	}
	else if (path_type == 0)
	{
		std::vector<std::string>	dir_conts = getDirContents(path);
		std::string index = findIndex(dir_conts);
		if (!index.empty())
		{
			if (!(path.back() == '/'))
				path += '/';
			this->retreiveBody(path + index, 200);
		}
		else if (_autoindex)
		{
			this->addHeader(std::to_string(code) + " ", _codes[code]);
			this->addHeader("Content-type: ", "text/html");
			this->listDirectory(path, dir_conts);
		}
	}
	else
	{		
		std::cerr << _autoindex <<" File not found => \"" << path.substr(1) << "\"" << std::endl;
		this->retreiveBody(_error_pages[404], 404);

	}
}

int			Response::pathIsFile(std::string const &path)
{
	struct stat s;
	if (stat(path.c_str(), &s) == 0)
	{
		if (s.st_mode & S_IFDIR)
			return (0);
		else if (s.st_mode & S_IFREG)
			return (1);
		else
			return (-1);
	}
	else
		return (-1);
}

std::vector<std::string>	Response::getDirContents(std::string const &path)
{
	DIR							*dir=opendir(path.substr(1).c_str());
	std::vector<std::string>	dir_contents;

	if (dir != NULL)
	{
		for (struct dirent *entry = readdir(dir); entry; entry = readdir(dir))
			dir_contents.push_back(std::string(entry->d_name));
		closedir(dir);
	}
	else
		std::cerr << "can't reach to directory" << std::endl;
	return (dir_contents);
}

void		Response::listDirectory(std::string const &path, std::vector<std::string> dir_cont)
{
		std::string	clean_p;
		
		if (path.back() == '/')
			clean_p = path.substr(0, path.length() - 1);
		else
			clean_p = path;
		this->_body ="<!DOCTYPE html><html><head><title>" + clean_p + "</title>\
		</head><body><h1>Index of " + path + "</h1><p>";
		for (std::vector<std::string>::iterator it = dir_cont.begin(); it != dir_cont.end(); it++)
			this->_body += "\t\t<p><a href=\"http://" + _host + ":" + std::to_string(_port) + clean_p + "/"\
			+ *it + "\">" + *it + "</a></p>";
		this->_body += "</p></body></html>";
}

std::string		Response::findIndex(std::vector<std::string> dir_cont)
{
	std::string							find_index;

	for (std::list<std::string>::iterator itl = _index_list.begin(); itl != _index_list.end(); ++itl)
		if (std::find(dir_cont.begin(), dir_cont.end(), *itl) != dir_cont.end())
			return (*itl);
	return (std::string());
}

bool 		Response::endsWith(std::string const &value, std::string const &ending)
{
	if (ending.size() > value.size())
		return (false);
	return (std::equal(ending.rbegin(), ending.rend(), value.rbegin()));
}

int			Response::setLocationBlock(std::string const &path)
{
	for (std::vector<Location_t>::iterator	it = _conf.get_locations().begin();it != _conf.get_locations().end(); it++)
		if (path.rfind(it->get_uri(), 0) == 0)
		{
			_loc = *it;
			return (true);
		}
	return (false);
}

void		Response::deleteMethod(std::string const &path)
{
	if (pathIsFile(path.substr(1)) != -1)
	{
		if (remove(path.substr(1).c_str()) == 0)
			this->handleHeader(path, 204);
		else
			retreiveBody(_error_pages[403], 403);
	}
	else
		retreiveBody(_error_pages[404], 404);
}

void		Response::getMethod(Request &request)
{
	if (endsWith(request.getPath(), ".php"))
	{
		std::string s("bin/php-cgi"); // Path to cgi binary
		std::string t_path(request.getPath());
		if (t_path[0] == '/')
			t_path.erase(0, 1);
		Cgi cgi((char *)s.c_str(), t_path, request);
		cgi.runCgi(request);
		if (cgi.getStatusCode() - 400 <= 100 && cgi.getStatusCode() - 400 >= 0)
			this->retreiveBody(_error_pages[cgi.getStatusCode()], cgi.getStatusCode());
		else
		{
			this->handleHeader(request.getPath(), cgi.getStatusCode());
			this->_body = cgi.getOutput();
		}
	}
	else
		retreiveBody(request.getPath(), 200);
}

void		Response::postMethod(Request &request)
{
	if (endsWith(request.getPath(), ".php"))
	{
		std::string s("bin/php-cgi"); // Path to cgi binary
		std::string t_path(request.getPath());
		if (t_path[0] == '/')
			t_path.erase(0, 1);
		Cgi cgi((char *)s.c_str(), t_path, request);
		cgi.runCgi(request);
		if (cgi.getStatusCode() - 400 <= 100 && cgi.getStatusCode() - 400 >= 0)
			this->retreiveBody(_error_pages[cgi.getStatusCode()], cgi.getStatusCode());
		else
		{
			this->handleHeader(request.getPath(), cgi.getStatusCode());
			this->_body = cgi.getOutput();
		}
	}
	else
		this->handleHeader(request.getPath(), 204);
}

void		Response::prepare(Request &request)
{
	_host = request.getHost().first;
	if (request.getHost().second.empty())
		_port = 80;
	else
		_port = std::stoi(request.getHost().second);
	if (_conf.is_allowed_get())
		_allowed_methods.push_back(GET);
	if (_conf.is_allowed_post())
		_allowed_methods.push_back(POST);
	if (_conf.is_allowed_delete())
		_allowed_methods.push_back(DELETE);
	if (setLocationBlock(request.getPath()))
	{
		_error_pages = _loc.get_error_page();
		_autoindex = _loc.get_autoindex();
		_index_list = _loc.get_index();
	}
	else
	{
		_error_pages = _conf.get_error_page();
		_autoindex = _conf.get_autoindex();
		_index_list = _conf.get_index();
	}
	if (request.getStatusCode() == 414)
		this->retreiveBody(_error_pages[414], 414);
	else if (request.getStatusCode() == 400)
		this->retreiveBody(_error_pages[400], 400);
	else if (std::find(_allowed_methods.begin(), _allowed_methods.end(), request.getMethod()) == _allowed_methods.end())
		this->retreiveBody(_error_pages[405], 405);
	else if (request.getStatusCode() == 413)
		this->retreiveBody(_error_pages[413], 413);
	else
	{
		if (request.getMethod() == GET)
			this->getMethod(request);
		else if (request.getMethod() == POST)
			this->postMethod(request);
		else if (request.getMethod() == DELETE)
			this->deleteMethod(request.getPath());
	}
}

std::string Response::parse(void)
{
	std::string hello = "HTTP/1.1 ";
	for (std::map<std::string, std::string>::iterator it = this->_header.begin(); it != this->_header.end(); it++)
		hello += it->first +  it->second + "\n";
	hello += "Content-Length: " + std::to_string(this->_body.size()) + "\n\n" + this->_body;
	return (hello);
}
