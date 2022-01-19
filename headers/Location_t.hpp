#ifndef LOCATION_T_HPP
# define LOCATION_T_HPP

# include <netinet/in.h>
# include <string>
# include <set>
# include <map>

# include "Context_t.hpp"

class Location_t : public Context_t
{
    private:
        std::string _uri;

    public:
        Location_t(std::string uri);
        Location_t(const Location_t &copy);
        Location_t  &operator=(const Location_t &other);
        virtual ~Location_t();

        const std::string &get_uri() const;

        //testing
        void print();
};


#endif
