#ifndef CONFIG_HPP
# define CONFIG_HPP

# include <vector>
# include <fstream>

# include "../headers/Server_t.hpp"

# define CONF_ERR_HEAD "Error in conf: "

# define CONF_ERR_NO_SERV CONF_ERR_HEAD "expected server{...} directive"
# define CONF_ERR_NO_BRKT CONF_ERR_HEAD "expected closing bracket }"
# define CONF_ERR_UNEX_BRKT CONF_ERR_HEAD "unexpected closing bracket }"
# define CONF_ERR_LIST_NARG CONF_ERR_HEAD "wrong number of args on listen directive"
# define CONF_ERR_LIST_VARG CONF_ERR_HEAD "wrong argument value on listen directive"

class Config
{
    private:
        std::vector<Server_t> _servers;

/* Private typedefs */
        typedef std::vector<std::string> args_t;

/* Private functions used for parsing */

        void check_comment(std::fstream &file, std::string &word) const;
        void next_word(std::fstream &file, std::string &word) const;

        void parse_directive(std::fstream &file, std::string &word, Server_t serv);

        // Related to listen
        bool is_valid_ip(std::string &ip);
        bool is_valid_port(std::string &port);
        void parse_listen(args_t &args, Server_t &server, std::fstream &f);

        // Related to server_name
        void parse_names(args_t &args, Server_t &server);

        void check_server(std::fstream &file, std::string &word) const;
        void parse_server(std::fstream &file, std::string &word);

        // Exception management + file closing
        void throw_close(const char *s, std::fstream &f);

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
