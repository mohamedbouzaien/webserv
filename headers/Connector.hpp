/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Connector.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouzaie <mbouzaie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/11 16:37:10 by mbouzaie          #+#    #+#             */
/*   Updated: 2022/02/11 17:05:26 by acastelb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONNECTOR_HPP

# define CONNECTOR_HPP

# include "Listener.hpp"
# include <sys/socket.h>
# include <unistd.h>

class Connector
{
	private:
		int				_client_socket;
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
		void		poll_server();
		void		accept_c();
		int			handle();
		void		setClientSocket(int client_socket);
		int			getClientSocket()	const;
		int			readSocket(std::string &request);
		Listener	getListener()	const;
};

#endif
