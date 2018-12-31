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
	std::cout << "Usage:	./gomoku  		for two players\n"
				<<"	./gomoku AI  		for playing with bot\n"
				 << "	./gomoku AI [CONDITION]	for playing with bot and start condition\n"
				 << "	Posible CONDITION: \n"
				 << "		PRO \n		SWAP\n		SWAP2\n" << std::endl;
}

int 		inputProcess(int ac, char **av, std::shared_ptr<Gomoku> &gomoku)
{
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
			gomoku = std::make_shared<Gomoku>("AI",condition);
		}
	} else if (ac == 2 ) {
		if (strcmp(av[1],"AI") == 0)
			gomoku = std::make_shared<Gomoku>("AI",EMPTY_FIELD);
		else {
			printUsage();
			return 0;
		}
	} else {
		gomoku = std::make_shared<Gomoku>("none",EMPTY_FIELD);
	}
	return 1;
}

int main(int ac, char **av)
{
	std::shared_ptr<Gomoku> gomoku;

	if (inputProcess(ac, av, gomoku)){
		if (gomoku)
			gomoku->render();
	};
	return 0;
}
