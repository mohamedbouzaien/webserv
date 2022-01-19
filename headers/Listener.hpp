/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Listener.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouzaie <mbouzaie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/11 14:38:57 by mbouzaie          #+#    #+#             */
/*   Updated: 2022/01/13 15:15:26 by mbouzaie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP

# define SERVER_HPP

# define PORT 80

# include<sys/socket.h>
# include<netinet/in.h>
# include<iostream>

class	Listener
{
	private:
		int			_fd;
		sockaddr_in _address;

	public:
		Listener();
		Listener(Listener &copy);
		Listener	&operator=(Listener &copy);
		virtual	~Listener() {};
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