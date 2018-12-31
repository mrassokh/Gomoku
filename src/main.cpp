/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrassokh <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/25 11:35:01 by mrassokh          #+#    #+#             */
/*   Updated: 2018/08/25 11:35:05 by mrassokh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Gomoku.hpp"
# include <cstring>

void 		printUsage()
{
	std::cout << "Usage" << std::endl;
}

int main(int ac, char **av)
{

	Gomoku *gomoku;
	eStartGame condition = EMPTY_FIELD;
	if (ac > 3) {
		printUsage();
		return 0;
	}
	if (ac == 3){
		if (strcmp(av[2],"PRO") == 0)
			condition = PRO_FIELD;
		else if (strcmp(av[2],"SWAP") == 0)
			condition = SWAP_FIELD;
		else if (strcmp(av[2],"SWAP2") == 0)
			condition = SWAP_TWO_FIELD;

		if (condition == EMPTY_FIELD && strcmp(av[1],"AI") == 0){
			printUsage();
			return 0;
		}
		else {
			gomoku = new Gomoku("AI",condition);
		}
	} else if (ac == 2 ) {
		if (strcmp(av[1],"AI") == 0)
			gomoku = new Gomoku("AI",EMPTY_FIELD);
		else {
			printUsage();
			return 0;
		}
	} else {
	 	gomoku = new Gomoku("none", EMPTY_FIELD);
	}
	gomoku->render();
	return 0;
}
