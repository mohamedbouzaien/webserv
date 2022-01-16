/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouzaie <mbouzaie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/10 12:24:45 by mbouzaie          #+#    #+#             */
/*   Updated: 2022/01/12 12:44:24 by mbouzaie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include<iostream>
# include "headers/Connector.hpp"
# include "headers/Config.hpp"

//int main(int ac, char **av)
int nrml(int ac, char **av)
{
    const char * conf_path = "./default.conf";
    if (ac > 2)
        std::cerr << "Wrong arg number. Can take at most one arg (configuration file path)" << std::endl;
    else
    {
        if (ac == 2)
            conf_path = av[1];
        try
        {
            Config conf(conf_path);
            /*
            Listener listener;
            listener.execute();
            while (true)
            {
                Connector	connector(listener);
                connector.accept_c();
                connector.handle();
            }
            */
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << " errno: "<< errno << '\n';
        }
    }
    return 1;
}

int main(int ac, char **av) {
    nrml(ac, av);
    while (1);
}
