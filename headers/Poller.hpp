/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Poller.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouzaie <mbouzaie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/19 15:22:28 by mbouzaie          #+#    #+#             */
/*   Updated: 2022/01/25 23:00:00 by mbouzaie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef POLLER_HPP
# define POLLER_HPP

# define MAX_CLIENTS	200

# include <sys/poll.h>
# include <iostream>

# include "Listener.hpp"

class Poller
{
	private:
		struct pollfd	_fds[MAX_CLIENTS];
		int				_nfds;
		Listener		_listener;
	public:
		Poller(Listener &listner);
		Poller(const Poller &copy);
		Poller  &operator=(const Poller &other);
		virtual ~Poller() {};
		class PollFailedException : public std::exception
		{
			public:
				virtual const char* what() const throw();
		};
		void    start(void);
		void    handle(void);
};

#endif