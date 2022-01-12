/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Connector.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouzaie <mbouzaie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/11 16:37:10 by mbouzaie          #+#    #+#             */
/*   Updated: 2022/01/12 12:43:06 by mbouzaie         ###   ########.fr       */
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
		int		_client_socket;
		Listener	_listener;
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
		void	accept_c();
		void	handle();
};

#endif