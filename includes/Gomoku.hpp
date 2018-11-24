/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Gomoku.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrassokh <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/25 11:37:27 by mrassokh          #+#    #+#             */
/*   Updated: 2018/08/25 11:37:28 by mrassokh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GOMOKU_HPP
# define GOMOKU_HPP
# include "IWindow.hpp"
# include <dlfcn.h>
# include <vector>
# include <ctime>
# include <cmath>
# include <Render.hpp>
# include <algorithm>
# include <functional>
# include <queue>

static const int winHeuristic = 1000000;

typedef std::priority_queue<MovePtr, std::deque<MovePtr>, MoveCmp>	movePriorityQueue;

class Gomoku
{

public:
	Gomoku(std::string input);
	Gomoku(Gomoku & rhs) = delete;
	Gomoku& operator = (Gomoku & rhs) = delete;
	virtual ~Gomoku();

	void 								render();
private:

	void 								initGameField(int N);
	void 								clearGameField(int N);
	void 								emptyGameField(std::array<typeArr, N> &	gamefield);
	eType								findOppositeType(eType type);
	//void								eraseTiles(std::vector<vecInt *>	*gameField, int startX, int startY, int endX, int endY);
	void								eraseTiles(std::array<typeArr, N>	*gameField, int startX, int startY, int endX, int endY);
	//void 								moving(Move* currentMove);
	void 								moving(Move *currentMove);
	void 								AI_Move(Move* currentMove);

	void 								redefineGameArea(Move* currentMove, std::array<typeArr, N> & gamefield, int x, int y);
	//void 								checkNewMove(currentMove,gamefield,x,y);
	// void 								moveChecking(Move* currentMove);
	void 								fillMoveOptions(MovePtr currentMove, int x_center, int y_center, movePriorityQueue & movingOptions);
	void 								moveChecking(Move* currentMove, int x, int y);
	int 								checkWin(Move* currentMove, int x, int y);
	int 								checkWinHorizontal(Move* currentMove, int x, int y);
	int 								checkWinVertical(Move* currentMove, int x, int y);
	int 								checkWinDiagonalLeft(Move* currentMove, int x, int y);
	int 								checkWinDiagonalRight(Move* currentMove, int x, int y);

	inline int 							checkPossibleCaptureHorizontalWin(Move* currentMove, int startX, int endX, int y);
	inline int 							checkPossibleCaptureVerticalWin(Move* currentMove, int startY, int endY, int x);
	inline int 							checkPossibleCaptureDiagonalLeftWin(Move* currentMove, t_pos const & start);
	inline int 							checkPossibleCaptureDiagonalRightWin(Move* currentMove, t_pos const & start);
	int 								checkCapture(Move* currentMove, int x, int y);
	int 								checkCaptureHorizontal(Move* currentMove, int x, int y);
	int 								checkCaptureVertical(Move* currentMove, int x, int y);
	int 								checkCaptureDiagonalLeft(Move* currentMove, int x, int y);
	int 								checkCaptureDiagonalRight(Move* currentMove, int x, int y);
	int 								checkFreeTree(Move* currentMove, int x, int y);
	int									checkDoubleFreeTree(Move &currentMove);
	inline int							fillThreeSet(Move & currentMove, posSet const & examinedSet);
	int 								fillHorisontalFreeTreeSet(Move* currentMove, int x, int y, t_pos const & start, t_pos const & end);
	int 								fillVerticalFreeTreeSet(Move* currentMove, int x, int y, t_pos const & start, t_pos const & end);
	int 								fillDiagonalLeftFreeTreeSet(Move* currentMove, int x, int y, t_pos const & start, t_pos const & end);
	int 								fillDiagonalRightFreeTreeSet(Move* currentMove, int x, int y, t_pos const & start, t_pos const & end);
	inline int 							validMatchFreeTreeSet(posSet const & left, posSet const & right);
	inline int							validFreeThreeHorisontal(Move & currentMove, t_pos & start, t_pos & end, int y);
	inline int							validFreeThreeVertical(Move & currentMove, t_pos & start, t_pos & end, int x);
	inline int							validFreeThreeDiagonalLeft(Move & currentMove, t_pos & start, t_pos & end);
	inline int							validFreeThreeDiagonalRight(Move & currentMove, t_pos & start, t_pos & end);
	int 								checkFreeTreeHorizontal(Move* currentMove, int x, int y);
	int 								checkFreeTreeVertical(Move* currentMove, int x, int y);
	int 								checkFreeTreeDiagonalLeft(Move* currentMove, int x, int y);
	int 								checkFreeTreeDiagonalRight(Move* currentMove, int x, int y);
	void 								moveProcessing(Move* currentMove);
	void 								moveAI_Processing(MovePtr optionMove, movePriorityQueue & movingOptions);
	void 								moveReset(Move* currentMove);

	Render								*m_render;
	//std::vector<vecInt *>				m_gameField;
	int 								m_N;
	//int 								m_AI_play;
	//Move								m_currentMove;
	Move								m_currentMove;
	//eType								m_currentTurn;
	event								m_event;
	int									m_exit;
	int									m_AI;
	double 								m_turnTime;
	clock_t 							m_start;
	windowCondition 					m_windowCondition;

};

#endif
