#ifndef SERVER_T_HPP
# define SERVER_T_HPP

# include <netinet/in.h>
# include <string>
# include <set>
# include <vector>

# include "Context_t.hpp"
# include "Location_t.hpp"

class Server_t : public Context_t
{
    public:
        typedef std::pair<std::string, in_port_t> listen_pair_t;

    private:

        std::set<std::string>   _names;
        std::set<listen_pair_t> _listen;
        std::vector<Location_t> _locations;

    public:

        Server_t();
        Server_t(const Server_t &copy);
        Server_t  &operator=(const Server_t &other);
        virtual ~Server_t();

// Setters ---------------------------------
        void add_name(const std::string &name);
        void add_listen(const listen_pair_t &listen_pair);
        void add_location(const Location_t &loc);

// Getters ----------------------------------
        std::set<std::string>   &get_names();
        std::set<listen_pair_t> &get_listen();
        std::vector<Location_t> &get_locations();

// checks --------------------
        bool has_name(const std::string name) const;
        bool has_listen(const listen_pair_t listen_pair) const;

        bool listen_empty() const;
        bool names_empty() const;


        //testing
        void print();
};


#endif
