#ifndef CONTEXT_T_HPP
# define CONTEXT_T_HPP

# include <vector>
# include <list>
# include <string>
# include <map>

# define IS_BOOL_SIZE 5

# define IS_ROOT  0
# define IS_INDEX  1
# define IS_AUTO_INDEX  2
# define IS_MAX_BODY_SIZE 3
# define IS_ERROR_PAGES 4

class Location_t;

class Context_t
{
    protected:
        std::vector<bool> _is_set;

        std::string            _root;
        std::list<std::string> _index;
        bool                   _auto_index;
        unsigned long          _client_max_body_size;
        std::map<std::string, std::string> _error_pages;

    public:
        Context_t();
        Context_t(const Context_t &copy);
        Context_t  &operator=(const Context_t &other);
        virtual ~Context_t();

// Inheritance -----------------------------
        void inherit(Context_t &parent);

// Setters ---------------------------------
        void set_root(const std::string s);
        void add_index(const std::string s);
        void set_auto_index(const bool b);
        void set_client_max_body_size(const unsigned long n);
        void add_error_page(std::string & error, std::string & page);

// Getters ----------------------------------
        const std::string &get_root();
        const std::list<std::string> &get_index();
        bool get_auto_index();
        unsigned long get_client_max_body_size();
        std::map<std::string, std::string> get_error_page();
};

#endif
