/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouzaie <mbouzaie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/10 12:24:45 by mbouzaie          #+#    #+#             */
/*   Updated: 2022/01/19 19:36:30 by mbouzaie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include<iostream>
# include "headers/Connector.hpp"
# include "headers/Poller.hpp"

# include <string.h>
#include "headers/Request.hpp"

int main()
{
	try
	{
		Listener listener;
		listener.execute();
		Poller		poller(listener);
		while (true)
		{
			poller.start();
			poller.handle();
		}
		
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << " errno: "<< errno << '\n';
	}
	
}
