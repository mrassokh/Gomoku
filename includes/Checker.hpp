/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Checker.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrassokh <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/24 15:07:17 by mrassokh          #+#    #+#             */
/*   Updated: 2018/12/24 15:07:19 by mrassokh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHECKER_HPP
# define CHECKER_HPP
#include "Move.hpp"
#include <deque>
#include <algorithm>

typedef std::function< bool (const Move &, const int, const int) > captureOnWinFunc;

class Checker final{
public:
	Checker();
	~Checker();

	void 			 	moveChecking(Move & currentMove, int x, int y);

private:
	Checker(Checker const & rhs) = delete;
	Checker& operator = (Checker const & rhs) = delete;

	 inline void	eraseTiles(std::array<typeArr, N> & gameField, int startX, int startY, int endX, int endY);
	 void 			redefineGameArea(Move & currentMove, std::array<typeArr, N> & gamefield, int x, int y);

	 int 			checkWin(Move & currentMove, int x, int y);
	 int 			checkWinHorizontal(Move & currentMove, int x, int y);
	 int 			checkWinVertical(Move & currentMove, int x, int y);
	 int 			checkWinDiagonalLeft(Move & currentMove, int x, int y);
	 int 			checkWinDiagonalRight(Move & currentMove, int x, int y);

	 int 			checkPossibleCaptureWin(Move & currentMove, int x, int y);

	 int 			checkCapture(Move & currentMove, int x, int y);
	 int 			checkCaptureHorizontal(Move & currentMove, int x, int y);
	 int 			checkCaptureVertical(Move & currentMove, int x, int y);
	 int 			checkCaptureDiagonalLeft(Move & currentMove, int x, int y);
	 int 			checkCaptureDiagonalRight(Move & currentMove, int x, int y);

	 int 			checkFreeTree(Move & currentMove, int x, int y);
	 int			checkDoubleFreeTree(Move &currentMove);

	 int 			validMatchFreeTreeSet(posSet const & left, posSet const & right);
	 int			validFreeThreeHorisontal(Move & currentMove, t_pos & start, t_pos & end, int y);
	 int			validFreeThreeVertical(Move & currentMove, t_pos & start, t_pos & end, int x);
	 int			validFreeThreeDiagonalLeft(Move & currentMove, t_pos & start, t_pos & end);
	 int			validFreeThreeDiagonalRight(Move & currentMove, t_pos & start, t_pos & end);

	 inline int		fillThreeSet(Move & currentMove, posSet const & examinedSet);
	 inline int 	fillHorisontalFreeTreeSet(Move & currentMove, int x, int y,
		 								t_pos const & start, t_pos const & end);
	 inline int 	fillVerticalFreeTreeSet(Move & currentMove, int x, int y,
		 								t_pos const & start, t_pos const & end);
	 inline int 	fillDiagonalLeftFreeTreeSet(Move & currentMove, int x, int y,
		  									t_pos const & start, t_pos const & end);
	 inline int		fillDiagonalRightFreeTreeSet(Move & currentMove, int x, int y,
		 									t_pos const & start, t_pos const & end);

	 int 			checkFreeTreeHorizontal(Move & currentMove, int x, int y);
	 int 			checkFreeTreeVertical(Move & currentMove, int x, int y);
	 int 			checkFreeTreeDiagonalLeft(Move & currentMove, int x, int y);
	 int 			checkFreeTreeDiagonalRight(Move & currentMove, int x, int y);

	std::deque<captureOnWinFunc *> 						m_isPossibleCaptureWinFunc;
	std::deque<int (Checker::*)(Move &, int, int)> 		m_checkWinFunc;
};

static eType		findOppositeType(eType type) {
	return type == BLACK ? WHITE : (type == WHITE ? BLACK : EMPTY);
}


static std::function< bool (const Move &, const int, const int, const int , const int , const int , const int ,const int , const int ) > closeCapture
= [](Move const & currentMove, const int x, const int y, const int mult_X0, const int mult_Y0, const int mult_X1, const int mult_Y1,const int mult_X2, const int mult_Y2){
	if ((currentMove.getGameField()[y + mult_Y0])[x + mult_X0] == findOppositeType(currentMove.getCurrentType())
		&& (currentMove.getGameField()[y + mult_Y1])[x + mult_X1] == currentMove.getCurrentType()
		&& (currentMove.getGameField()[y + mult_Y2])[x + mult_X2] == EMPTY){
			return true;
	}
	return false;
};

static captureOnWinFunc isPossibleCaptureHorizontalWin = [](Move const & currentMove, const int posX, const int posY){
	int y = posY;

	if (y < 1 || y > 16)
		return false;
	if (y + 2 <= 17){
		if (closeCapture(currentMove, posX, y, 0, -1, 0, 1, 0, 2)
			|| closeCapture(currentMove, posX, y, 0, 2, 0, 1, 0, -1))
			return true;
	}

	if (y - 2 >= 0) {
		if (closeCapture(currentMove, posX, y, 0, 1, 0, -1, 0, -2)
			|| closeCapture(currentMove, posX, y, 0, -2, 0, -1, 0, 1))
			return true;
	}
	return false;
};


static captureOnWinFunc  isPossibleCaptureVerticalWin = [](Move const & currentMove, const int posX, const int posY){
	int x = posX;

	if (x < 1 || x > 16)
		return false;
	if (x + 2 <= 17){
		if (closeCapture(currentMove, x, posY, -1, 0, 1, 0, 2, 0)
			|| closeCapture(currentMove, x, posY, 2, 0, 1, 0, -1, 0))
			return true;
	}
	if (x - 2 >= 0) {
		if (closeCapture(currentMove, x, posY, 1, 0, -1, 0, -2, 0)
			|| closeCapture(currentMove, x, posY, -2, 0, -1, 0, 1, 0))
			return true;
	}
	return false;
};

static captureOnWinFunc  isPossibleCaptureDiagonalLeftlWin = [](Move const & currentMove, const int posX, const int posY){
	if (posX + 2 <= 17 && posY - 2 >= 0 && posX - 1 >=0 && posY + 1 <= 17)
	{
		if (closeCapture(currentMove, posX, posY, -1, 1, 1, -1, 2, -2)
			|| closeCapture(currentMove, posX, posY, 2, -2, 1, -1, -1, 1))
			return true;
	}

	if (posX + 1 <= 17 && posY - 1 >= 0 && posX - 2 >=0  && posY + 2 <= 17) {
		if (closeCapture(currentMove, posX, posY, 1, -1, -1, 1, -2, 2)
			|| closeCapture(currentMove, posX, posY, -2, 2, -1, 1, 1, -1))
			return true;
	}
	return false;
};

static captureOnWinFunc isPossibleCaptureDiagonalRightWin = [](Move const & currentMove, const int posX, const int posY){
	if (posX + 2 <= 17 && posY + 2 <= 17 && posX - 1 >=0 && posY - 1 >= 0) {
		if (closeCapture(currentMove, posX, posY, -1, -1, 1, 1, 2, 2)
			|| closeCapture(currentMove, posX, posY, 2, 2, 1, 1, -1, -1))
			return true;
	}
	if (posX + 1 <= 17 && posY + 1 <= 17 && posX - 2 >=0 && posY - 2 >= 0) {
		if (closeCapture(currentMove, posX, posY, 1, 1, -1, -1, -2, -2)
			|| closeCapture(currentMove, posX, posY, -2, -2, -1, -1, 1, 1))
			return true;
	}
	return false;
};

#endif
