#include "../headers/Config.hpp"
#include <iostream>

/**************************************\
|* Private Functions used for parsing *|
\**************************************/

/***************\
|* General pvs *|
\***************/

/* Checks if current word contains a comment,
 * extracts next word if the whole word is comment
 */
void Config::check_comment(std::fstream &file, std::string &word)
{
    size_t pos = word.find('#', 0);
    if (pos != std::string::npos)
    {
        word.resize(pos);
        _line.clear();
        if (word.empty())
            next_word(file, word);
    }
}

/* Get next word, and calls function above */
void Config::next_word(std::fstream &file, std::string &word)
{
    while ((!_line.size() || _line.find_first_not_of(" \t\n\r\f\v") == std::string::npos))
    {
        getline(file, _line);
        ++_line_number;
        if (!file.good())
            return;
    }
    size_t start = _line.find_first_not_of(" \t\n\r\f\v");
    size_t end = _line.find_first_of(" \t\n\r\f\v", start);
    word = _line.substr(start, end - start);
    _line.erase(0, end);
    check_comment(file, word);
}

bool Config::ft_isdigit(const char c) const
{
    if (c >= '0' && c <= '9')
        return true;
    return false;
}

long Config::ft_atoi(const char *str) const
{
	unsigned long	nb;
	long			sgn;
	long			i;

	i = 0;
	nb = 0;
	sgn = 1;
	while (str[i] && (str[i] == '\t' || str[i] == '\n' || str[i] == '\r'
		|| str[i] == '\f' || str[i] == ' ' || str[i] == '\v'))
		i++;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			sgn *= -1;
		i++;
	}
	while (str[i] && ft_isdigit(str[i]))
	{
		nb = nb * 10 + (str[i] - '0');
		i++;
	}
	return ((long)((long)(nb) * sgn));
}

/**********************\
|* location main loop *|
\**********************/

void Config::parse_location(args_t &args, Server_t &serv, std::fstream &file, std::string &word){
    if (args.size() != 2)
        throw_close(CONF_ERR_LOC_NARG, file);
    Location_t loc(args[1]);

std::cout << "location {";
    while (word[0] != '}' && file.good())
        parse_directive(file, word, loc);
std::cout << "}(loc)" << std::endl;

    if (word == "}")
        next_word(file, word);
    else
        word.erase(0, 1);
    /*
    if (loc.names_empty()) // add empty name if no name has been provided
        loc.add_name("");
    */
    //loc.print();
    serv.add_location(loc);
}


/******************\
|* root directive *|
\******************/

void Config::parse_root(args_t &args, Context_t &context, std::fstream &file){
    if (args.size() != 2)
        throw_close(CONF_ERR_ROOT_NARG, file);
    context.set_root(args[1]);
}

/*******************\
|* index directive *|
\*******************/

void Config::parse_index(args_t &args, Context_t &context, std::fstream &file){
    if (args.size() <= 1)
        throw_close(CONF_ERR_IDX_NARG, file);
    for (args_t::iterator it = ++args.begin(); it != args.end(); ++it)
        context.add_index(*it);
}

/************************\
|* auto_index directive *|
\************************/

void Config::parse_auto_index(args_t &args, Context_t &context, std::fstream &file){
    if (args.size() != 2)
        throw_close(CONF_ERR_AUTO_IDX_NARG, file);
    if (args[1] == "on")
        context.set_auto_index(true);
    else if (args[1] == "off")
        context.set_auto_index(false);
    else
        throw_close(CONF_ERR_AUTO_IDX_VARG, file);
}


/**********************************\
|* client_max_body_size directive *|
\**********************************/

void Config::parse_client_max_body_size(args_t &args, Context_t &context, std::fstream &file){
    if (args.size() != 2)
        throw_close(CONF_ERR_MAXSZ_NARG, file);
    for (std::string::iterator it = args[1].begin(); it != args[1].end(); ++it)
        if (!ft_isdigit(*it))
            throw_close(CONF_ERR_AUTO_IDX_VARG, file);
    context.set_client_max_body_size(ft_atoi(args[1].c_str()));
}


/************************\
|* error_page directive *|
\************************/

void Config::parse_error_page(args_t &args, Context_t &context, std::fstream &file){
    if (args.size() < 3)
        throw_close(CONF_ERR_ERPAGE_NARG, file);
    for (args_t::iterator it = ++args.begin(); it != --args.end(); ++it)
    {
        for (std::string::iterator str_it = it->begin(); str_it != it->end(); ++str_it)
            if (!ft_isdigit(*str_it))
                throw_close(CONF_ERR_ERPAGE_VARG, file);
        context.add_error_page(*it, args.back());
    }
}

/**************************\
|* allow_method directive *|
\**************************/

void Config::parse_allow_method(args_t &args, Context_t &context, std::fstream &file){
    if (args.size() < 2)
        throw_close(CONF_ERR_METH_NARG, file);
    for (args_t::iterator it = ++args.begin(); it != args.end(); ++it)
    {
        if (*it == "GET")
            context.allow_get();
        else if (*it == "POST")
            context.allow_post();
        else if (*it == "DELETE")
            context.allow_delete();
        else
            throw_close(CONF_ERR_METH_VARG, file);
    }
}

/*************************\
|* server_name directive *|
\*************************/

void Config::parse_names(args_t &args, Server_t &server, std::fstream &file){
    if (args.size() <= 1)
        throw_close(CONF_ERR_NONAME, file);
    for (args_t::iterator it = ++args.begin(); it != args.end(); ++it)
        server.add_name(*it);
}


/********************\
|* listen directive *|
\********************/

bool Config::is_valid_ip(std::string &ip){
    if (ip == "*")
        return true;
    if (ip.find('.') == std::string::npos)
        return false;
    for (std::string::iterator it = ip.begin(); it != ip.end(); ++it)
        if (!ft_isdigit(*it) && *it != '.')
            return false;
    return true;
}

bool Config::is_valid_port(std::string &port){
    for (std::string::iterator it = port.begin(); it != port.end(); ++it)
        if (!ft_isdigit(*it))
            return false;
    return true;
}

void Config::parse_listen(args_t &args, Server_t &server, std::fstream &file){
    std::string  ip = "*";
    in_port_t   port = 80;

    if (args.size() != 2)
        throw_close(CONF_ERR_LIST_NARG, file);
    if (args[1].find(':') != std::string::npos)
    {
        size_t pos = args[1].find(':');
        std::string port_s(args[1], pos + 1, args[1].size());
        args[1].resize(pos);
        if (!is_valid_ip(args[1]) || !is_valid_port(port_s))
            throw_close(CONF_ERR_LIST_VARG, file);
        ip = args[1];
        port = ft_atoi(port_s.c_str());
    }
    else
    {
        if (is_valid_ip(args[1]))
            ip = args[1];
        else if (is_valid_port(args[1]))
            port = ft_atoi(args[1].c_str());
        else
            throw_close(CONF_ERR_LIST_VARG, file);
    }
    server.add_listen(std::make_pair(ip, port));
}

/**********************\
|* directive dispatch *|
\**********************/

void Config::parse_location_directive(std::fstream &file, args_t &args, Location_t &location)
{
    (void)location;
    (void)file;
    (void)args;
    throw_close(CONF_ERR_WRG_DIR, file);
}

void Config::parse_server_directive(std::fstream &file, args_t &args, Server_t &serv)
{
    if (args[0] == "listen")
        parse_listen(args, serv, file);
    else if (args[0] == "server_name")
        parse_names(args, serv, file);
    else
        throw_close(CONF_ERR_WRG_DIR, file);
}

bool Config::parse_common_directive(std::fstream &file, args_t &args, Context_t &context)
{
    if (args[0] == "root")
        parse_root(args, context, file);
    else if (args[0] == "index")
        parse_index(args, context, file);
    else if (args[0] == "auto_index")
        parse_auto_index(args, context, file);
    else if (args[0] == "client_max_body_size")
        parse_client_max_body_size(args, context, file);
    else if (args[0] == "error_page")
        parse_error_page(args, context, file);
    else if (args[0] == "allow_method")
        parse_allow_method(args, context, file);
    else
        return false;
    return true;
}

/* Parse one directive and give it it's argument */
void Config::parse_directive(std::fstream &file, std::string &word, Context_t &context)
{
    args_t args;
    size_t pos = word.find(';', 0);
    while (pos == std::string::npos && file.good())
    {
        if (word.find('}', 0) != std::string::npos)
            throw_close(CONF_ERR_UNEX_CBRKT, file);
       // std::cout << "param: " << word << " | ";
        args.push_back(word);
        next_word(file, word);
        pos = word.find(';', 0);
        if (pos == std::string::npos)
            pos = word.find('{', 0);
    }
    if (pos == word.find('{', 0) && args[0] != "location")
        throw_close(CONF_ERR_UNEX_OBRKT, file);
    if (word == ";" || word == "{")
        next_word(file, word);
    else
    {
        args.push_back(std::string(word, 0, pos));
        word.erase(0, ++pos);
        if (word.empty())
            next_word(file, word);
    }
    if (parse_common_directive(file, args, context))
        return;
    if (dynamic_cast<Server_t*>(&context))
        if (args[0] == "location")
            parse_location(args, *dynamic_cast<Server_t*>(&context), file, word);
        else
            parse_server_directive(file, args, *dynamic_cast<Server_t*>(&context));
    else if (dynamic_cast<Location_t*>(&context))
        parse_location_directive(file, args, *dynamic_cast<Location_t*>(&context));
    else
        throw_close(CONF_ERR_WRG_DIR, file);
}



/**************\
|* server pvs *|
\**************/

/* Checks server directive call validity
 * (when not in server{}, it's the only callable directive)
 */
void Config::check_server(std::fstream &file, std::string &word)
{
    if (word == "server")
    {
        next_word(file, word);
        if (word[0] == '{')
            word.erase(1);
        else
            throw_close(CONF_ERR_NO_SERV, file);
    }
    else if (!word.compare(0, 7, "server{"))
        word.erase(0, 7);
    else
        throw_close(CONF_ERR_NO_SERV, file);
    if (word.empty())
        next_word(file, word);
}

/* The loop for each server{} scope */
void Config::parse_server(std::fstream &file, std::string &word)
{
    check_server(file, word);
    Server_t serv;

std::cout << "server {";
    while (word[0] != '}' && file.good())
        parse_directive(file, word, serv);
std::cout << "}" << std::endl;

    if (!file.good())
        throw_close(CONF_ERR_NO_BRKT, file);

    serv.init_not_set(); // to set varaiables that haven't been set already
    for (std::vector<Location_t>::iterator it = serv.get_locations().begin(); it != serv.get_locations().end(); ++it)
    {
        it->inherit(serv);
        //TODO Add it->init_not_set() ? // Not needed because inheritance sets it all ?
    }

    if (serv.names_empty()) // add empty name if no name has been provided
        serv.add_name("");
    _servers.push_back(serv);
}

// Print all servers ----------- testing
void Config::print_servers()
{
    std::cout << "All servers:\n";
    for (std::vector<Server_t>::iterator it = _servers.begin(); it != _servers.end(); ++it)
        it->print();
}

/********************\
|* Public Functions *|
\********************/

/* Constructor */
Config::Config(const char * path): _servers(std::vector<Server_t>()),
                                   _line(std::string()),
                                   _line_number(0)
{
    std::fstream file(path, std::ios_base::in);
    std::string word;
    if (file.fail())
        throw (FileOpenException());
    while (file.good())
    {
        next_word(file, word);
        if (!word.empty())
            parse_server(file, word);
    }
    file.close();

    print_servers();
}

Config::Config(const Config &copy)
{
	//TODO ???
	(void)copy;
}


Config::~Config()
{}

Config	&Config::operator=(const Config &other)
{
	//TODO ???
    (void)other;
	return *this;
}


void Config::throw_close(const char *message, std::fstream &f){
    f.close();
    std::stringstream s;
    s << CONF_ERR_HEAD << "at line " << _line_number << ": " << message <<std::endl;
    std::cerr << s.str();
    throw(ParseErrException());
}

const char* Config::FileOpenException::what() const throw()
{
	return ("Configuration file couldn't be open");
}

const char* Config::ParseErrException::what() const throw()
{
	return ("Error reading configuration file");
}
