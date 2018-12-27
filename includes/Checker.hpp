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

enum eAlignmentType{
	HORIZONTAL,
	VERTICAL,
	LEFT_DIAGONAL,
	RIGHT_DIAGONAL
};

typedef std::function< bool (const Move &, const t_pos &) > captureOnWinFunc;

class Checker final{
public:
	Checker();
	~Checker();

	void 			 	moveChecking(Move & currentMove, int x, int y);

private:
	Checker(Checker const & rhs) = delete;
	Checker& operator = (Checker const & rhs) = delete;

	// inline eType		findOppositeType(eType type);

	inline void			eraseTiles(std::array<typeArr, N>	*gameField, int startX, int startY, int endX, int endY);
	inline void 		redefineGameArea(Move & currentMove, std::array<typeArr, N> & gamefield, int x, int y);

	inline int 			checkWin(Move & currentMove, int x, int y);
	inline int 			checkWinHorizontal(Move & currentMove, int x, int y);
	inline int 			checkWinVertical(Move & currentMove, int x, int y);
	inline int 			checkWinDiagonalLeft(Move & currentMove, int x, int y);
	inline int 			checkWinDiagonalRight(Move & currentMove, int x, int y);

	inline int 			checkPossibleCaptureOnWin(Move & currentMove, t_pos const & start, t_pos const & end, int x, int y);

	inline int 			checkPossibleCaptureHorizontalWin(Move & currentMove, int y, eAlignmentType alType);
//	inline int 			checkPossibleCaptureHorizontalWin(Move & currentMove, int startX, int endX, int y);
	inline int 			checkPossibleCaptureVerticalWin(Move & currentMove, int startY, int endY, int x);
	inline int 			checkPossibleCaptureDiagonalLeftWin(Move & currentMove, t_pos const & start);
	inline int 			checkPossibleCaptureDiagonalRightWin(Move & currentMove, t_pos const & start);

	inline int 			checkCapture(Move & currentMove, int x, int y);
	inline int 			checkCaptureHorizontal(Move & currentMove, int x, int y);
	inline int 			checkCaptureVertical(Move & currentMove, int x, int y);
	inline int 			checkCaptureDiagonalLeft(Move & currentMove, int x, int y);
	inline int 			checkCaptureDiagonalRight(Move & currentMove, int x, int y);
	inline int 			checkFreeTree(Move & currentMove, int x, int y);
	inline int			checkDoubleFreeTree(Move &currentMove);

	inline int			fillThreeSet(Move & currentMove, posSet const & examinedSet);
	inline int 			fillHorisontalFreeTreeSet(Move & currentMove, int x, int y, t_pos const & start, t_pos const & end);
	inline int 			fillVerticalFreeTreeSet(Move & currentMove, int x, int y, t_pos const & start, t_pos const & end);
	inline int 			fillDiagonalLeftFreeTreeSet(Move & currentMove, int x, int y, t_pos const & start, t_pos const & end);
	inline int 			fillDiagonalRightFreeTreeSet(Move & currentMove, int x, int y, t_pos const & start, t_pos const & end);

	inline int 			validMatchFreeTreeSet(posSet const & left, posSet const & right);
	inline int			validFreeThreeHorisontal(Move & currentMove, t_pos & start, t_pos & end, int y);
	inline int			validFreeThreeVertical(Move & currentMove, t_pos & start, t_pos & end, int x);
	inline int			validFreeThreeDiagonalLeft(Move & currentMove, t_pos & start, t_pos & end);
	inline int			validFreeThreeDiagonalRight(Move & currentMove, t_pos & start, t_pos & end);

	inline int 			checkFreeTreeHorizontal(Move & currentMove, int x, int y);
	inline int 			checkFreeTreeVertical(Move & currentMove, int x, int y);
	inline int 			checkFreeTreeDiagonalLeft(Move & currentMove, int x, int y);
	inline int 			checkFreeTreeDiagonalRight(Move & currentMove, int x, int y);

	std::deque<t_pos>				m_winAllignment;
	std::deque<captureOnWinFunc *> 	m_isPossibleCaptureWinFunc;

	//captureOnWinFunc	isPossibleCaptureHorizontalWin;
	//inline bool			isPossibleCaptureHorizontalWin(Move const & currentMove, const t_pos & match, const eType currType, const int y);


};

static eType		findOppositeType(eType type) {
	return type == BLACK ? WHITE : (type == WHITE ? BLACK : EMPTY);
}

static captureOnWinFunc isPossibleCaptureHorizontalWin = [](Move const & currentMove, const t_pos & match){
	int y = match.y;
	if (y + 2 <= 17
		&& (currentMove.getGameField()[y - 1])[match.x] == findOppositeType(currentMove.getCurrentType())
		&& (currentMove.getGameField()[y + 1])[match.x] == currentMove.getCurrentType()
		&& (currentMove.getGameField()[y + 2])[match.x] == EMPTY){
		//printf("checkPossibleCaptureHorizontalWin for x =%d and y =%d \n", i, y);
		return true;
	}
	if (y - 2 >= 0
		&& (currentMove.getGameField()[y + 1])[match.x] == findOppositeType(currentMove.getCurrentType())
		&& (currentMove.getGameField()[y - 1])[match.x] == currentMove.getCurrentType()
		&& (currentMove.getGameField()[y - 2])[match.x] == EMPTY){
			//printf("checkPossibleCaptureHorizontalWin for x =%d and y =%d \n", i, y);
			return true;
	}
	return false;
};


static captureOnWinFunc  isPossibleCaptureVerticalWin = [](Move const & currentMove, const t_pos & match){
	int x = match.x;
	if (x + 2 <= 17
		&& (currentMove.getGameField()[match.y])[x - 1] == findOppositeType(currentMove.getCurrentType())
		&& (currentMove.getGameField()[match.y])[x + 1] == currentMove.getCurrentType()
		&& (currentMove.getGameField()[match.y])[x + 2] == EMPTY){
		//printf("checkPossibleCaptureVerticalWin for x =%d and x =%d \n", i, x);
		return true;
	}
	if (x - 2 >= 0
		&& (currentMove.getGameField()[match.y])[x + 1] == findOppositeType(currentMove.getCurrentType())
		&& (currentMove.getGameField()[match.y])[x - 1] == currentMove.getCurrentType()
		&& (currentMove.getGameField()[match.y])[x - 2] == EMPTY){
			//printf("checkPossibleCaptureVerticalWin for x =%d and x =%d \n", i, x);
			return true;
	}
	return false;
};

static captureOnWinFunc  isPossibleCaptureDiagonalLeftlWin = [](Move const & currentMove, const t_pos & match){
	int posX = match.x;
	int posY = match.y;
	if (posX + 2 <= 17 && posY - 2 >= 0 && posX - 1 >=0 && posY + 1 <= 17
		&& (currentMove.getGameField()[posY + 1])[posX - 1] == findOppositeType(currentMove.getCurrentType())
		&& (currentMove.getGameField()[posY - 1])[posX + 1] == currentMove.getCurrentType()
		&& (currentMove.getGameField()[posY - 2])[posX + 2] == EMPTY) {
		//printf("checkPossibleCaptureDiagonalLeftWin for x =%d and x =%d \n", posX, posY);
		return true;
	}

	if (posX + 1 <= 17 && posY - 1 >= 0 && posX - 2 >=0  && posY + 2 <= 17
		&& (currentMove.getGameField()[posY - 1])[posX + 1] == findOppositeType(currentMove.getCurrentType())
		&& (currentMove.getGameField()[posY + 1])[posX - 1] == currentMove.getCurrentType()
		&& (currentMove.getGameField()[posY + 2])[posX - 2] == EMPTY) {
		//printf("checkPossibleCaptureDiagonalLeftWin for x =%d and x =%d \n", posX, posY);
		return true;
	}
	return false;
};

static captureOnWinFunc isPossibleCaptureDiagonalRightWin = [](Move const & currentMove, const t_pos & match){
	int posX = match.x;
	int posY = match.y;
	if (posX + 2 <= 17 && posY + 2 <= 17 && posX - 1 >=0 && posY - 1 >= 0
		&& (currentMove.getGameField()[posY - 1])[posX - 1] == findOppositeType(currentMove.getCurrentType())
		&& (currentMove.getGameField()[posY + 1])[posX + 1] == currentMove.getCurrentType()
		&& (currentMove.getGameField()[posY + 2])[posX + 2] == EMPTY) {
		//printf("checkPossibleCaptureDiagonalLeftWin for x =%d and x =%d \n", posX, posY);
		return true;
	}
	if (posX + 1 <= 17 && posY + 1 <= 17 && posX - 2 >=0 && posY - 2 >= 0
		&& (currentMove.getGameField()[posY + 1])[posX + 1] == findOppositeType(currentMove.getCurrentType())
		&& (currentMove.getGameField()[posY - 1])[posX - 1] == currentMove.getCurrentType()
		&& (currentMove.getGameField()[posY - 2])[posX - 2] == EMPTY) {
		//printf("checkPossibleCaptureDiagonalLeftWin for x =%d and x =%d \n", posX, posY);
		return true;
	}
	return false;
};


#endif
