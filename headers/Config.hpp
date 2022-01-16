#ifndef CONFIG_HPP
# define CONFIG_HPP

# include <vector>
# include <fstream>

# include "../headers/Server_t.hpp"

# define CONF_ERR_HEAD "Error in conf: "

# define CONF_ERR_NO_SERV CONF_ERR_HEAD "expected server{...} directive"
# define CONF_ERR_NO_BRKT CONF_ERR_HEAD "expected closing bracket }"

class Config
{
    private:
        std::vector<Server_t> _servers;

/* Private functions used for parsing */
        void check_comment(std::fstream &file, std::string &word) const;
        void next_word(std::fstream &file, std::string &word) const;

        void check_server(std::fstream &file, std::string &word) const;
        void parse_server(std::fstream &file, std::string &word);

    public:
        Config(const char * path);
        Config(const Config &copy);
        Config  &operator=(const Config &other);
        virtual ~Config();

       	class FileOpenException : public std::exception
        {
            public:
                virtual const char* what() const throw();
        };

        class ParseErrException : public std::exception
        {
            private:
                const char *_s;
            public:
                ParseErrException(const char *s);
                virtual const char* what() const throw();
        };

        std::vector<Server_t> get_servers();
};
#endif
