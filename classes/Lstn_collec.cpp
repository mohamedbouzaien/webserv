# include "../headers/Lstn_collec.hpp"

Lstn_collec::Lstn_collec(const std::vector<Server_t> &servs)
{
    std::set<Server_t::listen_pair_t> occurences;

    for (std::vector<Server_t>::const_iterator srv = servs.begin(); srv != servs.end(); ++srv)
    {
        for (std::set<Server_t::listen_pair_t>::iterator it = srv->get_listen().begin(); it != srv->get_listen().end(); ++it)
            occurences.insert(*it);
    }
    for (std::set<Server_t::listen_pair_t>::iterator it = occurences.begin(); it != occurences.end(); ++it)
    {
        _collec.push_back(Listener());
        _collec.back().execute(it->first, it->second);
    }

    // Output
    std::cout << BI_PURPLE << "Listening to :\n";
    std::cout << I_PURPLE;
    for (std::set<Server_t::listen_pair_t>::iterator it = occurences.begin(); it != occurences.end(); ++it)
        std::cout << " - " << it->first << ":" << it->second << '\n';
    std::cout << COLOR_OFF;
    std::cout << "\n";
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

std::vector<Listener> & Lstn_collec::get_collec(){
    return _collec;
}
