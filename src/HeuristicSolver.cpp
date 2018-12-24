/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HeuristicSolver.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrassokh <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/24 16:57:25 by mrassokh          #+#    #+#             */
/*   Updated: 2018/12/24 16:57:27 by mrassokh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HeuristicSolver.hpp"
HeuristicSolver &	HeuristicSolver::Instance()
{
	static HeuristicSolver instance;
	return instance;
}

// void 		HeuristicSolver::defineHeuristic(MovePtr optionMove, eMoveResult result)
// {
// 	if (result == WIN) {
// 		optionMove->setHeuristic(optionMove->getCurrentType() == BLACK ? winHeuristic : -winHeuristic);
// 		return;
// 	}
// 	std::array<typeArr, N> & gamefield = optionMove->getGameFieldMod();
// 	int heur = 0;
// 	heur +=	defineHorizontalHeuristic(gamefield, optionMove->getCurrentType(), optionMove->getLeftTop(), optionMove->getRightBottom());
// 	heur +=	defineVerticalHeuristic(gamefield, optionMove->getCurrentType(), optionMove->getLeftTop(), optionMove->getRightBottom());
// 	heur +=	defineDiagonalLeftHeuristic(gamefield, optionMove->getCurrentType(), optionMove->getLeftTop(), optionMove->getRightBottom());
// 	heur +=	defineDiagonalRightHeuristic(gamefield, optionMove->getCurrentType(), optionMove->getLeftTop(), optionMove->getRightBottom());
// 	heur += optionMove->getWhiteCapture() * (-20000) + optionMove->getBlackCapture() * 20000;
// 	optionMove->setHeuristic(heur);
// }


int 		HeuristicSolver::defineHorizontalHeuristic(std::array<typeArr, N> & gamefield, eType currentTurn,
													t_pos const & leftTop, t_pos const & rightBottom)
{
	m_heuristicField.clear();
	int start = leftTop.y > 0 ? leftTop.y - 1 : 0;
	int end = rightBottom.y < 17 ? rightBottom.y + 1 : 17;
	for (int y =  start; y < end; ++y) {
		std::vector<eType> row;
		for (int x = 0; x < 18; ++x) {
			row.push_back(gamefield[y][x]);
		}
		m_heuristicField.push_back(row);
	}
	return resolveHeuristicFromField(m_heuristicField, currentTurn);
}


int 		HeuristicSolver::defineVerticalHeuristic(std::array<typeArr, N> & gamefield, eType currentTurn,
																		t_pos const & leftTop, t_pos const & rightBottom)
{
	m_heuristicField.clear();
	int start = leftTop.x > 0 ? leftTop.x - 1 : 0;
	int end = rightBottom.x < 17 ? rightBottom.x + 1 : 17;
	for (int x = start; x < end; ++x) {
		std::vector<eType> column;
		for (int y = 0; y < 18; ++y) {
			column.push_back(gamefield[y][x]);
		}
		m_heuristicField.push_back(column);
	}
	return resolveHeuristicFromField(m_heuristicField, currentTurn);
}

int 		HeuristicSolver::defineDiagonalLeftHeuristic(std::array<typeArr, N> & gamefield, eType currentTurn,
																		t_pos const & leftTop, t_pos const & rightBottom)
{
	m_heuristicField.clear();
	int diagonalX;
	int diagonalY;
	//printf("defineDiagonalLeftHeuristic left top x = %d y = %d rightBottom x = %d y = %d:\n", leftTop.x, leftTop.y, rightBottom.x, rightBottom.y);
	for (int i = leftTop.x; i <= rightBottom.x; ++i) {
		if (i < leftTop.y) {
			diagonalX = 0;
			diagonalY = leftTop.y - i;
		} else {
			diagonalX = i - leftTop.y;
			diagonalY = 0;
		}
		//printf("i = %d top diagonalX = %d diagonalY = %d :\n", i,diagonalX, diagonalY);
		std::vector<eType> diagonal;
		while (diagonalX < 18 && diagonalY < 18) {
		//	printf("top diagonalX = %d diagonalY = %d elem = %d:\n", diagonalX, diagonalY, gamefield[diagonalY][diagonalX]);
			diagonal.push_back(gamefield[diagonalY][diagonalX]);
			diagonalX++;
			diagonalY++;
		}
		m_heuristicField.push_back(diagonal);
	}

	for (int i = leftTop.y + 1; i <= rightBottom.y; ++i) {
		if (i < leftTop.x) {
			diagonalX = leftTop.x - i;
			diagonalY = 0;
		} else {
			diagonalX = 0;
			diagonalY = i - leftTop.x;
		}
		std::vector<eType> diagonal;
		while (diagonalX < 18 && diagonalY < 18) {
			diagonal.push_back(gamefield[diagonalY][diagonalX]);
			diagonalX++;
			diagonalY++;
		}
		m_heuristicField.push_back(diagonal);
	}
	//printf("m_heuristicField:\n");
	// for (auto && row : m_heuristicField) {
	// 	for (auto && element : row) {
	// 		printf("%d ",  element);
	// 	}
	// 	printf(";\n ");
	// }
	return resolveHeuristicFromField(m_heuristicField, currentTurn);
}

int 		HeuristicSolver::defineDiagonalRightHeuristic(std::array<typeArr, N> & gamefield, eType currentTurn,
																		t_pos const & leftTop, t_pos const & rightBottom)
{
	m_heuristicField.clear();
	int diagonalX;
	int diagonalY;
	int rightX = 17 - leftTop.y;
	//printf("definediagonalRightHeuristic left top x = %d y = %d rightBottom x = %d y = %d:\n", leftTop.x, leftTop.y, rightBottom.x, rightBottom.y);
	for (int i = leftTop.x; i <= rightBottom.x; ++i) {
		if (i < rightX) {
			diagonalX = leftTop.y + i < 18 ? leftTop.y + i : 17;
			diagonalY = 0; //leftTop.y - i;
		} else {
			diagonalX = 17;
			diagonalY = leftTop.y - (17 - i);//leftTop.y - i > 0 ? leftTop.y - i : 0; //leftTop.y - i;
		}
	//	printf("i = %d top diagonalX = %d diagonalY = %d :\n", i,diagonalX, diagonalY);
		std::vector<eType> diagonal;
		while (diagonalX >= 0 && diagonalY < 18) {
		//	printf("top diagonalX = %d diagonalY = %d elem = %d:\n", diagonalX, diagonalY, gamefield[diagonalY][diagonalX]);
			diagonal.push_back(gamefield[diagonalY][diagonalX]);
			diagonalX--;
			diagonalY++;
		}
		m_heuristicField.push_back(diagonal);
	}

	for (int i = leftTop.y + 1; i <= rightBottom.y; ++i) {
		rightX = 17 - i;
		if (rightBottom.x < rightX) {
			diagonalX = rightBottom.x + i;
			diagonalY = 0;
		} else {
			diagonalX = 17;
			diagonalY = i - (17 - rightBottom.x);//i - leftTop.x;
		}
		//printf("i = %d top diagonalX = %d diagonalY = %d :\n", i,diagonalX, diagonalY);
		std::vector<eType> diagonal;
		while (diagonalX >= 0 && diagonalY < 18) {
			//printf("top diagonalX = %d diagonalY = %d elem = %d:\n", diagonalX, diagonalY, gamefield[diagonalY][diagonalX]);
			diagonal.push_back(gamefield[diagonalY][diagonalX]);
			diagonalX--;
			diagonalY++;
		}
		m_heuristicField.push_back(diagonal);
	}
	//printf("m_heuristicField:\n");
	// for (auto && row : m_heuristicField) {
	// 	for (auto && element : row) {
	// 		printf("%d ",  element);
	// 	}
	// 	printf(";\n ");
	// }
	return resolveHeuristicFromField(m_heuristicField, currentTurn);
}

int		HeuristicSolver::resolveHeuristicFromField(std::vector<std::vector<eType>> const & heuristicField, eType currentTurn)
{
	int heuristic = 0;

	if ((heuristic = defineTwoOpenHeuristic(blackFourTwoOpen, whiteFourTwoOpen, heuristicField, fourTwoOpenHeuristic, currentTurn))){
		//printf("FourTwoOpen heuristic = %d\n\n\n\n", heuristic);
		return heuristic;
	}

	if ((heuristic = defineOneOpenHeuristic(blackFourOneOpenLeft, whiteFourOneOpenLeft, heuristicField, fourOneOpenHeuristic, currentTurn))
		|| (heuristic = defineOneOpenHeuristic(blackFourOneOpenRight, whiteFourOneOpenRight, heuristicField, fourOneOpenHeuristic, currentTurn))
		|| (heuristic = defineOneOpenHeuristic(blackFourOneOpenCenter_1, whiteFourOneOpenCenter_1, heuristicField, fourOneOpenHeuristic, currentTurn))
		|| (heuristic = defineOneOpenHeuristic(blackFourOneOpenCenter_2, whiteFourOneOpenCenter_2, heuristicField, fourOneOpenHeuristic, currentTurn))
		|| (heuristic = defineOneOpenHeuristic(blackFourOneOpenCenter_3, whiteFourOneOpenCenter_3, heuristicField, fourOneOpenHeuristic, currentTurn))) {
		//printf("FourOneOpen heuristic = %d\n\n\n\n", heuristic);
		return heuristic;
	}
	//FIND THREE ALLIGNMENT
	if ((heuristic = defineTwoOpenHeuristic(blackThreeTwoOpen_1, whiteThreeTwoOpen_1, heuristicField, threeTwoOpenHeuristic, currentTurn))
		|| (heuristic = defineTwoOpenHeuristic(blackThreeTwoOpen_2, whiteThreeTwoOpen_2, heuristicField, threeTwoOpenHeuristic, currentTurn))){
		//printf("ThreeTwoOpen heuristic = %d\n\n\n\n", heuristic);
		return heuristic;
	}

	if ((heuristic = defineOneOpenHeuristic(blackThreeOneOpenRight, whiteThreeOneOpenRight, heuristicField, threeOneOpenHeuristic, currentTurn))
		|| (heuristic = defineOneOpenHeuristic(blackThreeOneOpenLeft, whiteThreeOneOpenLeft, heuristicField, threeOneOpenHeuristic, currentTurn))
		|| (heuristic = defineOneOpenHeuristic(blackThreeOneOpenCenter_1, whiteThreeOneOpenCenter_1, heuristicField, threeOneOpenHeuristic, currentTurn))
		|| (heuristic = defineOneOpenHeuristic(blackThreeOneOpenCenter_2, whiteThreeOneOpenCenter_2, heuristicField, threeOneOpenHeuristic, currentTurn))
		|| (heuristic = defineOneOpenHeuristic(blackThreeOneOpenCenter_3, whiteThreeOneOpenCenter_3, heuristicField, threeOneOpenHeuristic, currentTurn))
		|| (heuristic = defineOneOpenHeuristic(blackThreeOneOpenCenter_4, whiteThreeOneOpenCenter_4, heuristicField, threeOneOpenHeuristic, currentTurn))) {
		//printf("ThreeOneOpen heuristic = %d\n\n\n\n", heuristic);
		return heuristic;
	}

	//FIND TWO ALLIGNMENT
	if ((heuristic = defineTwoOpenHeuristic(blackTwoTwoOpen_1, whiteTwoTwoOpen_1, heuristicField, twoTwoOpenHeuristic, currentTurn))
		|| (heuristic = defineTwoOpenHeuristic(blackTwoTwoOpen_2, whiteTwoTwoOpen_2, heuristicField, twoTwoOpenHeuristic, currentTurn))
		|| (heuristic = defineTwoOpenHeuristic(blackTwoTwoOpen_3, whiteTwoTwoOpen_3, heuristicField, twoTwoOpenHeuristic, currentTurn))){
		//printf("TwoTwoOpen heuristic = %d\n\n\n\n", heuristic);
		return heuristic;
	}

	if ((heuristic = defineOneOpenHeuristic(blackTwoOneOpenRight, whiteTwoOneOpenRight, heuristicField, oneTwoOpenHeuristic, currentTurn))
		|| (heuristic = defineOneOpenHeuristic(blackTwoOneOpenLeft, whiteTwoOneOpenLeft, heuristicField, oneTwoOpenHeuristic, currentTurn))
	) {
		//printf("TwoOneOpen heuristic = %d\n\n\n\n", heuristic);
		return heuristic;
	}

	return 0;
}

int 		HeuristicSolver::defineTwoOpenHeuristic(twoOpenSample const & blackSample, twoOpenSample const & whiteSample, std::vector<std::vector<eType>> const & gamefield,
																		const int heuristic, eType currentTurn)
{
	int returnHeur = 0;
	if (findTwoOpen(blackSample, gamefield))
		returnHeur += currentTurn == BLACK ? heuristic / 2 : heuristic;
	if (findTwoOpen(whiteSample, gamefield))
		returnHeur -= currentTurn == BLACK ? heuristic  : heuristic / 2;
	return returnHeur;
}

int 							HeuristicSolver::defineOneOpenHeuristic( oneOpenSample const & blackSample, oneOpenSample const & whiteSample, std::vector<std::vector<eType>> const & gamefield,
														  const int heuristic, eType currentTurn)
{
	int returnHeur = 0;
	if (findOneOpen(blackSample, gamefield))
		returnHeur += currentTurn == BLACK ? heuristic / 2 : heuristic;
	if (findOneOpen(whiteSample, gamefield))
		returnHeur -= currentTurn == BLACK ? heuristic  : heuristic / 2;
	return returnHeur;
}




int 		HeuristicSolver::findTwoOpen(twoOpenSample const & sampleTwoOpen, std::vector<std::vector<eType>> const & gamefield)
{
	//std::vector<eType>::iterator it;
	for (auto && column : gamefield) {
		auto it = std::search(column.begin(), column.end(), sampleTwoOpen.begin(), sampleTwoOpen.end());
		if (it != column.end())
		{
			//printf("TwoOpen find in  and y = %ld\n\n\n\n", it - column.begin());
			return 1;
		}
	}
	return 0;
}

int 		HeuristicSolver::findOneOpen(oneOpenSample const & sampleOneOpen, std::vector<std::vector<eType>> const & gamefield)
{
	//std::vector<eType>::iterator it;
	for (auto && column : gamefield) {
		auto it = std::search(column.begin(), column.end(), sampleOneOpen.begin(), sampleOneOpen.end());
		if (it != column.end())
		{
			//printf("OneOpen find in  and y = %ld\n\n\n\n", it - column.begin());
			return 1;
		}
	}
	return 0;
}


void 		HeuristicSolver::defineHeuristic(MovePtr optionMove, eMoveResult result)
{
	if (result == WIN) {
		optionMove->setHeuristic(optionMove->getCurrentType() == BLACK ? winHeuristic : -winHeuristic);
		return;
	}
	std::array<typeArr, N> & gamefield = optionMove->getGameFieldMod();
	int heur = 0;
	heur +=	defineHorizontalHeuristic(gamefield, optionMove->getCurrentType(), optionMove->getLeftTop(), optionMove->getRightBottom());
	heur +=	defineVerticalHeuristic(gamefield, optionMove->getCurrentType(), optionMove->getLeftTop(), optionMove->getRightBottom());
	heur +=	defineDiagonalLeftHeuristic(gamefield, optionMove->getCurrentType(), optionMove->getLeftTop(), optionMove->getRightBottom());
	heur +=	defineDiagonalRightHeuristic(gamefield, optionMove->getCurrentType(), optionMove->getLeftTop(), optionMove->getRightBottom());
	heur += optionMove->getWhiteCapture() * (-20000) + optionMove->getBlackCapture() * 20000;
	optionMove->setHeuristic(heur);
}
