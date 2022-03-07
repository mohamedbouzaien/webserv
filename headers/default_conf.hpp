#ifndef DEFAULT_CONF_HPP
# define DEFAULT_CONF_HPP

// Server directives
# define DEFAULT_SERV_NAME ""

# define DEFAULT_LISTEN std::make_pair("*",80)

// Location directives
# define DEFAULT_ALIAS ""

// Common directives
# define DEFAULT_ROOT "/html/"

# define DEFAULT_AUTOINDEX false

# define DEFAULT_MAX_BODY_SIZE 1000000

# define DEFAULT_CGI_PATH "./bin/php-cgi"
# define DEFAULT_CGI_TYPE ".php"

# define DEFAULT_UPLOAD_TO ""

# define DEFAULT_REDIR ""

# define DEFAULT_INDEX "index.html"

#endif
