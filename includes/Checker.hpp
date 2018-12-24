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

class Checker {
public:
	static Checker &	Instance();

	void 			 	moveChecking(MovePtr currentMove, int x, int y);

private:
	Checker(){};
	Checker(Checker const & rhs) = delete;
	Checker& operator = (Checker const & rhs) = delete;
	~Checker(){};

	inline eType		findOppositeType(eType type);
	inline void			eraseTiles(std::array<typeArr, N>	*gameField, int startX, int startY, int endX, int endY);
	inline void 			redefineGameArea(MovePtr currentMove, std::array<typeArr, N> & gamefield, int x, int y);

	inline int 			checkWin(MovePtr currentMove, int x, int y);
	inline int 			checkWinHorizontal(MovePtr currentMove, int x, int y);
	inline int 			checkWinVertical(MovePtr currentMove, int x, int y);
	inline int 			checkWinDiagonalLeft(MovePtr currentMove, int x, int y);
	inline int 			checkWinDiagonalRight(MovePtr currentMove, int x, int y);

	inline int 			checkPossibleCaptureHorizontalWin(MovePtr currentMove, int startX, int endX, int y);
	inline int 			checkPossibleCaptureVerticalWin(MovePtr currentMove, int startY, int endY, int x);
	inline int 			checkPossibleCaptureDiagonalLeftWin(MovePtr currentMove, t_pos const & start);
	inline int 			checkPossibleCaptureDiagonalRightWin(MovePtr currentMove, t_pos const & start);

	inline int 			checkCapture(MovePtr currentMove, int x, int y);
	inline int 			checkCaptureHorizontal(MovePtr currentMove, int x, int y);
	inline int 			checkCaptureVertical(MovePtr currentMove, int x, int y);
	inline int 			checkCaptureDiagonalLeft(MovePtr currentMove, int x, int y);
	inline int 			checkCaptureDiagonalRight(MovePtr currentMove, int x, int y);
	inline int 			checkFreeTree(MovePtr currentMove, int x, int y);
	inline int			checkDoubleFreeTree(Move &currentMove);

	inline int			fillThreeSet(Move & currentMove, posSet const & examinedSet);
	inline int 			fillHorisontalFreeTreeSet(MovePtr currentMove, int x, int y, t_pos const & start, t_pos const & end);
	inline int 			fillVerticalFreeTreeSet(MovePtr currentMove, int x, int y, t_pos const & start, t_pos const & end);
	inline int 			fillDiagonalLeftFreeTreeSet(MovePtr currentMove, int x, int y, t_pos const & start, t_pos const & end);
	inline int 			fillDiagonalRightFreeTreeSet(MovePtr currentMove, int x, int y, t_pos const & start, t_pos const & end);

	inline int 			validMatchFreeTreeSet(posSet const & left, posSet const & right);
	inline int			validFreeThreeHorisontal(Move & currentMove, t_pos & start, t_pos & end, int y);
	inline int			validFreeThreeVertical(Move & currentMove, t_pos & start, t_pos & end, int x);
	inline int			validFreeThreeDiagonalLeft(Move & currentMove, t_pos & start, t_pos & end);
	inline int			validFreeThreeDiagonalRight(Move & currentMove, t_pos & start, t_pos & end);

	inline int 			checkFreeTreeHorizontal(MovePtr currentMove, int x, int y);
	inline int 			checkFreeTreeVertical(MovePtr currentMove, int x, int y);
	inline int 			checkFreeTreeDiagonalLeft(MovePtr currentMove, int x, int y);
	inline int 			checkFreeTreeDiagonalRight(MovePtr currentMove, int x, int y);
};

#endif
