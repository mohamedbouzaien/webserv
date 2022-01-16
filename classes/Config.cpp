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



/**************\
|* listen pvs *|
\**************/



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
            throw (ExpectedServerException());
        }
    }
    else if (!word.compare(0, 7, "server{"))
        word.erase(0, 7);
    else
    {
        file.close();
        throw (ExpectedServerException());
    }
    if (word.empty())
        next_word(file, word);
}

void Config::parse_server(std::fstream &file, std::string &word)
{
    check_server(file, word);
    Server_t serv;
    std::cout << word << " | ";
    while (word[0] != '}')
    {
        next_word(file, word);
        std::cout << word << " | ";
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
        std::cout << word << " | ";
        parse_server(file, word);
    }
    file.close();
}

Config::Config(Config &copy)
{
	(void)copy;
}


Config::~Config()
{}

Config	&Config::operator=(Config &other)
{
	//to do
	return (other);
}

const char* Config::FileOpenException::what() const throw()
{
	return ("Configuration file couldn't be open");
}

const char* Config::ExpectedServerException ::what() const throw()
{
	return ("Configuration file parsing error: expected server{...} directive");
}
