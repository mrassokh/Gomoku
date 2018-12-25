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

class Checker final{
public:
	Checker(){};
	~Checker(){};

	void 			 	moveChecking(Move & currentMove, int x, int y);

private:
	Checker(Checker const & rhs) = delete;
	Checker& operator = (Checker const & rhs) = delete;

	inline eType		findOppositeType(eType type);
	inline void			eraseTiles(std::array<typeArr, N>	*gameField, int startX, int startY, int endX, int endY);
	inline void 		redefineGameArea(Move & currentMove, std::array<typeArr, N> & gamefield, int x, int y);

	inline int 			checkWin(Move & currentMove, int x, int y);
	inline int 			checkWinHorizontal(Move & currentMove, int x, int y);
	inline int 			checkWinVertical(Move & currentMove, int x, int y);
	inline int 			checkWinDiagonalLeft(Move & currentMove, int x, int y);
	inline int 			checkWinDiagonalRight(Move & currentMove, int x, int y);

	inline int 			checkPossibleCaptureHorizontalWin(Move & currentMove, int startX, int endX, int y);
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
};

#endif
