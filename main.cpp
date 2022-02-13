/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouzaie <mbouzaie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/10 12:24:45 by mbouzaie          #+#    #+#             */
/*   Updated: 2022/02/09 18:25:04 by mbouzaie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include<iostream>
# include "headers/Connector.hpp"
# include "headers/Config.hpp"
# include "headers/Poller.hpp"

# include <string.h>
#include "headers/Request.hpp"



int main(int ac, char **av) {
    const char * conf_path = "./tests/default.conf";
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
            Listener listener;
            listener.execute();
            Poller		poller(listener);
            while (false)
            {
                poller.start();
                poller.handle(conf.get_servers()[0]);
            }

        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << " errno: "<< errno << '\n';
        }
    }
}
