#include "../headers/Context_t.hpp"

Context_t::Context_t():
    _is_set(std::vector<bool>(IS_BOOL_SIZE, false)),
    _root("html"),
    _index(std::list<std::string>()),
    _auto_index(false),
    _client_max_body_size(1000000),
    _error_pages(std::map<std::string, std::string>())
{
}

Context_t::Context_t(const Context_t &copy):
    _is_set(copy._is_set),
    _root(copy._root),
    _index(copy._index),
    _auto_index(copy._auto_index),
    _client_max_body_size(copy._client_max_body_size),
    _error_pages(copy._error_pages)
{
}

Context_t::~Context_t()
{}

Context_t	&Context_t::operator=(const Context_t &other)
{
    _is_set                 = other._is_set;
    _root                   = other._root;
    _index                  = other._index;
    _auto_index             = other._auto_index;
    _client_max_body_size   = other._client_max_body_size;
    _error_pages            = other._error_pages;
	return *this;
}

// Inheritance -----------------------------

#include <iostream>
void Context_t::inherit(Context_t &parent) {
    if (!_is_set[IS_ROOT])
        _root = parent._root;
    if (!_is_set[IS_INDEX])
        _index = parent._index;
    if (!_is_set[IS_AUTO_INDEX])
        _auto_index = parent._auto_index;
    if (!_is_set[IS_MAX_BODY_SIZE])
        _client_max_body_size = parent._client_max_body_size;
    if (!_is_set[IS_ERROR_PAGES])
        _error_pages = parent._error_pages;
}

// Setters ---------------------------------
void Context_t::set_root(const std::string s) {
    _is_set[IS_ROOT] = true;
    _root = s;
}

void Context_t::add_index(const std::string s) {
    _is_set[IS_INDEX] = true;
    _index.push_back(s);
}

void Context_t::set_auto_index(const bool b) {
    _is_set[IS_AUTO_INDEX] = true;
    _auto_index = b;
}

void Context_t::set_client_max_body_size(const unsigned long n) {
    _is_set[IS_MAX_BODY_SIZE] = true;
    _client_max_body_size = n;
}

void Context_t::add_error_page(std::string & error, std::string & page) {
    _is_set[IS_ERROR_PAGES] = true;
    _error_pages.insert(std::make_pair(error, page));
}

// Getters ----------------------------------
const std::string &Context_t::get_root()
{
    return _root;
}

const std::list<std::string> &Context_t::get_index()
{
    return _index;
}

bool Context_t::get_auto_index()
{
    return _auto_index;
}

unsigned long Context_t::get_client_max_body_size()
{
    return _client_max_body_size;
}

std::map<std::string, std::string> Context_t::get_error_page()
{
    return _error_pages;
}
