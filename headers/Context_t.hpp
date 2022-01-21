#ifndef CONTEXT_T_HPP
# define CONTEXT_T_HPP

# include <vector>
# include <list>
# include <string>

# define IS_BOOL_SIZE 4

# define IS_ROOT  0
# define IS_INDEX  1
# define IS_AUTO_INDEX  2
# define IS_MAX_BODY_SIZE 3

class Location_t;

class Context_t
{
    protected:
        std::vector<bool> _is_set;

        std::string            _root;
        std::list<std::string> _index;
        bool                   _auto_index;
        unsigned long          _client_max_body_size;

    public:
        Context_t();
        Context_t(const Context_t &copy);
        Context_t  &operator=(const Context_t &other);
        virtual ~Context_t();

        void set_root(const std::string s);
        void add_index(const std::string s);
        void set_auto_index(const bool b);
        void set_client_max_body_size(const unsigned long n);
        const std::string &get_root();
        const std::list<std::string> &get_index();
        bool get_auto_index();
        unsigned long get_client_max_body_size();

        bool is_set_root() const;
        bool is_set_index() const;
        bool is_set_auto_index() const;
        bool is_set_client_max_body_size() const;

};

#endif
