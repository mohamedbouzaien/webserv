/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbouzaie <mbouzaie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/10 12:24:45 by mbouzaie          #+#    #+#             */
/*   Updated: 2022/03/13 17:09:12 by acastelb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include<iostream>
# include "headers/Connector.hpp"
# include "headers/Config.hpp"
# include "headers/Poller.hpp"
# include "headers/Lstn_collec.hpp"
# include "headers/colors.hpp"
# include "headers/common.hpp"
# include <string.h>
#include <csignal>
# include "headers/Request.hpp"


int should_run = 1;

void sig_handler(int signal) {
	(void) signal;
	std::cout << RED << " [Stoping Webserv]" << COLOR_OFF << std::endl;
	should_run = 0;
}

void ascii_title()
{
    std::cout << RED;
	std::cout << "                   __        __   _" << std::endl;
	std::cout << "                   \\ \\      / /__| |__  ___  ___ _ ____   __" << std::endl;
	std::cout << "                    \\ \\ /\\ / / _ \\ '_ \\/ __|/ _ \\ '__\\ \\ / /" << std::endl;
	std::cout << "                     \\ V  V /  __/ |_) \\__ \\  __/ |   \\ V / " << std::endl;
	std::cout << "                      \\_/\\_/ \\___|_.__/|___/\\___|_|    \\_/  " << std::endl;
	std::cout << COLOR_OFF;
	std::cout << I_PURPLE;
	std::cout << "                              _   _          _   _ " << std::endl;
	std::cout << "                             | | | |_      _| | | |" << std::endl;
	std::cout << "                             | | | \\ \\ /\\ / / | | |" << std::endl;
	std::cout << "                             | |_| |\\ V  V /| |_| |" << std::endl;
	std::cout << "                              \\___/  \\_/\\_/  \\___/ " << std::endl;
	std::cout << COLOR_OFF;
    std::cout << std::endl;
	std::cout << std::endl;
	std::cout << std::endl;
}

void print_servs_found(int n)
{
    std::cout << BI_PURPLE << "Webserv launched, " << n;
    if (n == 1)
       std::cout << " server found\n";
    else
       std::cout << " servers found\n";
    std::cout << "Starting to listen..." << COLOR_OFF << std::endl << std::endl;
}

int main(int ac, char **av) {
    const char * conf_path = "./config/default.conf";

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
            //conf.print_servers(); // Testing // prints all servers content
            ascii_title();

            print_servs_found(conf.get_servers().size());

            Lstn_collec listeners(conf.get_servers());

            Poller		poller(listeners.get_collec());
            while (should_run)
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
