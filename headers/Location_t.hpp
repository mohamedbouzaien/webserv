#ifndef LOCATION_T_HPP
# define LOCATION_T_HPP

# include <netinet/in.h>
# include <string>
# include <vector>
# include <set>

# include "Context_t.hpp"
# include "default_conf.hpp"

class Location_t : public Context_t
{
    private:
        std::string _uri;
        std::vector<Location_t> _locations;
        std::string _alias;

    public:
        Location_t();
        Location_t(std::string uri);
        Location_t(const Location_t &copy);
        Location_t  &operator=(const Location_t &other);
        virtual ~Location_t();

// Setters ---------------------------------
        void add_location(const Location_t *loc);
        void set_alias(const std::string &alias);

// Getters ----------------------------------
        std::vector<Location_t> &get_locations();
        const std::string &get_uri() const;
        const std::string &get_alias() const;
		std::pair<std::string, size_t> get_loc_best_client_max_body_size(std::string path) const;
        //testing
        void print() const;
};


#endif
