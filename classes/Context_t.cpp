#include "../headers/Context_t.hpp"

Context_t::Context_t():
    _is_set(std::vector<bool>(IS_BOOL_SIZE, false)),
    _root("html"),
    _alias("")
{
}

Context_t::Context_t(const Context_t &copy):
    _is_set(copy._is_set),
    _root(copy._root),
    _alias(copy._alias)
{
}

Context_t::~Context_t()
{}

Context_t	&Context_t::operator=(const Context_t &other)
{
    _is_set = other._is_set;
    _root   = other._root;
    _alias  = other._alias;
	return *this;
}

void Context_t::set_root(const std::string s) {
    _is_set[IS_ROOT] = true;
    _root = s;
}

void Context_t::set_alias(const std::string s) {
    _is_set[IS_ALIAS] = true;
    _alias = s;
}

bool Context_t::is_set_root() const {
    return _is_set[IS_ROOT];
}
bool Context_t::is_set_alias() const {
    return _is_set[IS_ALIAS];
}
