#ifndef CONFIG_HPP
# define CONFIG_HPP

# include <vector>
# include <fstream>

# include "Server_t.hpp"
# include "Location_t.hpp"
# include "Context_t.hpp"

# define CONF_ERR_HEAD "Error in conf: "

# define CONF_ERR_NO_SERV CONF_ERR_HEAD "expected server{...} directive"
# define CONF_ERR_NO_BRKT CONF_ERR_HEAD "expected closing bracket }"
# define CONF_ERR_UNEX_CBRKT CONF_ERR_HEAD "unexpected closing bracket }"
# define CONF_ERR_UNEX_OBRKT CONF_ERR_HEAD "unexpected opening bracket {"

# define CONF_ERR_LIST_NARG CONF_ERR_HEAD "wrong number of args on listen directive"
# define CONF_ERR_LIST_VARG CONF_ERR_HEAD "wrong argument value on listen directive"

# define CONF_ERR_NONAME CONF_ERR_HEAD "no argument provided to server_name directive"

# define CONF_ERR_ROOT_NARG CONF_ERR_HEAD "root directive can only take one argument"

# define CONF_ERR_LOC_NARG CONF_ERR_HEAD "wrong number of args on location context"

# define CONF_ERR_IDX_NARG CONF_ERR_HEAD "wrong number of args on index directive"

# define CONF_ERR_AUTO_IDX_NARG CONF_ERR_HEAD "wrong number of args on auto_index directive"
# define CONF_ERR_AUTO_IDX_VARG CONF_ERR_HEAD "wrong arg value on auto_index directive"

# define CONF_ERR_WRG_DIR CONF_ERR_HEAD "unrecognized directive"

class Config
{
    private:
        std::vector<Server_t> _servers;

/* Private typedefs */
        typedef std::vector<std::string> args_t;

/* Private functions used for parsing */
        void check_comment(std::fstream &file, std::string &word) const;
        void next_word(std::fstream &file, std::string &word) const;
        int	 ft_atoi(const char *str) const;
        bool ft_isdigit(const char c) const;

/* General directive parser/dispatcher */
        void parse_directive(std::fstream &file, std::string &word, Context_t &context);
        void parse_server_directive(std::fstream &file, args_t &args, Server_t &serv);
        void parse_location_directive(std::fstream &file, args_t &args, Location_t &location);
        bool parse_common_directive(std::fstream &file, args_t &args, Context_t &context);

        // location context
        void parse_location(args_t &args, Server_t &serv, std::fstream &f, std::string &word);

        // *** Common Directives ***
        void parse_root(args_t &args, Context_t &context, std::fstream &file);
        void parse_index(args_t &args, Context_t &context, std::fstream &file);
        void parse_auto_index(args_t &args, Context_t &context, std::fstream &file);

        // *** Server Directives ***
        // listen directive
        bool is_valid_ip(std::string &ip);
        bool is_valid_port(std::string &port);
        void parse_listen(args_t &args, Server_t &server, std::fstream &f);

        // server_name directive
        void parse_names(args_t &args, Server_t &server, std::fstream &file);

        // server main loop
        void check_server(std::fstream &file, std::string &word) const;
        void parse_server(std::fstream &file, std::string &word);

        // Exception management + file closing
        void throw_close(const char *s, std::fstream &f);

        void print_servers();

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
