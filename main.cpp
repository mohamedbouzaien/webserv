/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouzaie <mbouzaie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/10 12:24:45 by mbouzaie          #+#    #+#             */
/*   Updated: 2022/01/12 15:40:10 by acastelb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include<iostream>
# include "headers/Connector.hpp"
#include "headers/Request.hpp"

int main()
{
	try
	{
		Listener listener;
		listener.execute();
		while (true)
		{
			Connector	connector(listener);
			connector.accept_c();
			connector.handle();
		}
		
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << " errno: "<< errno << '\n';
	}
	
}
