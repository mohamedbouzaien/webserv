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
        void add_location(const Location_t *loc);

// Getters ----------------------------------
        const std::set<std::string>   &get_names() const;
        const std::set<listen_pair_t> &get_listen() const;
        std::vector<Location_t> &get_locations();

		std::pair<bool, Location_t> get_best_location_block(std::string) const;
		unsigned long get_best_client_max_body_size(std::string) const;
		std::pair<std::string, std::string> get_best_cgi(std::string) const;
// checks --------------------
        bool has_name(const std::string &name) const;
        bool has_listen(const listen_pair_t &listen_pair) const;

        bool listen_empty() const;
        bool names_empty() const;


        //testing
        void print() const;
};


#endif
