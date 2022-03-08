/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouzaie <mbouzaie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/10 12:24:45 by mbouzaie          #+#    #+#             */
/*   Updated: 2022/03/08 10:36:27 by acastelb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include<iostream>
# include "headers/Connector.hpp"
# include "headers/Config.hpp"
# include "headers/Poller.hpp"
# include "headers/Lstn_collec.hpp"

# include <string.h>
#include <csignal>
# include "headers/Request.hpp"


void sig_handler(int signal) {
	std::cout << "signal handled :" << signal << std::endl;
	exit(1);
}

int main(int ac, char **av) {
    const char * conf_path = "./tests/default.conf";
	signal(SIGINT, sig_handler);
    if (ac > 2)
        std::cerr << "Wrong arg number. Can take at most one arg (configuration file path)" << std::endl;
    else
    {
        if (ac == 2)
            conf_path = av[1];
        try
        {
            Config conf(conf_path);
            conf.print_servers(); // Testing // prints all servers content

            Lstn_collec listeners(conf.get_servers());

            Poller		poller(listeners.get_collec());
            while (true)
            {
                poller.start();
                poller.handle(conf.get_servers());
            }

        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << " errno: "<< errno << '\n';
        }
    }
}
