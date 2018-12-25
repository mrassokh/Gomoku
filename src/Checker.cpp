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

void 		Checker::moveChecking(Move & currentMove, int x, int y)
{
	auto & gamefield = currentMove.getGameFieldMod();
	if (gamefield[y][x] != EMPTY){
		currentMove.setResult(NON_EMPTY);
		//printf("Push point with x =%d and y =%d denied - qquare is not empty\n", x, y);
	} else if (checkWin(currentMove, x, y)){
		//printf("Push point with x =%d and y =%d and win!\n", x, y);
		//gamefield[y][x] = m_currentMove.getCurrentType();
		redefineGameArea(currentMove, gamefield, x ,y);
		//redefineGameArea(x,y,gamefield,leftTop,rightBottom);
		currentMove.setResult(WIN);
	} else 	if (checkCapture(currentMove, x, y)){
		// gamefield[y][x] = m_currentMove.getCurrentType();

		redefineGameArea(currentMove, gamefield, x, y);
		currentMove.setResult((currentMove.getWhiteCapture() >= 5 || currentMove.getBlackCapture() >= 5) ? WIN : CAPTURE);
	} else if (checkFreeTree(currentMove, x, y)){
		currentMove.setResult(DOUBLE_FREE_TREE);
	} else {
		// gamefield[y][x] = m_currentMove.getCurrentType();
		// t_pos & leftTop = const_cast<t_pos &>(currentMove.getLeftTop());
		// t_pos & rightBottom = const_cast<t_pos &>(currentMove.getRightBottom());
		redefineGameArea(currentMove, gamefield, x, y);
		currentMove.setResult(DEF);
	}
}


int 			Checker::checkWin(Move & currentMove, int x, int y)
{

	if (checkWinHorizontal(currentMove, x, y))
		return 1;
	else if (checkWinVertical(currentMove, x, y))
		return 1;
	else if (checkWinDiagonalLeft(currentMove, x, y))
		return 1;
	else if (checkWinDiagonalRight(currentMove, x, y))
		return 1;
	return 0;
}

inline int 			Checker::checkPossibleCaptureHorizontalWin(Move & currentMove, int startX, int endX, int y)
{
	if (y < 1 || y > 16)
		return 1;
	eType const & currType = currentMove.getCurrentType();
	for (int i = startX; i < endX; ++i)
	{
		if (y + 2 <= 17
			&& (currentMove.getGameField()[y - 1])[i] == findOppositeType(currType)
			&& (currentMove.getGameField()[y + 1])[i] == currType
			&& (currentMove.getGameField()[y + 2])[i] == EMPTY){
			//printf("checkPossibleCaptureHorizontalWin for x =%d and y =%d \n", i, y);
			return 0;
		}
		if (y - 2 >= 0
			&& (currentMove.getGameField()[y + 1])[i] == findOppositeType(currType)
			&& (currentMove.getGameField()[y - 1])[i] == currType
			&& (currentMove.getGameField()[y - 2])[i] == EMPTY){
				//printf("checkPossibleCaptureHorizontalWin for x =%d and y =%d \n", i, y);
				return 0;
		}
	}
	return 1;
}

inline int 			Checker::checkPossibleCaptureVerticalWin(Move & currentMove, int startY, int endY, int x)
{
	if (x < 1 || x > 16)
		return 1;
	for (int i = startY; i < endY; ++i)
	{
		if (x + 2 <= 17
			&& (currentMove.getGameField()[i])[x - 1] == findOppositeType(currentMove.getCurrentType())
			&& (currentMove.getGameField()[i])[x + 1] == currentMove.getCurrentType()
			&& (currentMove.getGameField()[i])[x + 2] == EMPTY){
			//printf("checkPossibleCaptureVerticalWin for x =%d and x =%d \n", i, x);
			return 0;
		}
		if (x - 2 >= 0
			&& (currentMove.getGameField()[i])[x + 1] == findOppositeType(currentMove.getCurrentType())
			&& (currentMove.getGameField()[i])[x - 1] == currentMove.getCurrentType()
			&& (currentMove.getGameField()[i])[x - 2] == EMPTY){
				//printf("checkPossibleCaptureVerticalWin for x =%d and x =%d \n", i, x);
				return 0;
		}
	}
	return 1;
}

inline int 		Checker::checkPossibleCaptureDiagonalLeftWin(Move & currentMove, t_pos const & start)
{
	int posX = start.x;
	int posY = start.y;
	for (int i = 0; i < 5; ++i)
	{
		if (posX + 2 <= 17 && posY - 2 >= 0 && posX - 1 >=0 && posY + 1 <= 17
			&& (currentMove.getGameField()[posY + 1])[posX - 1] == findOppositeType(currentMove.getCurrentType())
			&& (currentMove.getGameField()[posY - 1])[posX + 1] == currentMove.getCurrentType()
			&& (currentMove.getGameField()[posY - 2])[posX + 2] == EMPTY) {
			//printf("checkPossibleCaptureDiagonalLeftWin for x =%d and x =%d \n", posX, posY);
			return 0;
		}

		if (posX + 1 <= 17 && posY - 1 >= 0 && posX - 2 >=0  && posY + 2 <= 17
			&& (currentMove.getGameField()[posY - 1])[posX + 1] == findOppositeType(currentMove.getCurrentType())
			&& (currentMove.getGameField()[posY + 1])[posX - 1] == currentMove.getCurrentType()
			&& (currentMove.getGameField()[posY + 2])[posX - 2] == EMPTY) {
			//printf("checkPossibleCaptureDiagonalLeftWin for x =%d and x =%d \n", posX, posY);
			return 0;
		}
		posX++;
		posY++;
	}
 	return 1;
}

void 					Checker::redefineGameArea(Move & currentMove, std::array<typeArr, N> & gamefield, int x, int y)
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

inline int 		Checker::checkPossibleCaptureDiagonalRightWin(Move & currentMove, t_pos const & start)
{
	int posX = start.x;
	int posY = start.y;
	for (int i = 0; i < 5; ++i)
	{
		if (posX + 2 <= 17 && posY + 2 <= 17 && posX - 1 >=0 && posY - 1 >= 0
			&& (currentMove.getGameField()[posY - 1])[posX - 1] == findOppositeType(currentMove.getCurrentType())
			&& (currentMove.getGameField()[posY + 1])[posX + 1] == currentMove.getCurrentType()
			&& (currentMove.getGameField()[posY + 2])[posX + 2] == EMPTY) {
			//printf("checkPossibleCaptureDiagonalLeftWin for x =%d and x =%d \n", posX, posY);
			return 0;
		}
		if (posX + 1 <= 17 && posY + 1 <= 17 && posX - 2 >=0 && posY - 2 >= 0
			&& (currentMove.getGameField()[posY + 1])[posX + 1] == findOppositeType(currentMove.getCurrentType())
			&& (currentMove.getGameField()[posY - 1])[posX - 1] == currentMove.getCurrentType()
			&& (currentMove.getGameField()[posY - 2])[posX - 2] == EMPTY) {
			//printf("checkPossibleCaptureDiagonalLeftWin for x =%d and x =%d \n", posX, posY);
			return 0;
		}
		posX--;
		posY++;
	}
 	return 1;
}


int 			Checker::checkWinHorizontal(Move & currentMove, int x, int y)
{
		//printf("checkWinHorizontalx =%d and y =%d to win\n", x, y );
	int match = 1;
	int startX = x;
	int endX = x;
	for (int i = 1; i < 5; ++i) {
		int posX = x + i;
		if (posX > 17 || (currentMove.getGameField()[y])[posX] != currentMove.getCurrentType())
			break;
		else if(++match == 5){
			endX = posX;
			//printf("match for x =%d and y =%d is %d\n", x, y, match );
			return checkPossibleCaptureHorizontalWin(currentMove, startX, endX, y);
		}
		//printf("match for x =%d and y =%d is %d\n", x, y, match );
	}
	for (int i = 1; i < 5 ; ++i) {
		int posX = x - i;
		if (posX < 0 || (currentMove.getGameField()[y])[posX] != currentMove.getCurrentType())
			break;
		else if(++match == 5){
			startX = posX;
			//printf("match for x =%d and y =%d is %d\n", x, y, match );
			return checkPossibleCaptureHorizontalWin(currentMove, startX, endX, y);
		}
		//printf("match for x =%d and y =%d is %d\n", x, y, match );
	}
	return 0;
}



int 			Checker::checkWinVertical(Move & currentMove, int x, int y)
{
		//printf("checkWinVertical x =%d and y =%d to win\n", x, y );
	int match = 1;
	int startY = y;
	int endY = y;
	for (int i = 1; i < 5; ++i) {
		int posY = y + i;
		if (posY > 17 || (currentMove.getGameField()[posY])[x] != currentMove.getCurrentType())
			break;
		else if(++match == 5) {
			endY = posY;
			return checkPossibleCaptureVerticalWin(currentMove, startY,endY, x);
		}
	}
	for (int i = 1; i < 5 ; ++i){
		int posY = y - i;
		if (posY < 0 || (currentMove.getGameField()[posY])[x] != currentMove.getCurrentType())
			break;
		else if (++match == 5){
			startY = posY;
			return checkPossibleCaptureVerticalWin(currentMove, startY,endY, x);
		}
	}
	return 0;

}

int 			Checker::checkWinDiagonalLeft(Move & currentMove, int x, int y)
{
	//printf("checkWinDiagonalLeft x =%d and y =%d to win\n", x, y );
	int match = 1;
	t_pos start;
	start.x = x;
	start.y = y;

	for (int i = 1; i < 5; ++i) {
		int posX = x + i;
		int posY = y + i;
		if (posX > 17 || posY > 17 || (currentMove.getGameField()[posY])[posX] != currentMove.getCurrentType())
			break;
		else if (++match == 5) {
			return checkPossibleCaptureDiagonalLeftWin(currentMove, start);//1;
		}
	}
	for (int i = 1; i < 5 ; ++i){
		int posX = x - i;
		int posY = y - i;
		if (posX < 0 || posY < 0 || (currentMove.getGameField()[posY])[posX] != currentMove.getCurrentType())
			break;
		else if (++match == 5){
			start.x = posX;
			start.y = posY;
			return checkPossibleCaptureDiagonalLeftWin(currentMove, start);
		}
	}
	return 0;
}

int 			Checker::checkWinDiagonalRight(Move & currentMove, int x, int y)
{
	//printf("checkWinDiagonalRight x =%d and y =%d to win\n", x, y );
	int match = 1;
	t_pos start;
	start.x = x;
	start.y = y;
	for (int i = 1; i < 5; ++i) {
		int posX = x - i;
		int posY = y + i;
		if (posX < 0 || posY > 17 || (currentMove.getGameField()[posY])[posX] != currentMove.getCurrentType())
			break;
		else if(++match == 5) {
			return checkPossibleCaptureDiagonalRightWin(currentMove, start);
		}
	}
	for (int i = 1; i < 5 ; ++i){
		int posX = x + i;
		int posY = y - i;
		if (posX > 17 || posY < 0 || (currentMove.getGameField()[posY])[posX] != currentMove.getCurrentType())
			break;
		else if (++match == 5){
			start.x = posX;
			start.y = posY;
			return  checkPossibleCaptureDiagonalRightWin(currentMove, start);
		}
	}
	return 0;
}

eType			Checker::findOppositeType(eType type)
{
	return type == BLACK ? WHITE : (type == WHITE ? BLACK : EMPTY);
}

void			Checker::eraseTiles(std::array<typeArr, N> *gameField, int startX, int startY, int endX, int endY)
{
	//printf("ERASE TILE x =%d and y =%d x2 =%d and y2 =%d\n", startX, startY,  endX, endY);
	if (startX < 0 || startX > 17 || endX > 17
		|| startY < 0 || startY > 17 || endY > 17 )
		return;
	if (startX == endX){
		for (int i = startY; i <= endY; ++i) {
			//printf("ERASE TILE x =%d and y =%d\n", startX, i );
			(*gameField)[i][startX] = EMPTY;
		}
		return;
	}

	if (startY == endY){
		for (int i = startX; i <= endX; ++i) {
			//printf("ERASE TILE x =%d and y =%d\n", i, startY );
			(*gameField)[startY][i] = EMPTY;
		}
		return;
	}
	int j = startX;
	for (int i = startY; i <= endY; ++i) {
			//printf("ERASE TILE x =%d and y =%d\n", j, i );
			(*gameField)[i][j] = EMPTY;
			if (startX < endX)
				j++;
			else
				j--;
	}
}

int 			Checker::checkCapture(Move & currentMove, int x, int y)
{
	//printf("checkCapture x =%d and y =%d to win\n", x, y );
	int captures = 0;
	//printf("checkCaptureHorizontal x =%d and y =%d captures = %d\n", x, y, captures );
	captures += checkCaptureHorizontal(currentMove, x, y);
	//printf("checkCaptureVertical x =%d and y =%d captures = %d\n", x, y, captures );
	captures += checkCaptureVertical(currentMove, x, y);
	//printf("checkCaptureDiagonalLeft x =%d and y =%d captures = %d\n", x, y, captures );
	captures += checkCaptureDiagonalLeft(currentMove, x, y);
	//printf("checkCaptureDiagonalRight x =%d and y =%d captures = %d\n", x, y, captures );
	captures += checkCaptureDiagonalRight(currentMove, x, y);
	//printf("checkCapture x =%d and y =%d captures = %d\n", x, y, captures );
	if (currentMove.getCurrentType() == WHITE)
		currentMove.setWhiteCapture(currentMove.getWhiteCapture() + captures);
	else
	 	currentMove.setBlackCapture (currentMove.getBlackCapture() + captures);
	return captures;
}

int 			Checker::checkCaptureHorizontal(Move & currentMove, int x, int y)
{
	int capture = 0;
	int cursor = 0;
	for (cursor = 1; cursor < 4; ++cursor) {
		int posX = x + cursor;
	//	printf("Chek type for capture x =%d and y =%d\n type = %d current move = %d iteration = %d\n", posX, y, (int)findOppositeType(currentMove.getCurrentType()), (int)(currentMove.getGameField()[y])[posX], cursor);
		if (cursor < 3
			&& (posX > 17 || (currentMove.getGameField()[y])[posX] != findOppositeType(currentMove.getCurrentType())))
				break;
		if (cursor == 3
			 &&	(posX > 17 || (currentMove.getGameField()[y])[posX] != currentMove.getCurrentType()))
				break;
	}
	if (cursor == 4){
		//cursor = 0;
		//printf("Chek capture x =%d and y =%d\n", x, y );
		eraseTiles(&(currentMove.getGameFieldMod()), x + 1 , y, x + 2, y);
		capture++;
	}
	for (cursor = 1; cursor < 4; ++cursor) {
		int posX = x - cursor;
		//printf("Chek type for capture to left x =%d and y =%d\n type = %d current move = %d iteration = %d\n", posX, y, (int)findOppositeType(currentMove.getCurrentType()), (int)(currentMove.getGameField()[y])[posX], cursor);
		if (cursor < 3
			&& (posX < 0 || (currentMove.getGameField()[y])[posX] != findOppositeType(currentMove.getCurrentType())))
				break;
		if (cursor == 3
			&& (posX < 0 || (currentMove.getGameField()[y])[posX] != currentMove.getCurrentType()))
				break;
	}
	if (cursor == 4){
		//printf("Chek capture x =%d and y =%d\n", x, y );
		eraseTiles(&(currentMove.getGameFieldMod()), x - 2 , y, x -1, y);
		capture++;
	}
	return capture;
}

int 			Checker::checkCaptureVertical(Move & currentMove, int x, int y)
{
	int capture = 0;
	int cursor = 0;
	for (cursor = 1; cursor < 4; ++cursor) {
		int posY = y + cursor;
		//printf("Chek type for capture x =%d and y =%d\n type = %d current move = %d iteration = %d\n", posY, y, (int)findOppositeType(currentMove.getCurrentType()), (int)(currentMove.getGameField()[posY])[x], cursor);
		if (cursor < 3
			&& (posY > 17 || (currentMove.getGameField()[posY])[x] != findOppositeType(currentMove.getCurrentType())))
				break;
		if (cursor == 3
			 &&	(posY > 17 || (currentMove.getGameField()[posY])[x] != currentMove.getCurrentType()))
				break;
	}
	if (cursor == 4){
		//cursor = 0;
		//printf("Chek capture x =%d and y =%d\n", x, y );
		eraseTiles(&(currentMove.getGameFieldMod()), x , y + 1, x , y + 2);
		capture++;
	}
	for (cursor = 1; cursor < 4; ++cursor) {
		int posY = y - cursor;
		//printf("Chek type for capture to left x =%d and y =%d\n type = %d current move = %d iteration = %d\n", posX, y, (int)findOppositeType(currentMove.getCurrentType()), (int)(currentMove.getGameField()[y])[posX], cursor);
		if (cursor < 3
			&& (posY < 0 || (currentMove.getGameField()[posY])[x] != findOppositeType(currentMove.getCurrentType())))
				break;
		if (cursor == 3
			&& (posY < 0 || (currentMove.getGameField()[posY])[x] != currentMove.getCurrentType()))
				break;
	}
	if (cursor == 4){
		//printf("Chek capture x =%d and y =%d\n", x, y );
		eraseTiles(&(currentMove.getGameFieldMod()), x , y - 2, x, y - 1);
		capture++;
	}
	return capture;
}
//
int 			Checker::checkCaptureDiagonalLeft(Move & currentMove, int x, int y)
{
	int capture = 0;
	int cursor = 0;
	for (cursor = 1; cursor < 4; ++cursor) {
		int posX = x + cursor;
		int posY = y + cursor;
	//	printf("Chek type for capture x =%d and y =%d\n type = %d current move = %d iteration = %d\n", posX, posY, (int)findOppositeType(currentMove.getCurrentType()), (int)(currentMove.getGameField()[posY])[posX], cursor);
		if (cursor < 3
			&& (posY > 17 || posX > 17 || (currentMove.getGameField()[posY])[posX] != findOppositeType(currentMove.getCurrentType())))
				break;
		if (cursor == 3
			 &&	(posY > 17 || posX > 17 || (currentMove.getGameField()[posY])[posX] != currentMove.getCurrentType()))
				break;
	}
	if (cursor == 4){
		//cursor = 0;
		//printf("Chek capture x =%d and y =%d\n", x, y );
		eraseTiles(&(currentMove.getGameFieldMod()), x + 1 , y + 1, x + 2 , y + 2);
		capture++;
	}
	for (cursor = 1; cursor < 4; ++cursor) {
		int posX = x - cursor;
		int posY = y - cursor;
		//printf("Chek type for capture to left x =%d and y =%d\n type = %d current move = %d iteration = %d\n", posX, y, (int)findOppositeType(currentMove.getCurrentType()), (int)(currentMove.getGameField()[y])[posX], cursor);
		if (cursor < 3
			&& (posY < 0 || posX  < 0 || (currentMove.getGameField()[posY])[posX] != findOppositeType(currentMove.getCurrentType())))
				break;
		if (cursor == 3
			&& (posY < 0 || posX  < 0 || (currentMove.getGameField()[posY])[posX] != currentMove.getCurrentType()))
				break;
	}
	if (cursor == 4){
		//printf("Chek capture x =%d and y =%d\n", x, y );
		eraseTiles(&(currentMove.getGameFieldMod()), x - 2 , y - 2, x - 1, y - 1);
		capture++;
	}
	return capture;
}

int 			Checker::checkCaptureDiagonalRight(Move & currentMove, int x, int y)
{
	int capture = 0;
	int cursor = 0;
	for (cursor = 1; cursor < 4; ++cursor) {
		int posX = x - cursor;
		int posY = y + cursor;
	//	printf("Chek type for capture x =%d and y =%d\n type = %d current move = %d iteration = %d\n", posX, posY, (int)findOppositeType(currentMove.getCurrentType()), (int)(currentMove.getGameField()[posY])[posX], cursor);
		if (cursor < 3
			&& (posY > 17 || posX < 0 || (currentMove.getGameField()[posY])[posX] != findOppositeType(currentMove.getCurrentType())))
				break;
		if (cursor == 3
			 &&	(posY > 17 || posX < 0 || (currentMove.getGameField()[posY])[posX] != currentMove.getCurrentType()))
				break;
	}
	if (cursor == 4){
		//cursor = 0;
		//printf("Chek capture x =%d and y =%d\n", x, y );
		eraseTiles(&(currentMove.getGameFieldMod()), x - 1 , y + 1, x - 2 , y + 2);
		capture++;
	}
	for (cursor = 1; cursor < 4; ++cursor) {
		int posX = x + cursor;
		int posY = y - cursor;
		//printf("Chek type for capture to left x =%d and y =%d\n type = %d current move = %d iteration = %d\n", posX, y, (int)findOppositeType(currentMove.getCurrentType()), (int)(currentMove.getGameField()[y])[posX], cursor);
		if (cursor < 3
			&& (posY < 0 || posX  > 17 || (currentMove.getGameField()[posY])[posX] != findOppositeType(currentMove.getCurrentType())))
				break;
		if (cursor == 3
			&& (posY < 0 || posX  > 17 || (currentMove.getGameField()[posY])[posX] != currentMove.getCurrentType()))
				break;
	}
	if (cursor == 4){
		//printf("Chek capture x =%d and y =%d\n", x, y );
		eraseTiles(&(currentMove.getGameFieldMod()), x + 2 , y - 2, x  + 1, y - 1);
		capture++;
	}
	return capture;
}

int 			Checker::checkFreeTree(Move & currentMove, int x, int y)
{
	int free_tree = 0;
//printf("Chek free_tree x =%d and y =%d free_tree = %d\n", x, y, free_tree);
	free_tree += checkFreeTreeHorizontal(currentMove, x, y);
	free_tree += checkFreeTreeVertical(currentMove, x, y);
	free_tree += checkFreeTreeDiagonalLeft(currentMove, x, y);
	free_tree +=checkFreeTreeDiagonalRight(currentMove, x, y);
	//printf("Chek free_tree x =%d and y =%d free_tree = %d\n", x, y, free_tree);
	//printf("currentMove.whiteFreeTree.size() %lu\n", currentMove.whiteFreeTree.size());
	// for (size_t i = 0; i < currentMove.whiteFreeTree.size(); ++i) {
	// 	posSet set1 = (currentMove.whiteFreeTree)[i];
	// for (size_t j = 0; j < set1.size(); ++j)
	// {
	// 	printf("print whiteFreeTree\n");
	// 	t_pos tile =set1[j];
	// 	printf(" x =%d and y =%d is %d\n", tile.x, tile.y, tile.type);
	// }
	// }
	//printf("currentMove.blackFreeTree.size() %lu\n", currentMove.blackFreeTree.size());
	// for (size_t i = 0; i < currentMove.blackFreeTree.size(); ++i) {
	// 	posSet set = currentMove.blackFreeTree[i];
	// 	for (size_t j = 0; j < set.size(); ++j)
	// 	{
	// 		printf("print blackFreeTree\n");
	// 		t_pos tile = set[j];
	// 		printf(" x =%d and y =%d is %d\n", tile.x, tile.y, tile.type);
	// 	}
	// }
	if (free_tree > 1) {
		return 1;
	}  else if (free_tree == 1) {
		//printf ("checkcheckDoubleFreeTree current turn is %d\n", currentMove.getCurrentType());
		return checkDoubleFreeTree(currentMove);
	}
	return 0;
}

int			Checker::checkDoubleFreeTree(Move &currentMove)
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
			//printf ("x = %d y = %d doubleFreeThree = %d\n", x, y, doubleFreeThree);
			if (doubleFreeThree){
				//printf ("Double FREE THREE!!!!!\n");
				return 1;
			}
		}
	}
	return 0;
}
inline int		Checker::validFreeThreeHorisontal(Move & currentMove, t_pos & start, t_pos & end,
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

inline int		Checker::validFreeThreeVertical(Move & currentMove, t_pos & start, t_pos & end,
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

inline int		Checker::validFreeThreeDiagonalLeft(Move & currentMove, t_pos & start, t_pos & end)
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

inline int			Checker::validFreeThreeDiagonalRight(Move & currentMove, t_pos & start, t_pos & end)
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



std::function< bool (const t_pos & , const t_pos &) > comparator = [](const t_pos & left, const t_pos & right){
	return (left.x == right.x && left.y == right.y && left.type == right.type);
										// // Lambda function to compare 2 strings in case insensitive manner
										// return std::equal(left.begin(), left.end(), right.begin(), [](t_pos & l, t_pos & r) {
										// 															return (l.x == r.x && l.y == r.y && l.type == r.type));
										// 													});
									};

inline int 				Checker::validMatchFreeTreeSet(posSet const & left, posSet const & right)
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

inline int				Checker::fillThreeSet(Move & currentMove, posSet const & examinedSet)
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


int 				Checker::fillHorisontalFreeTreeSet(Move & currentMove, int x, int y, t_pos const & start, t_pos const & end)
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
		//
		// printf("pushTile start.x - 1 = %d; end.x + 1 = %d \n", start.x - 1 ,end.x + 1);
		// printf("pushTile tile->x = %d; tile->y = %d tile->type = %u\n", tile.x, tile.y, tile.type);
	}
	return fillThreeSet(currentMove, freeTreeSet);
}

int 			Checker::fillVerticalFreeTreeSet(Move & currentMove, int x, int y, t_pos const & start, t_pos const & end)
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

int				Checker::fillDiagonalLeftFreeTreeSet(Move & currentMove, int x, int y, t_pos const & start, t_pos const & end)
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

int 			Checker::fillDiagonalRightFreeTreeSet(Move & currentMove, int x, int y, t_pos const & start, t_pos const & end)
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

int 			Checker::checkFreeTreeHorizontal(Move & currentMove, int x, int y)
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

int 			Checker::checkFreeTreeVertical(Move & currentMove, int x, int y)
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

int 			Checker::checkFreeTreeDiagonalLeft(Move & currentMove, int x, int y)
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

int 			Checker::checkFreeTreeDiagonalRight(Move & currentMove, int x, int y)
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
