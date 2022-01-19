/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouzaie <mbouzaie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/10 12:24:45 by mbouzaie          #+#    #+#             */
/*   Updated: 2022/01/19 15:05:25 by mbouzaie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include<iostream>
# include "headers/Connector.hpp"

# include <string.h>

int main()
{
	try
	{
		Listener listener;
		listener.execute();
		int	rc;
		int	current_sockets;
		Connector	connector(listener);
		while (true)
		{
			current_sockets = nfds;
			for (int i = 0; i < current_sockets; i++)
			{
				if(fds[i].revents == 0)
        			continue;
				if (fds[i].fd == listener.getFd())
				{
					printf("  Listening socket is readable\n");
					connector.accept_c();
					printf("  New incoming connection - %d\n", connector.getClientSocket());
					fds[nfds].fd = connector.getClientSocket();
					fds[nfds].events = POLLIN;
					nfds++;
					}
				else
				{	
					printf("  Descriptor %d is readable\n", fds[i].fd);
					connector.setClientSocket(fds[i].fd);
					connector.handle();
				}
			}
		}
		
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << " errno: "<< errno << '\n';
	}
	
}