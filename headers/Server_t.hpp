#ifndef SERVER_T_HPP
# define SERVER_T_HPP

# include <netinet/in.h>
# include <string>
# include <set>
# include <map>

class Server_t
{
    private:
        typedef std::pair<std::string, in_port_t> listen_pair_t;

        std::set<std::string> _names;
        std::set<listen_pair_t> _listen;

    public:
        Server_t();
        Server_t(const Server_t &copy);
        Server_t  &operator=(const Server_t &other);
        virtual ~Server_t();

        void add_name(const std::string name);
        void add_listen(const std::pair<std::string, in_port_t> listen_pair);

        bool has_name(const std::string name) const;
        bool has_listen(const std::pair<std::string, in_port_t> listen_pair) const;

        bool listen_empty();
};


#endif
