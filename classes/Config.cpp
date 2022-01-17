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
void Config::check_comment(std::fstream &file, std::string &word) const
{
    size_t pos = word.find('#', 0);
    if (pos != std::string::npos)
    {
        word.resize(pos);
        file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        if (word.empty())
        {
            file >> word;
            check_comment(file, word);
        }
    }
}

/* Get next word, and calls function above */
void Config::next_word(std::fstream &file, std::string &word) const
{
    file >> word;
    check_comment(file, word);
}

/*******************\
|* server_name pvs *|
\*******************/

void Config::parse_names(args_t &args, Server_t &server){
    (void)args;
    (void)server;
}


/**************\
|* listen pvs *|
\**************/

bool Config::is_valid_ip(std::string &ip){
    if (ip == "*")
        return true;
    for (std::string::iterator it = ip.begin(); it != ip.end(); ++it)
        if (!(*it >= '0' && *it <= '9') || *it != '.')
            return false;
    return true;
}

void Config::parse_listen(args_t &args, Server_t &server){
    std::string ip = "*";
    in_port_t   port = 80;

    if (args.size() != 2)
        throw(CONF_ERR_LIST_NARG); //Il faudrait peut etre close le fd ici mais CA ME LES BRISE on va sans doute le passer en variable d'objet hein
    if (args[1].find(':'))
    {

    }
    else
    {
        if (is_valid_ip(args[1]))
            ip = args[1];
        else if (is_valid_port(args[1]))
            (void)port; // la faudra mettre un atoi une connerie du genre mais flm vrmt
        else
            throw (CONF_ERR_LIST_VARG) // Com 2 au dessus bis.
    }
    server.add_listen(std::make_pair(args[1], port));
}

/*****************\
|* directive pvs *|
\*****************/

/* Parse one directive and give it it's argument */
void Config::parse_directive(std::fstream &file, std::string &word, Server_t serv)
{
    args_t args;
    size_t pos = word.find(';', 0);
    while (pos == std::string::npos && file.good())
    {
        if (word.find('}', 0) != std::string::npos)
        {
            file.close();
            throw ParseErrException(CONF_ERR_UNEX_BRKT);
        }
        //std::cout << "param: " << word << " | ";
        args.push_back(word);
        next_word(file, word);
        pos = word.find(';', 0);
    }
    if (word == ";")
        next_word(file, word);
    else
    {
        args.push_back(std::string(word, 0, pos));
        word.erase(0, ++pos);
        if (word.empty())
            next_word(file, word);
    }

    if (args[0] == "listen)
        listen(args, server);
    // TODO Dispatcher les directives vers la fonctions qui leur correspond
    std::cout << std::endl << "  directive: ";
    for (std::vector<std::string>::iterator it = args.begin(); it != args.end(); it++)
        std::cout << *it << " | ";
    std::cout << std::endl;
}



/**************\
|* server pvs *|
\**************/

/* Checks server directive call validity
 * (when not in server{}, it's the only callable directive)
 */
void Config::check_server(std::fstream &file, std::string &word) const
{
    if (word == "server")
    {
        next_word(file, word);
        if (word[0] == '{')
            word.erase(1);
        else
        {
            file.close();
            throw (ParseErrException(CONF_ERR_NO_SERV));
        }
    }
    else if (!word.compare(0, 7, "server{"))
        word.erase(0, 7);
    else
    {
        file.close();
        throw (ParseErrException(CONF_ERR_NO_SERV));
    }
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
    {
        file.close();
        throw (ParseErrException(CONF_ERR_NO_BRKT));
    }
    _servers.push_back(serv);
}

/********************\
|* Public Functions *|
\********************/

/* Constructor */
Config::Config(const char * path): _servers(std::vector<Server_t>())
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
}

Config::Config(const Config &copy)
{
	(void)copy;
}


Config::~Config()
{}

Config	&Config::operator=(const Config &other)
{
	//to do
    (void)other;
	return *this;
}

const char* Config::FileOpenException::what() const throw()
{
	return ("Configuration file couldn't be open");
}

Config::ParseErrException::ParseErrException(const char *s): _s(s) {}
const char* Config::ParseErrException::what() const throw()
{
	return (_s);
}
