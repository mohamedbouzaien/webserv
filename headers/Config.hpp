#ifndef CONFIG_HPP
# define CONFIG_HPP

# include <iostream>
# include <vector>
# include <fstream>
# include <sstream>

# include "Server_t.hpp"
# include "Location_t.hpp"
# include "Context_t.hpp"

# include "default_conf.hpp"

# define CONF_ERR_HEAD "Error in conf: "

# define CONF_ERR_NO_SERV  "expected server{...} directive"
# define CONF_ERR_NO_BRKT  "expected closing bracket }"
# define CONF_ERR_UNEX_CBRKT  "unexpected closing bracket }"
# define CONF_ERR_UNEX_OBRKT  "unexpected opening bracket {"

# define CONF_ERR_LIST_VARG  "wrong argument value on listen directive"

# define CONF_ERR_NONAME  "no argument provided to server_name directive"

# define CONF_ERR_AUTO_IDX_VARG  "wrong arg value on autoindex directive"

# define CONF_ERR_MAXSZ_VARG  "wrong arg value on client_max_body_size directive"

# define CONF_ERR_ERPAGE_NAN     "argument value is not an error number in error page directive"
# define CONF_ERR_ERPAGE_WRG_ERR "error value must be between 300 and 599"
# define CONF_ERR_ERPAGE_499     "wrong error value: 499"

# define CONF_ERR_METH_VARG  "unrecognized method in allow_method directive"

# define CONF_ERR_CGI_WRG_TYPE "wrong cgi type in set_cgi directive (file types start by a dot: .xxx)"


# define CONF_ERR_SUBLOCATION  "sublocation is outside of its parent"

# define CONF_ERR_WRG_DIR  "unrecognized directive"

class Config
{
    private:
        std::vector<Server_t> _servers;
        std::string  _line;
        unsigned int _line_number;
        unsigned int _last_dir;

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

        // *** Common Directives ***
        void parse_location(args_t &args, Context_t &context, std::fstream &f, std::string &word);
        void parse_root(args_t &args, Context_t &context, std::fstream &file);
        void parse_index(args_t &args, Context_t &context, std::fstream &file);
        void parse_autoindex(args_t &args, Context_t &context, std::fstream &file);
        void parse_client_max_body_size(args_t &args, Context_t &context, std::fstream &file);
        void parse_error_page(args_t &args, Context_t &context, std::fstream &file);
        void parse_allow_method(args_t &args, Context_t &context, std::fstream &file);
        void parse_set_cgi(args_t &args, Context_t &context, std::fstream &file);
        void parse_upload_to(args_t &args, Context_t &context, std::fstream &file);

        // *** Location Directive(s)
        void parse_alias(args_t &args, Location_t &loc, std::fstream &file);
        //TODO IMPLEMENT THIS

        // *** Server Directives ***
        // listen directive
        bool is_valid_ip(std::string &ip);
        bool is_valid_port(std::string &port);
        void parse_listen(args_t &args, Server_t &server, std::fstream &f);

        // server_name directive
        void parse_names(args_t &args, Server_t &server, std::fstream &file);

        // server main loop
        void recursive_inherit(Location_t &loc);
        void check_server(std::fstream &file, std::string &word);
        void parse_server(std::fstream &file, std::string &word);

        // Error management + file closing
        void throw_close_narg(const char *s, std::fstream &f);
        void throw_close(const char *s, std::fstream &f);




    public:
        Config(const char * path);
        Config(const Config &copy);
        Config  &operator=(const Config &other);
        virtual ~Config();

        //Testing, outputs each server's attributes
        void print_servers();

        const std::vector<Server_t> & get_servers();

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
