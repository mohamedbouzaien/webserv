#ifndef SERVER_T_HPP
# define SERVER_T_HPP

# include <netinet/in.h>
# include <string>
# include <set>

class Server_t
{
    private:
        std::set<std::string> _names;
        std::set<in_port_t> _ports;

    public:
        Server_t();
        Server_t(const Server_t &copy);
        Server_t  &operator=(const Server_t &other);
        virtual ~Server_t();

        void add_name(const std::string name);
        void add_port(const in_port_t port);

        bool has_name(const std::string name) const;
        bool has_port(const in_port_t port) const;
};


#endif
