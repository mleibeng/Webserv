/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mleibeng <mleibeng@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/20 00:05:06 by mleibeng          #+#    #+#             */
/*   Updated: 2024/08/21 09:55:30 by mleibeng         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "WebServer.hpp"
#include <iostream>


//should probably also implement signal handling!
int main(int argc, char **argv)
{
	if (argc != 2)
	{
		std::cout << "Wrong usage: No Config file given" << std::endl;
		return 1;
	}
	std::string config_file = argv[1];
	try
	{
		WebServer MainServ(config_file);
		MainServ.start();
	}
	catch (const std::exception &e)
	{
		std::cerr << "Error: " << e.what() << std::endl;
		return 1;
	}
	catch (...)
	{
		std::cerr << "unknown error" << std::endl;
		return 1;
	}
	return (0);
}