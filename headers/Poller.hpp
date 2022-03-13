/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Poller.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouzaie <mbouzaie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/19 15:22:28 by mbouzaie          #+#    #+#             */
/*   Updated: 2022/03/13 21:01:50 by acastelb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef POLLER_HPP
# define POLLER_HPP

# define MAX_CLIENTS	200


# include "common.hpp"
# include "colors.hpp"
# include "Listener.hpp"
# include "Config.hpp"

class Poller
{
	private:
        typedef std::vector<Listener> lstnrs;

		struct pollfd               _fds[MAX_CLIENTS + 1];
		nfds_t		                _nfds;
        lstnrs		                _listeners;
        std::map<int, Listener*>    _listen_map;
        std::map<int, sockaddr_in> _client_map;


        //testing
        void print_fds();

	public:
		Poller(lstnrs &listeners);
		Poller(const Poller &copy);
		Poller  &operator=(const Poller &other);
		virtual ~Poller() {};
        class max_clients_reached : public std::exception
		{
			public:
				virtual const char* what() const throw();
		};
		class PollFailedException : public std::exception
		{
			public:
				virtual const char* what() const throw();
		};
		void    start(void);
		void    handle(const std::vector<Server_t> &servs);
};

#endif
