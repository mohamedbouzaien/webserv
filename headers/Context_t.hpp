#ifndef CONTEXT_T_HPP
# define CONTEXT_T_HPP

# include <vector>
# include <list>
# include <string>
# include <map>

// is_set bool vector correspondance
# define IS_BOOL_SIZE     6
# define IS_ROOT          0
# define IS_INDEX         1
# define IS_AUTO_INDEX    2
# define IS_MAX_BODY_SIZE 3
# define IS_ERROR_PAGES   4
# define IS_ALLOW_METHOD  5

// allow_method bool vector correspondance
# define METH_SIZE   3
# define METH_GET    0
# define METH_POST   1
# define METH_DELETE 2

class Location_t;

class Context_t
{
    protected:
        std::vector<bool> _is_set;

        std::string            _root;
        std::list<std::string> _index;
        bool                   _autoindex;
        unsigned long          _client_max_body_size;
        std::map<std::string, std::string> _error_pages;
        std::vector<bool>      _allow_method;

    public:
        Context_t();
        Context_t(const Context_t &copy);
        Context_t  &operator=(const Context_t &other);
        virtual ~Context_t();

// Inheritance -----------------------------
        void inherit(Context_t &parent);

// Unset directives default values ---------
        void init_not_set();

// Setters ---------------------------------
        void set_root(const std::string s);
        void add_index(const std::string s);
        void set_autoindex(const bool b);
        void set_client_max_body_size(const unsigned long n);
        void add_error_page(std::string & error, std::string & page);
        void allow_get();
        void allow_post();
        void allow_delete();
        virtual void add_location(const Location_t*) = 0;

// Getters ----------------------------------
        const std::string &get_root();
        const std::list<std::string> &get_index();
        bool get_autoindex();
        unsigned long get_client_max_body_size();
        std::map<std::string, std::string> get_error_page();
        bool is_allowed_get();
        bool is_allowed_post();
        bool is_allowed_delete();
};

#endif
