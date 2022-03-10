#include "../headers/Context_t.hpp"

Context_t::Context_t():
    _is_set(std::vector<bool>(IS_BOOL_SIZE, false)),
    _root(DEFAULT_ROOT),
    _index(std::list<std::string>()),
    _autoindex(DEFAULT_AUTOINDEX),
    _client_max_body_size(DEFAULT_MAX_BODY_SIZE ),
    _error_pages(std::map<int, std::string>()),
    _allow_method(std::vector<bool>(IS_BOOL_SIZE, false)),
    _cgi(std::make_pair(DEFAULT_CGI_PATH, DEFAULT_CGI_TYPE)),
    _upload_to(DEFAULT_UPLOAD_TO),
    _redir(DEFAULT_REDIR)
{
}

Context_t::Context_t(const Context_t &copy):
    _is_set(copy._is_set),
    _root(copy._root),
    _index(copy._index),
    _autoindex(copy._autoindex),
    _client_max_body_size(copy._client_max_body_size),
    _error_pages(copy._error_pages),
    _allow_method(copy._allow_method),
    _cgi(copy._cgi),
    _upload_to(copy._upload_to),
    _redir(copy._redir)
{
}

Context_t::~Context_t()
{}

Context_t	&Context_t::operator=(const Context_t &other)
{
    _is_set                 = other._is_set;
    _root                   = other._root;
    _index                  = other._index;
    _autoindex              = other._autoindex;
    _client_max_body_size   = other._client_max_body_size;
    _error_pages            = other._error_pages;
    _allow_method           = other._allow_method;
    _cgi                    = other._cgi;
    _upload_to              = other._upload_to;
    _redir                  = other._redir;
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
        _autoindex = parent._autoindex;
    if (!_is_set[IS_MAX_BODY_SIZE])
        _client_max_body_size = parent._client_max_body_size;
    if (!_is_set[IS_ERROR_PAGES])
        _error_pages = parent._error_pages;
    if (!_is_set[IS_ALLOW_METHOD])
         _allow_method = parent._allow_method;
    if (!_is_set[IS_CGI])
         _cgi = parent._cgi;
    for (std::map<int, std::string>::iterator it = parent._error_pages.begin(); it != parent._error_pages.end(); it++)
        if (_error_pages.find(it->first) == _error_pages.end())
            _error_pages[it->first] = it->second;
    if (!_is_set[IS_UPLOAD_TO])
        _upload_to = parent._upload_to;
    if (!_is_set[IS_REDIR])
        _redir = parent._redir;
}

// Unset directives default values ---------
void Context_t::init_not_set()
{
    if (!_is_set[IS_INDEX])
        add_index("index.html");
    if (!_is_set[IS_ALLOW_METHOD])
    {
        allow_get();
        allow_post();
        allow_delete();
        allow_put();
    }
	_error_pages.insert(std::make_pair<int, std::string>(301, "/error_page/300_error/301.html"));
	_error_pages.insert(std::make_pair<int, std::string>(400, "/error_page/400_error/400.html"));
	_error_pages.insert(std::make_pair<int, std::string>(403, "/error_page/400_error/403.html"));
	_error_pages.insert(std::make_pair<int, std::string>(404, "/error_page/400_error/404.html"));
	_error_pages.insert(std::make_pair<int, std::string>(405, "/error_page/400_error/405.html"));
	_error_pages.insert(std::make_pair<int, std::string>(413, "/error_page/400_error/413.html"));
    _error_pages.insert(std::make_pair<int, std::string>(414, "/error_page/400_error/414.html"));
    _error_pages.insert(std::make_pair<int, std::string>(431, "/error_page/400_error/431.html"));
	_error_pages.insert(std::make_pair<int, std::string>(500, "/error_page/500_error/500.html"));
	_error_pages.insert(std::make_pair<int, std::string>(505, "/error_page/500_error/505.html"));
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

void Context_t::set_autoindex(const bool b) {
    _is_set[IS_AUTO_INDEX] = true;
    _autoindex = b;
}

void Context_t::set_client_max_body_size(const unsigned long n) {
    _is_set[IS_MAX_BODY_SIZE] = true;
    _client_max_body_size = n;
}

void Context_t::add_error_page(std::string & error, std::string & page) {
    _is_set[IS_ERROR_PAGES] = true;
    _error_pages[std::stoi(error)] = page;
}

void Context_t::allow_get() {
    _is_set[IS_ALLOW_METHOD] = true;
    _allow_method[METH_GET] = true;
}

void Context_t::allow_post() {
    _is_set[IS_ALLOW_METHOD] = true;
    _allow_method[METH_POST] = true;
}

void Context_t::allow_delete() {
    _is_set[IS_ALLOW_METHOD] = true;
    _allow_method[METH_DELETE] = true;
}

void Context_t::allow_put() {
    _is_set[IS_ALLOW_METHOD] = true;
    _allow_method[METH_PUT] = true;
}

void  Context_t::set_cgi(std::string &path, std::string &type) {
    _is_set[IS_CGI] = true;
    _cgi.first = path;
    _cgi.second = type;
}

void Context_t::set_upload_to(const std::string s)
{
    _is_set[IS_UPLOAD_TO] = true;
    _upload_to = s;
}
void Context_t::set_redir(const std::string s)
{
    _is_set[IS_REDIR] = true;
    _redir = s;
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

bool Context_t::get_autoindex()
{
    return _autoindex;
}

unsigned long Context_t::get_client_max_body_size() const
{
    return _client_max_body_size;
}

std::map<int, std::string> Context_t::get_error_page()
{
    return _error_pages;
}

bool Context_t::is_allowed_get()    const
{
    return _allow_method[METH_GET];
}

bool Context_t::is_allowed_post()   const
{
    return _allow_method[METH_POST];
}

bool Context_t::is_allowed_delete() const
{
    return _allow_method[METH_DELETE];
}

bool Context_t::is_allowed_put()    const
{
    return _allow_method[METH_PUT];
}

const std::string &Context_t::get_cgi_path() const
{
    return _cgi.first;
}

const std::string &Context_t::get_cgi_type() const
{
    return _cgi.second;
}

const std::string &Context_t::get_upload_to() const
{
    return _upload_to;
}

const std::string &Context_t::get_redir() const
{
    return _redir;
}
