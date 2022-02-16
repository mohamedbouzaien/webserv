# include "../headers/Lstn_collec.hpp"

Lstn_collec::Lstn_collec(std::vector<Server_t> &servs)
{
    std::set<Server_t::listen_pair_t> occurences;

    for (std::vector<Server_t>::iterator srv = servs.begin(); srv != servs.end(); ++srv)
    {
        for (std::set<Server_t::listen_pair_t>::iterator it = srv->get_listen().begin(); it != srv->get_listen().end(); ++it)
            occurences.insert(*it);
    }
    for (std::set<Server_t::listen_pair_t>::iterator it = occurences.begin(); it != occurences.end(); ++it)
    {
        _collec.push_back(Listener());
        _collec.back().execute(it->first, it->second);
    }
}

Lstn_collec::Lstn_collec(const Lstn_collec &copy) : _collec(copy._collec)
{
}

Lstn_collec	&Lstn_collec::operator=(const Lstn_collec &copy)
{
    _collec = copy._collec;
    return *this;
}

Lstn_collec::~Lstn_collec(){}
