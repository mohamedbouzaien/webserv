#ifndef LSTN_COLLEC_HPP
# define LSTN_COLLEC_HPP

# include <set>

# include "Listener.hpp"
# include "Server_t.hpp"

class Lstn_collec{
    private:
        std::vector<Listener> _collec;

    public:
        Lstn_collec(const std::vector<Server_t> &servs);
        Lstn_collec(const Lstn_collec & cpy);
        Lstn_collec &operator=(const Lstn_collec & cpy);
        virtual ~Lstn_collec();

        std::vector<Listener> &get_collec();
};

#endif
