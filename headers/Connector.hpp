/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Connector.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouzaie <mbouzaie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/11 16:37:10 by mbouzaie          #+#    #+#             */
/*   Updated: 2022/01/17 18:03:01 by mbouzaie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONNECTOR_HPP

# define CONNECTOR_HPP

# define MAX_CLIENTS	200

# include "Listener.hpp"
# include <sys/socket.h>
# include <sys/poll.h>
# include <unistd.h>
# include <vector>

class Connector
{
	private:
		int				_client_socket;
		struct pollfd	_fds[MAX_CLIENTS];
		int				_nfds;
		Listener		_listener;
	public:
		Connector(Listener &listener);
		Connector(Connector &copy);
		Connector	&operator=(Connector &other);
		virtual	~Connector() {};
		class ConnectionFailedException : public std::exception
		{
			public:
				virtual const char* what() const throw();
		};
		class RecvFailedException : public std::exception
		{
			public:
				virtual const char* what() const throw();
		};
		class PollFailedException : public std::exception
		{
			public:
				virtual const char* what() const throw();
		};
		void	poll_server();
		void	accept_c();
		void	handle();
		void	setClientSocket(int client_socket);
		int		getClientSocket()	const;
};

#endif