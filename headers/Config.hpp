#ifndef CONFIG_HPP
# define CONFIG_HPP

# include <vector>
# include <fstream>
# include <sstream>

# include "Server_t.hpp"
# include "Location_t.hpp"
# include "Context_t.hpp"

# define CONF_ERR_HEAD "Error in conf: "

# define CONF_ERR_NO_SERV  "expected server{...} directive"
# define CONF_ERR_NO_BRKT  "expected closing bracket }"
# define CONF_ERR_UNEX_CBRKT  "unexpected closing bracket }"
# define CONF_ERR_UNEX_OBRKT  "unexpected opening bracket {"

# define CONF_ERR_LIST_NARG  "wrong number of args on listen directive"
# define CONF_ERR_LIST_VARG  "wrong argument value on listen directive"

# define CONF_ERR_NONAME  "no argument provided to server_name directive"

# define CONF_ERR_ROOT_NARG  "root directive can only take one argument"

# define CONF_ERR_LOC_NARG  "wrong number of args on location context"

# define CONF_ERR_IDX_NARG  "wrong number of args on index directive"

# define CONF_ERR_AUTO_IDX_NARG  "wrong number of args on auto_index directive"
# define CONF_ERR_AUTO_IDX_VARG  "wrong arg value on auto_index directive"

# define CONF_ERR_MAXSZ_NARG  "wrong number of args on client_max_body_size directive"
# define CONF_ERR_MAXSZ_VARG  "wrong arg value on client_max_body_size directive"

# define CONF_ERR_ERPAGE_NARG  "wrong number of args on error_page directive"
# define CONF_ERR_ERPAGE_VARG  "wrong value of args on error_page directive"

# define CONF_ERR_WRG_DIR  "unrecognized directive"

class Config
{
    private:
        std::vector<Server_t> _servers;
        std::string _line;
        unsigned int _line_number;

/* Private typedefs */
        typedef std::vector<std::string> args_t;

/* Private functions used for parsing */
        void check_comment(std::fstream &file, std::string &word);
        void next_word(std::fstream &file, std::string &word);
        long ft_atoi(const char *str) const;
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
        void parse_client_max_body_size(args_t &args, Context_t &context, std::fstream &file);
        void parse_error_page(args_t &args, Context_t &context, std::fstream &file);

        // *** Server Directives ***
        // listen directive
        bool is_valid_ip(std::string &ip);
        bool is_valid_port(std::string &port);
        void parse_listen(args_t &args, Server_t &server, std::fstream &f);

        // server_name directive
        void parse_names(args_t &args, Server_t &server, std::fstream &file);

        // server main loop
        void check_server(std::fstream &file, std::string &word);
        void parse_server(std::fstream &file, std::string &word);

        // Exception management + file closing
        void throw_close(const char *s, std::fstream &f);

        std::vector<Server_t> get_servers();

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
            public:
                virtual const char* what() const throw();
        };

};
#endif
