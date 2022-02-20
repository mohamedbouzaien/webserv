/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Poller.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouzaie <mbouzaie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/19 15:22:28 by mbouzaie          #+#    #+#             */
/*   Updated: 2022/02/09 18:26:03 by mbouzaie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef POLLER_HPP
# define POLLER_HPP

# define MAX_CLIENTS	200

# include <sys/poll.h>
# include <iostream>

# include "Listener.hpp"
# include "Config.hpp"

class Poller
{
	private:
        typedef std::vector<Listener> lstnrs;

		struct pollfd             _fds[MAX_CLIENTS];
		int			              _nfds;
        lstnrs		              _listeners;
        std::map<int, Listener*>  _index_map;
	public:
		Poller(lstnrs &listeners);
		Poller(const Poller &copy);
		Poller  &operator=(const Poller &other);
		virtual ~Poller() {};
		class PollFailedException : public std::exception
		{
			public:
				virtual const char* what() const throw();
		};
		void    start(void);
		void    handle(const std::vector<Server_t> &servs);
};

#endif
