#ifndef CONTEXT_T_HPP
# define CONTEXT_T_HPP

# include <vector>
# include <string>

# define IS_BOOL_SIZE 2

# define IS_ROOT  0
# define IS_ALIAS 1

class Location_t;

class Context_t
{
    protected:
        std::vector<bool> _is_set;

        std::string _root;
        std::string _alias;

    public:
        Context_t();
        Context_t(const Context_t &copy);
        Context_t  &operator=(const Context_t &other);
        virtual ~Context_t();

        void set_root(const std::string s);
        void set_alias(const std::string s);

        bool is_set_root() const;
        bool is_set_alias() const;

};

#endif
