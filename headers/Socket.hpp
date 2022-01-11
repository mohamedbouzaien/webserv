/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouzaie <mbouzaie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/11 14:38:57 by mbouzaie          #+#    #+#             */
/*   Updated: 2022/01/11 22:08:19 by mbouzaie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP

# define SERVER_HPP

# define PORT 8080

# include<sys/socket.h>
# include<netinet/in.h>
# include<iostream>

class	Socket
{
	private:
		int			_fd;
		sockaddr_in _address;

	public:
		Socket();
		Socket(Socket &copy);
		Socket	&operator=(Socket &copy);
		virtual	~Socket() {};
		int		getFd();
		sockaddr_in	&getAddress();
		class CreationFailedException : public std::exception
		{
			public:
				virtual const char* what() const throw();
		};
		class PortBindingFailedException : public std::exception
		{
			public:
				virtual const char* what() const throw();
		};
		class ListeningFailedException : public std::exception
		{
			public:
				virtual const char* what() const throw();
		};
		void	execute();
};

#endif