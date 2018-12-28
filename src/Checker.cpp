/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Checker.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrassokh <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/24 15:07:34 by mrassokh          #+#    #+#             */
/*   Updated: 2018/12/24 15:07:35 by mrassokh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Checker.hpp"

Checker::Checker()
{
	m_isPossibleCaptureWinFunc.push_back(&isPossibleCaptureHorizontalWin);
	m_isPossibleCaptureWinFunc.push_back(&isPossibleCaptureVerticalWin);
	m_isPossibleCaptureWinFunc.push_back(&isPossibleCaptureDiagonalLeftlWin);
	m_isPossibleCaptureWinFunc.push_back(&isPossibleCaptureDiagonalRightWin);

	m_checkWinFunc.push_back(&Checker::checkWinHorizontal);
	m_checkWinFunc.push_back(&Checker::checkWinVertical);
	m_checkWinFunc.push_back(&Checker::checkWinDiagonalLeft);
	m_checkWinFunc.push_back(&Checker::checkWinDiagonalRight);
}

Checker::~Checker()
{
	m_checkWinFunc.clear();
	m_isPossibleCaptureWinFunc.clear();
}

void 		Checker::moveChecking(Move & currentMove, int x, int y)
{
	auto & gamefield = currentMove.getGameFieldMod();
	if (gamefield[y][x] != EMPTY){
		currentMove.setResult(NON_EMPTY);
	} else if (checkWin(currentMove, x, y)){
		redefineGameArea(currentMove, gamefield, x ,y);
		currentMove.setResult(WIN);
	} else 	if (checkCapture(currentMove, x, y)){
		redefineGameArea(currentMove, gamefield, x, y);
		currentMove.setResult((currentMove.getWhiteCapture() >= 5 || currentMove.getBlackCapture() >= 5) ? WIN : CAPTURE);
	} else if (checkFreeTree(currentMove, x, y)){
		currentMove.setResult(DOUBLE_FREE_TREE);
	} else {
		redefineGameArea(currentMove, gamefield, x, y);
		currentMove.setResult(DEF);
	}
}


void		Checker::eraseTiles(std::array<typeArr, N> &gameField, int startX, int startY, int endX, int endY)
{
	if (startX < 0 || startX > 17 || endX > 17
		|| startY < 0 || startY > 17 || endY > 17 )
		return;
	if (startX == endX){
		for (int i = startY; i <= endY; ++i) {
			gameField[i][startX] = EMPTY;
		}
		return;
	}

	if (startY == endY){
		for (int i = startX; i <= endX; ++i) {
			gameField[startY][i] = EMPTY;
		}
		return;
	}
	int j = startX;
	for (int i = startY; i <= endY; ++i) {
			gameField[i][j] = EMPTY;
			if (startX < endX)
				j++;
			else
				j--;
	}
}

void 	Checker::redefineGameArea(Move & currentMove, std::array<typeArr, N> & gamefield, int x, int y)
{
	t_pos & leftTop = const_cast<t_pos &>(currentMove.getLeftTop());
	t_pos & rightBottom = const_cast<t_pos &>(currentMove.getRightBottom());
	if (leftTop.x == -1) {
		leftTop.x = x;
		leftTop.y = y;
		rightBottom.x = x;
		rightBottom.y = y;
	} else {
		if (x < leftTop.x)
			leftTop.x = x;
		if (y < leftTop.y)
			leftTop.y = y;
		if (x > rightBottom.x)
			rightBottom.x = x;
		if (y > rightBottom.y)
			rightBottom.y = y;
	}
	gamefield[y][x] = currentMove.getCurrentType();
}

int 		Checker::checkWin(Move & currentMove, int x, int y)
{
	for (size_t i = 0; i < m_checkWinFunc.size(); ++i) {
		if ((this->*m_checkWinFunc[i])(currentMove, x, y))
			return 1;
	}
	return 0;
}


int 		Checker::checkWinHorizontal(Move & currentMove, int x, int y)
{
	int match = 1;
	for (int i = 1; i < 5; ++i) {
		int posX = x + i;
		if (posX > 17 || (currentMove.getGameField()[y])[posX] != currentMove.getCurrentType())
			break;
		if (checkPossibleCaptureWin(currentMove, posX, y))
			break;
		if (++match == 5)
			return 1;
	}
	for (int i = 1; i < 5 ; ++i) {
		int posX = x - i;
		if (posX < 0 || (currentMove.getGameField()[y])[posX] != currentMove.getCurrentType())
			break;
		if (checkPossibleCaptureWin(currentMove, posX, y))
			break;
		if (++match == 5)
			return 1;
	}
	return 0;
}

int			Checker::checkWinVertical(Move & currentMove, int x, int y)
{
	int match = 1;

	for (int i = 1; i < 5; ++i) {
		int posY = y + i;
		if (posY > 17 || (currentMove.getGameField()[posY])[x] != currentMove.getCurrentType())
			break;
		if (checkPossibleCaptureWin(currentMove, x, posY))
			break;
		if (++match == 5)
			return 1;
	}
	for (int i = 1; i < 5 ; ++i) {
		int posY = y - i;
		if (posY < 0 || (currentMove.getGameField()[posY])[x] != currentMove.getCurrentType())
			break;
		if (checkPossibleCaptureWin(currentMove, x, posY))
			break;
		if (++match == 5)
			return 1;
	}
	return 0;

}

int 		Checker::checkWinDiagonalLeft(Move & currentMove, int x, int y)
{
	int match = 1;

	for (int i = 1; i < 5; ++i) {
		int posX = x + i;
		int posY = y + i;
		if (posX > 17 || posY > 17 || (currentMove.getGameField()[posY])[posX] != currentMove.getCurrentType())
			break;
		if (checkPossibleCaptureWin(currentMove, posX, posY))
			break;
		if (++match == 5)
			return 1;
	}
	for (int i = 1; i < 5 ; ++i){
		int posX = x - i;
		int posY = y - i;
		if (posX < 0 || posY < 0 || (currentMove.getGameField()[posY])[posX] != currentMove.getCurrentType())
			break;
		if (checkPossibleCaptureWin(currentMove, posX, posY))
			break;
		if (++match == 5)
			return 1;
	}
	return 0;
}

int 		Checker::checkWinDiagonalRight(Move & currentMove, int x, int y)
{
	int match = 1;

	for (int i = 1; i < 5; ++i) {
		int posX = x - i;
		int posY = y + i;
		if (posX < 0 || posY > 17 || (currentMove.getGameField()[posY])[posX] != currentMove.getCurrentType())
			break;
		if (checkPossibleCaptureWin(currentMove, posX, posY))
			break;
		if (++match == 5)
			return 1;
	}
	for (int i = 1; i < 5 ; ++i){
		int posX = x + i;
		int posY = y - i;
		if (posX > 17 || posY < 0 || (currentMove.getGameField()[posY])[posX] != currentMove.getCurrentType())
			break;
		if (checkPossibleCaptureWin(currentMove, posX, posY))
			break;
		if (++match == 5)
			return 1;
	}
	return 0;
}


int 		Checker::checkPossibleCaptureWin(Move & currentMove, int x, int y)
{
	int winAllignmentIsCaptured = std::count_if(m_isPossibleCaptureWinFunc.begin(),
													m_isPossibleCaptureWinFunc.end(),
													[&](captureOnWinFunc *func)->bool
														{
															return (*func)(currentMove, x, y);
														});
	return winAllignmentIsCaptured;
}





int 		Checker::checkCapture(Move & currentMove, int x, int y)
{
	int captures = 0;
	captures += checkCaptureHorizontal(currentMove, x, y);
	captures += checkCaptureVertical(currentMove, x, y);
	captures += checkCaptureDiagonalLeft(currentMove, x, y);
	captures += checkCaptureDiagonalRight(currentMove, x, y);
	if (currentMove.getCurrentType() == WHITE)
		currentMove.setWhiteCapture(currentMove.getWhiteCapture() + captures);
	else
	 	currentMove.setBlackCapture (currentMove.getBlackCapture() + captures);
	return captures;
}

int 		Checker::checkCaptureHorizontal(Move & currentMove, int x, int y)
{
	int capture = 0;
	int cursor = 0;
	for (cursor = 1; cursor < 4; ++cursor) {
		int posX = x + cursor;
		if (cursor < 3
			&& (posX > 17 || (currentMove.getGameField()[y])[posX] != findOppositeType(currentMove.getCurrentType())))
				break;
		if (cursor == 3
			 &&	(posX > 17 || (currentMove.getGameField()[y])[posX] != currentMove.getCurrentType()))
				break;
	}
	if (cursor == 4){
		eraseTiles(currentMove.getGameFieldMod(), x + 1 , y, x + 2, y);
		capture++;
	}
	for (cursor = 1; cursor < 4; ++cursor) {
		int posX = x - cursor;
		if (cursor < 3
			&& (posX < 0 || (currentMove.getGameField()[y])[posX] != findOppositeType(currentMove.getCurrentType())))
				break;
		if (cursor == 3
			&& (posX < 0 || (currentMove.getGameField()[y])[posX] != currentMove.getCurrentType()))
				break;
	}
	if (cursor == 4){
		eraseTiles(currentMove.getGameFieldMod(), x - 2 , y, x -1, y);
		capture++;
	}
	return capture;
}

int 		Checker::checkCaptureVertical(Move & currentMove, int x, int y)
{
	int capture = 0;
	int cursor = 0;
	for (cursor = 1; cursor < 4; ++cursor) {
		int posY = y + cursor;
		if (cursor < 3
			&& (posY > 17 || (currentMove.getGameField()[posY])[x] != findOppositeType(currentMove.getCurrentType())))
				break;
		if (cursor == 3
			 &&	(posY > 17 || (currentMove.getGameField()[posY])[x] != currentMove.getCurrentType()))
				break;
	}
	if (cursor == 4){
		eraseTiles(currentMove.getGameFieldMod(), x , y + 1, x , y + 2);
		capture++;
	}
	for (cursor = 1; cursor < 4; ++cursor) {
		int posY = y - cursor;
		if (cursor < 3
			&& (posY < 0 || (currentMove.getGameField()[posY])[x] != findOppositeType(currentMove.getCurrentType())))
				break;
		if (cursor == 3
			&& (posY < 0 || (currentMove.getGameField()[posY])[x] != currentMove.getCurrentType()))
				break;
	}
	if (cursor == 4){
		eraseTiles(currentMove.getGameFieldMod(), x , y - 2, x, y - 1);
		capture++;
	}
	return capture;
}

int 		Checker::checkCaptureDiagonalLeft(Move & currentMove, int x, int y)
{
	int capture = 0;
	int cursor = 0;
	for (cursor = 1; cursor < 4; ++cursor) {
		int posX = x + cursor;
		int posY = y + cursor;
		if (cursor < 3
			&& (posY > 17 || posX > 17 || (currentMove.getGameField()[posY])[posX] != findOppositeType(currentMove.getCurrentType())))
				break;
		if (cursor == 3
			 &&	(posY > 17 || posX > 17 || (currentMove.getGameField()[posY])[posX] != currentMove.getCurrentType()))
				break;
	}
	if (cursor == 4){
		eraseTiles(currentMove.getGameFieldMod(), x + 1 , y + 1, x + 2 , y + 2);
		capture++;
	}
	for (cursor = 1; cursor < 4; ++cursor) {
		int posX = x - cursor;
		int posY = y - cursor;
		if (cursor < 3
			&& (posY < 0 || posX  < 0 || (currentMove.getGameField()[posY])[posX] != findOppositeType(currentMove.getCurrentType())))
				break;
		if (cursor == 3
			&& (posY < 0 || posX  < 0 || (currentMove.getGameField()[posY])[posX] != currentMove.getCurrentType()))
				break;
	}
	if (cursor == 4){
		eraseTiles(currentMove.getGameFieldMod(), x - 2 , y - 2, x - 1, y - 1);
		capture++;
	}
	return capture;
}

int 		Checker::checkCaptureDiagonalRight(Move & currentMove, int x, int y)
{
	int capture = 0;
	int cursor = 0;
	for (cursor = 1; cursor < 4; ++cursor) {
		int posX = x - cursor;
		int posY = y + cursor;
		if (cursor < 3
			&& (posY > 17 || posX < 0 || (currentMove.getGameField()[posY])[posX] != findOppositeType(currentMove.getCurrentType())))
				break;
		if (cursor == 3
			 &&	(posY > 17 || posX < 0 || (currentMove.getGameField()[posY])[posX] != currentMove.getCurrentType()))
				break;
	}
	if (cursor == 4){
		eraseTiles(currentMove.getGameFieldMod(), x - 1 , y + 1, x - 2 , y + 2);
		capture++;
	}
	for (cursor = 1; cursor < 4; ++cursor) {
		int posX = x + cursor;
		int posY = y - cursor;
		if (cursor < 3
			&& (posY < 0 || posX  > 17 || (currentMove.getGameField()[posY])[posX] != findOppositeType(currentMove.getCurrentType())))
				break;
		if (cursor == 3
			&& (posY < 0 || posX  > 17 || (currentMove.getGameField()[posY])[posX] != currentMove.getCurrentType()))
				break;
	}
	if (cursor == 4){
		eraseTiles(currentMove.getGameFieldMod(), x + 2 , y - 2, x  + 1, y - 1);
		capture++;
	}
	return capture;
}

int		Checker::checkFreeTree(Move & currentMove, int x, int y)
{
	int free_tree = 0;
	free_tree += checkFreeTreeHorizontal(currentMove, x, y);
	free_tree += checkFreeTreeVertical(currentMove, x, y);
	free_tree += checkFreeTreeDiagonalLeft(currentMove, x, y);
	free_tree +=checkFreeTreeDiagonalRight(currentMove, x, y);
	if (free_tree > 1) {
		return 1;
	}  else if (free_tree == 1) {
		return checkDoubleFreeTree(currentMove);
	}
	return 0;
}

int		Checker::checkDoubleFreeTree(Move &currentMove)
{
	int doubleFreeThree = 0;
	std::array<typeArr, N>  & gamefield = currentMove.getGameFieldMod();
	for (int x = 0; x < 18; ++x){
		for (int y = 0; y < 18; ++y) {
			if (gamefield[y][x] != (currentMove.getCurrentType()))//{
				continue;
			doubleFreeThree += checkFreeTreeHorizontal(currentMove, x, y);
			doubleFreeThree += checkFreeTreeVertical(currentMove, x, y);
			doubleFreeThree += checkFreeTreeDiagonalLeft(currentMove, x, y);
			doubleFreeThree += checkFreeTreeDiagonalRight(currentMove, x, y);
			if (doubleFreeThree){
				return 1;
			}
		}
	}
	return 0;
}

 int 				Checker::validMatchFreeTreeSet(posSet const & left, posSet const & right)
{
	int match = 0;
	eType matcType = BLACK;

	for (size_t i = 0; i < left.size(); ++i){
		if (match >= 2){
			return 1;
		}
		for (size_t j = 0; j < right.size(); ++j)
		{
			if (left[i].x == right[j].x && left[i].y == right[j].y && left[i].type == right[j].type){
				matcType = left[i].type;
				match++;
			}
		}
	}

	if (match == 1 && matcType == EMPTY){
		return 1;
	}
	return 0;
}

int		Checker::validFreeThreeHorisontal(Move & currentMove, t_pos & start, t_pos & end,
													int y)
{
	auto  & gamefield = currentMove.getGameFieldMod();
	if (end.x + 1 > 17 || start.x - 1 < 0 || end.x - start.x > 3)
		return 0;
	if (gamefield[y][end.x + 1] != EMPTY)
		return 0;
	if  (gamefield[y][start.x - 1] != EMPTY)
	{
		if  (gamefield[y][start.x - 1] == currentMove.getCurrentType()
				&& start.x - 2 >= 0 && gamefield[y][start.x - 2] == EMPTY
				&& (gamefield[y][start.x + 2] == EMPTY || gamefield[y][start.x + 1] == EMPTY))
		{
			start.x--;
			end.x--;
		}
		 else
			return 0;
	}
	return 1;
}

int		Checker::validFreeThreeVertical(Move & currentMove, t_pos & start, t_pos & end,
													int x)
{
	if (end.y + 1 > 17 || start.y - 1 < 0 || end.y - start.y > 3)
		return 0;
	auto  & gamefield = currentMove.getGameFieldMod();
	if (gamefield[end.y + 1][x] != EMPTY)
		return 0;
	if  (gamefield[start.y - 1][x] != EMPTY){
		if  (gamefield[start.y - 1][x] == currentMove.getCurrentType()
				&& start.y - 2 >= 0 && gamefield[start.y - 2][x] == EMPTY
					&& (gamefield[start.y + 2][x] == EMPTY || gamefield[start.y + 1][x] == EMPTY)) {
			start.y--;
			end.y--;
		}
		 else
			return 0;
		}
	return 1;
}

int		Checker::validFreeThreeDiagonalLeft(Move & currentMove, t_pos & start, t_pos & end)
{
	if (end.y + 1 > 17 || start.y - 1 < 0 || end.y - start.y > 3)
		return 0;
	if (end.x + 1 > 17 || start.x - 1 < 0 || end.x - start.x > 3)
		return 0;
	auto & gamefield = currentMove.getGameFieldMod();

	if (gamefield[end.y + 1][end.x + 1] != EMPTY)
		return 0;
	if  (gamefield[start.y - 1][start.x - 1] != EMPTY)
	{
		if  (gamefield[start.y - 1][start.x - 1] == currentMove.getCurrentType()
				&& start.y - 2 > 0 && start.x - 2 > 0 && gamefield[start.y - 2][start.x - 2] == EMPTY
					&& (gamefield[start.y + 2][start.x + 2] == EMPTY || gamefield[start.y + 1][start.x + 1] == EMPTY)){
			start.y--;
			end.y--;
			start.x--;
			end.x--;
		}
		 else
			return 0;
	}
	return 1;
}

int		Checker::validFreeThreeDiagonalRight(Move & currentMove, t_pos & start, t_pos & end)
{
	if (end.y + 1 > 17 || start.y - 1 < 0 || end.y - start.y > 3)
		return 0;
	if (end.x - 1 < 0 || start.x + 1 > 17 || end.x - start.x > 3)
		return 0;
	std::array<typeArr, N>  & gamefield = currentMove.getGameFieldMod();
	if (gamefield[end.y + 1][end.x - 1] != EMPTY)
		return 0;
	if  (gamefield[start.y - 1][start.x + 1] != EMPTY){
		if  (gamefield[start.y - 1][start.x + 1] == currentMove.getCurrentType()
				&& start.y - 2 > 0 && start.x + 2 < 17 && gamefield[start.y - 2][start.x + 2] == EMPTY
					&& (gamefield[start.y + 2][start.x - 2] == EMPTY || gamefield[start.y + 1][start.x - 1] == EMPTY)){
			start.y--;
			end.y--;
			start.x++;
			end.x++;
		}
		 else
			return 0;
	}
	return 1;
}

int		Checker::fillThreeSet(Move & currentMove, posSet const & examinedSet)
{
	if (currentMove.getCurrentType() == BLACK) {
		if (currentMove.getBlackFreeThree().empty())
			currentMove.setBlackFreeThree(examinedSet);
		else
			return validMatchFreeTreeSet(currentMove.getBlackFreeThree(), examinedSet) ? 0 : 1;
	} else if ( currentMove.getCurrentType() == WHITE){
		if (currentMove.getWhiteFreeThree().empty())
			currentMove.setWhiteFreeThree(examinedSet);
		else
			return validMatchFreeTreeSet(currentMove.getWhiteFreeThree(), examinedSet) ? 0 : 1;
	}
	return 1;
}


int 		Checker::fillHorisontalFreeTreeSet(Move & currentMove, int x, int y, t_pos const & start, t_pos const & end)
{
	posSet freeTreeSet;

	for (int i = start.x - 1; i <= end.x + 1; ++i)
	{
		t_pos tile;
		tile.x = i;
		tile.y = y;
		if (tile.x == x && tile.y == y)
			tile.type = currentMove.getCurrentType();
		else
			tile.type = (eType)(currentMove.getGameField()[tile.y])[tile.x];
		freeTreeSet.push_back(tile);
	}
	return fillThreeSet(currentMove, freeTreeSet);
}

int 		Checker::fillVerticalFreeTreeSet(Move & currentMove, int x, int y, t_pos const & start, t_pos const & end)
{
	posSet freeTreeSet;
	for (int i = start.y - 1; i <= end.y + 1; ++i)
	{
		t_pos tile;
		tile.x = x;
		tile.y = i;
		if (tile.x == x && tile.y == y)
			tile.type = currentMove.getCurrentType();
		else
			tile.type = (eType)(currentMove.getGameField()[tile.y])[tile.x];
		freeTreeSet.push_back(tile);
	}
	return fillThreeSet(currentMove, freeTreeSet);
}

int		Checker::fillDiagonalLeftFreeTreeSet(Move & currentMove, int x, int y, t_pos const & start, t_pos const & end)
{
	posSet freeTreeSet;
	int size = end.y - start.y + 2;
	for (int i = 0; i < size; ++i)
	{
		t_pos tile;
		tile.x = start.x + i - 1;
		tile.y = start.y + i - 1;
		if (tile.x == x && tile.y == y)
			tile.type = currentMove.getCurrentType();
		else
			tile.type = (eType)(currentMove.getGameField()[tile.y])[tile.x];
		freeTreeSet.push_back(tile);
	}
	return fillThreeSet(currentMove, freeTreeSet);
}

int 		Checker::fillDiagonalRightFreeTreeSet(Move & currentMove, int x, int y, t_pos const & start, t_pos const & end)
{
	posSet freeTreeSet;
	int size = end.y - start.y + 2;
	for (int i = 0; i < size; ++i)
	{
		t_pos tile;
		tile.x = start.x - i + 1;
		tile.y = start.y + i - 1;
		if (tile.x == x && tile.y == y)
			tile.type = currentMove.getCurrentType();
		else
			tile.type = (eType)(currentMove.getGameField()[tile.y])[tile.x];
		freeTreeSet.push_back(tile);
	}
	return fillThreeSet(currentMove, freeTreeSet);
}

int 		Checker::checkFreeTreeHorizontal(Move & currentMove, int x, int y)
{
	int matchCurrentType = 1;
	int matchEmpty = 0;
	t_pos start;
	t_pos end;
	start.x = x;
	start.y = y;
	end.y = y;
	end.x = x;

	for (int i = 1; i < 4; ++i) {
		int posX = x + i;
		if (posX > 17 || (currentMove.getGameField()[y])[posX] == findOppositeType(currentMove.getCurrentType()))
			break;
		if ((currentMove.getGameField()[y])[posX] == currentMove.getCurrentType()) {
			end.x = posX;
			matchCurrentType++;
		} else {
			matchEmpty++;
		}
		if (matchEmpty == 2) {
			break;
		}
		if(matchCurrentType == 3){
			if (!validFreeThreeHorisontal(currentMove, start, end, y))
				break;
			return fillHorisontalFreeTreeSet(currentMove, x, y, start, end);
		}
	}
	matchEmpty = 0;
	for (int i = 1; i < 4 ; ++i) {
		int posX = x - i;
		if (posX < 0 ||  (currentMove.getGameField()[y])[posX] == findOppositeType(currentMove.getCurrentType()))
			break;

			if ((currentMove.getGameField()[y])[posX] == currentMove.getCurrentType()) {
				matchCurrentType++;
				start.x = posX;
			} else {
				matchEmpty++;
			}
			if (matchEmpty == 2) {
				break;
			}

		if (matchCurrentType == 3) {
			if (!validFreeThreeHorisontal(currentMove, start, end, y))
				break;
			return fillHorisontalFreeTreeSet(currentMove, x, y, start, end);
		}
	}
	return 0;
}

int 		Checker::checkFreeTreeVertical(Move & currentMove, int x, int y)
{
	int matchCurrentType = 1;
	int matchEmpty = 0;
	t_pos start;
	start.x = x;
	start.y = y;
	t_pos end;
	end.y = y;
	end.x = x;

	for (int i = 1; i < 4; ++i) {
		int posY = y + i;
		if (posY > 17 || (currentMove.getGameField()[posY])[x] == findOppositeType(currentMove.getCurrentType()))
			break;
		if ((currentMove.getGameField()[posY])[x] == currentMove.getCurrentType()) {
			end.y = posY;
			matchCurrentType++;
		} else {
			matchEmpty++;
		}
		if (matchEmpty == 2){
			break;
		}
		if(matchCurrentType == 3){
			if (!validFreeThreeVertical(currentMove, start, end, x))
				break;
			return fillVerticalFreeTreeSet(currentMove, x, y, start, end);
		}
	}
	matchEmpty = 0;
	for (int i = 1; i < 4 ; ++i) {
		int posY = y - i;
		if (posY < 0 ||  (currentMove.getGameField()[posY])[x] == findOppositeType(currentMove.getCurrentType()))
			break;

			if ((currentMove.getGameField()[posY])[x] == currentMove.getCurrentType()) {
				matchCurrentType++;
				start.y = posY;
			} else {
				matchEmpty++;
			}
			if (matchEmpty == 2){
				break;
			}

		if (matchCurrentType == 3) {
			if (!validFreeThreeVertical(currentMove, start, end, x))
				break;
			return fillVerticalFreeTreeSet(currentMove, x, y, start, end);
		}
	}
	return 0;
}

int 		Checker::checkFreeTreeDiagonalLeft(Move & currentMove, int x, int y)
{
	int matchCurrentType = 1;
	int matchEmpty = 0;
	t_pos start;
	start.x = x;
	start.y = y;
	t_pos end;
	end.y = y;
	end.x = x;

	for (int i = 1; i < 4; ++i) {
		int posY = y + i;
		int posX = x + i;
		if (posY > 17 || posX > 17 || (currentMove.getGameField()[posY])[posX] == findOppositeType(currentMove.getCurrentType()))
			break;
		if ((currentMove.getGameField()[posY])[posX] == currentMove.getCurrentType()) {
			end.y = posY;
			end.x = posX;
			matchCurrentType++;
		} else {
			matchEmpty++;
		}
		if (matchEmpty == 2){
			break;
		}
		if(matchCurrentType == 3){
			if (!validFreeThreeDiagonalLeft(currentMove, start, end))
				break;
			return fillDiagonalLeftFreeTreeSet(currentMove, x, y, start, end);
		}
	}
	matchEmpty = 0;
	for (int i = 1; i < 4 ; ++i) {
		int posY = y - i;
		int posX = x - i;
		if (posY < 0 ||  (currentMove.getGameField()[posY])[posX] == findOppositeType(currentMove.getCurrentType()))
			break;
			if ((currentMove.getGameField()[posY])[posX] == currentMove.getCurrentType()) {
				matchCurrentType++;
				start.y = posY;
				start.x = posX;
			} else {
				matchEmpty++;
			}
			if (matchEmpty == 2){
				break;
			}

		if (matchCurrentType == 3) {
			if (!validFreeThreeDiagonalLeft(currentMove, start, end))
				break;
			return fillDiagonalLeftFreeTreeSet(currentMove, x, y, start, end);
		}
	}
	return 0;
}

int 		Checker::checkFreeTreeDiagonalRight(Move & currentMove, int x, int y)
{
	int matchCurrentType = 1;
	int matchEmpty = 0;
	t_pos start;
	start.x = x;
	start.y = y;
	t_pos end;
	end.y = y;
	end.x = x;

	for (int i = 1; i < 4; ++i) {
		int posY = y + i;
		int posX = x - i;
		if (posY > 17 || posX < 0 || (currentMove.getGameField()[posY])[posX] == findOppositeType(currentMove.getCurrentType()))
			break;
		if ((currentMove.getGameField()[posY])[posX] == currentMove.getCurrentType()) {
			end.y = posY;
			end.x = posX;
			matchCurrentType++;
		} else {
			matchEmpty++;
		}
		if (matchEmpty == 2){
			break;
		}
		if(matchCurrentType == 3){
			if (!validFreeThreeDiagonalRight(currentMove, start, end))
				break;
			return fillDiagonalRightFreeTreeSet(currentMove, x, y, start, end);
		}
	}
	matchEmpty = 0;
	for (int i = 1; i < 4 ; ++i) {
		int posY = y - i;
		int posX = x + i;
		if (posY < 0 ||  (currentMove.getGameField()[posY])[posX] == findOppositeType(currentMove.getCurrentType()))
			break;
			if ((currentMove.getGameField()[posY])[posX] == currentMove.getCurrentType()) {
				matchCurrentType++;
				start.y = posY;
				start.x = posX;
			} else {
				matchEmpty++;
			}
			if (matchEmpty == 2){
				break;
			}

		if (matchCurrentType == 3) {
			if (!validFreeThreeDiagonalRight(currentMove, start, end))
				break;
			return fillDiagonalRightFreeTreeSet(currentMove, x, y, start, end);
		}
	}
	return 0;
}
