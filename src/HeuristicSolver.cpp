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

void 		HeuristicSolver::defineHeuristic(MovePtr optionMove, eMoveResult result)
{
	if (result == WIN) {
		optionMove->setHeuristic(optionMove->getCurrentType() == BLACK ? winHeuristic : -winHeuristic);
		return;
	}
	auto & gamefield = optionMove->getGameFieldMod();
	int heur = 0;
	heur +=	defineHorizontalHeuristic(gamefield, optionMove->getCurrentType(), optionMove->getLeftTop(), optionMove->getRightBottom());
	heur +=	defineVerticalHeuristic(gamefield, optionMove->getCurrentType(), optionMove->getLeftTop(), optionMove->getRightBottom());
	heur +=	defineDiagonalLeftHeuristic(gamefield, optionMove->getCurrentType(), optionMove->getLeftTop(), optionMove->getRightBottom());
	heur +=	defineDiagonalRightHeuristic(gamefield, optionMove->getCurrentType(), optionMove->getLeftTop(), optionMove->getRightBottom());
	heur += optionMove->getWhiteCapture() * (-20000) + optionMove->getBlackCapture() * 20000;
	optionMove->setHeuristic(heur);
}


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

	for (int i = leftTop.x; i <= rightBottom.x; ++i) {
		if (i < leftTop.y) {
			diagonalX = 0;
			diagonalY = leftTop.y - i;
		} else {
			diagonalX = i - leftTop.y;
			diagonalY = 0;
		}
		std::vector<eType> diagonal;
		while (diagonalX < 18 && diagonalY < 18) {
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
	return resolveHeuristicFromField(m_heuristicField, currentTurn);
}

int 		HeuristicSolver::defineDiagonalRightHeuristic(std::array<typeArr, N> & gamefield, eType currentTurn,
																		t_pos const & leftTop, t_pos const & rightBottom)
{
	m_heuristicField.clear();
	int diagonalX;
	int diagonalY;
	int rightX = 17 - leftTop.y;

	for (int i = leftTop.x; i <= rightBottom.x; ++i) {
		if (i < rightX) {
			diagonalX = leftTop.y + i < 18 ? leftTop.y + i : 17;
			diagonalY = 0;
		} else {
			diagonalX = 17;
			diagonalY = leftTop.y - (17 - i);
		}
		std::vector<eType> diagonal;
		while (diagonalX >= 0 && diagonalY < 18) {
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
			diagonalY = i - (17 - rightBottom.x);
		}
		std::vector<eType> diagonal;
		while (diagonalX >= 0 && diagonalY < 18) {
			diagonal.push_back(gamefield[diagonalY][diagonalX]);
			diagonalX--;
			diagonalY++;
		}
		m_heuristicField.push_back(diagonal);
	}
	return resolveHeuristicFromField(m_heuristicField, currentTurn);
}

int			HeuristicSolver::resolveHeuristicFromField(std::vector<std::vector<eType>> const & heuristicField, eType currentTurn)
{
	int heuristic = 0;
	//FIND FOUR ALLIGNMENT
	if ((heuristic = defineTwoOpenHeuristic(blackFourTwoOpen, whiteFourTwoOpen, heuristicField, fourTwoOpenHeuristic, currentTurn))){
		return heuristic;
	}

	if ((heuristic = defineOneOpenHeuristic(blackFourOneOpenLeft, whiteFourOneOpenLeft, heuristicField, fourOneOpenHeuristic, currentTurn))
		|| (heuristic = defineOneOpenHeuristic(blackFourOneOpenRight, whiteFourOneOpenRight, heuristicField, fourOneOpenHeuristic, currentTurn))
		|| (heuristic = defineOneOpenHeuristic(blackFourOneOpenCenter_1, whiteFourOneOpenCenter_1, heuristicField, fourOneOpenHeuristic, currentTurn))
		|| (heuristic = defineOneOpenHeuristic(blackFourOneOpenCenter_2, whiteFourOneOpenCenter_2, heuristicField, fourOneOpenHeuristic, currentTurn))
		|| (heuristic = defineOneOpenHeuristic(blackFourOneOpenCenter_3, whiteFourOneOpenCenter_3, heuristicField, fourOneOpenHeuristic, currentTurn))) {
		return heuristic;
	}
	//FIND THREE ALLIGNMENT
	if ((heuristic = defineTwoOpenHeuristic(blackThreeTwoOpen_1, whiteThreeTwoOpen_1, heuristicField, threeTwoOpenHeuristic, currentTurn))
		|| (heuristic = defineTwoOpenHeuristic(blackThreeTwoOpen_2, whiteThreeTwoOpen_2, heuristicField, threeTwoOpenHeuristic, currentTurn))){
		return heuristic;
	}

	if ((heuristic = defineOneOpenHeuristic(blackThreeOneOpenRight, whiteThreeOneOpenRight, heuristicField, threeOneOpenHeuristic, currentTurn))
		|| (heuristic = defineOneOpenHeuristic(blackThreeOneOpenLeft, whiteThreeOneOpenLeft, heuristicField, threeOneOpenHeuristic, currentTurn))
		|| (heuristic = defineOneOpenHeuristic(blackThreeOneOpenCenter_1, whiteThreeOneOpenCenter_1, heuristicField, threeOneOpenHeuristic, currentTurn))
		|| (heuristic = defineOneOpenHeuristic(blackThreeOneOpenCenter_2, whiteThreeOneOpenCenter_2, heuristicField, threeOneOpenHeuristic, currentTurn))
		|| (heuristic = defineOneOpenHeuristic(blackThreeOneOpenCenter_3, whiteThreeOneOpenCenter_3, heuristicField, threeOneOpenHeuristic, currentTurn))
		|| (heuristic = defineOneOpenHeuristic(blackThreeOneOpenCenter_4, whiteThreeOneOpenCenter_4, heuristicField, threeOneOpenHeuristic, currentTurn))) {
		return heuristic;
	}

	//FIND TWO ALLIGNMENT
	if ((heuristic = defineTwoOpenHeuristic(blackTwoTwoOpen_1, whiteTwoTwoOpen_1, heuristicField, twoTwoOpenHeuristic, currentTurn))
		|| (heuristic = defineTwoOpenHeuristic(blackTwoTwoOpen_2, whiteTwoTwoOpen_2, heuristicField, twoTwoOpenHeuristic, currentTurn))
		|| (heuristic = defineTwoOpenHeuristic(blackTwoTwoOpen_3, whiteTwoTwoOpen_3, heuristicField, twoTwoOpenHeuristic, currentTurn))){
		return heuristic;
	}

	if ((heuristic = defineOneOpenHeuristic(blackTwoOneOpenRight, whiteTwoOneOpenRight, heuristicField, oneTwoOpenHeuristic, currentTurn))
		|| (heuristic = defineOneOpenHeuristic(blackTwoOneOpenLeft, whiteTwoOneOpenLeft, heuristicField, oneTwoOpenHeuristic, currentTurn))
	) {
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

int 		HeuristicSolver::defineOneOpenHeuristic( oneOpenSample const & blackSample, oneOpenSample const & whiteSample, std::vector<std::vector<eType>> const & gamefield,
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
	for (auto && column : gamefield) {
		auto it = std::search(column.begin(), column.end(), sampleTwoOpen.begin(), sampleTwoOpen.end());
		if (it != column.end())	{
			return 1;
		}
	}
	return 0;
}

int 		HeuristicSolver::findOneOpen(oneOpenSample const & sampleOneOpen, std::vector<std::vector<eType>> const & gamefield)
{
	for (auto && column : gamefield) {
		auto it = std::search(column.begin(), column.end(), sampleOneOpen.begin(), sampleOneOpen.end());
		if (it != column.end()) {
			return 1;
		}
	}
	return 0;
}
