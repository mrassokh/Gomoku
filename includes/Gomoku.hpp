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

typedef std::array<eType, 6> twoOpenSample;
typedef std::array<eType, 5> oneOpenSample;

// BLACK SAMPLES
static const twoOpenSample blackFourTwoOpen = {{EMPTY,BLACK,BLACK,BLACK,BLACK,EMPTY}};
static const oneOpenSample blackFourOneOpenLeft = {{EMPTY,BLACK,BLACK,BLACK,BLACK}};
static const oneOpenSample blackFourOneOpenRight = {{BLACK,BLACK,BLACK,BLACK,EMPTY}};
static const oneOpenSample blackFourOneOpenCenter_1 = {{BLACK,BLACK,EMPTY,BLACK,BLACK}};
static const oneOpenSample blackFourOneOpenCenter_2 = {{BLACK,EMPTY,BLACK,BLACK,BLACK}};
static const oneOpenSample blackFourOneOpenCenter_3 = {{BLACK,BLACK,BLACK,EMPTY,BLACK}};
//
static const twoOpenSample blackThreeTwoOpen_1 = {{EMPTY,BLACK,BLACK,BLACK,EMPTY,EMPTY}};
static const twoOpenSample blackThreeTwoOpen_2 = {{EMPTY,EMPTY,BLACK,BLACK,BLACK,EMPTY}};
static const oneOpenSample blackThreeOneOpenRight = {{BLACK,BLACK,BLACK,EMPTY,EMPTY}};
static const oneOpenSample blackThreeOneOpenLeft = {{EMPTY,EMPTY,BLACK,BLACK,BLACK}};
static const oneOpenSample blackThreeOneOpenCenter_1 = {{EMPTY,BLACK,EMPTY,BLACK,BLACK}};
static const oneOpenSample blackThreeOneOpenCenter_2 = {{EMPTY,BLACK,BLACK,EMPTY,BLACK}};
static const oneOpenSample blackThreeOneOpenCenter_3 = {{BLACK,EMPTY,BLACK,BLACK,EMPTY}};
static const oneOpenSample blackThreeOneOpenCenter_4 = {{BLACK,BLACK,EMPTY,BLACK,EMPTY}};
//
static const twoOpenSample blackTwoTwoOpen_1 = {{EMPTY,BLACK,BLACK,EMPTY,EMPTY,EMPTY}};
static const twoOpenSample blackTwoTwoOpen_2 = {{EMPTY,EMPTY,BLACK,BLACK,EMPTY,EMPTY}};
static const twoOpenSample blackTwoTwoOpen_3 = {{EMPTY,EMPTY,EMPTY,BLACK,BLACK,EMPTY}};
//
static const oneOpenSample blackTwoOneOpenRight = {{BLACK,BLACK,EMPTY,EMPTY,EMPTY}};
static const oneOpenSample blackTwoOneOpenLeft = {{EMPTY,EMPTY,EMPTY,BLACK,BLACK}};


// WHITE SAMPLES
static const twoOpenSample whiteFourTwoOpen= {{EMPTY,WHITE,WHITE,WHITE,WHITE,EMPTY}};
static const oneOpenSample whiteFourOneOpenLeft = {{EMPTY,WHITE,WHITE,WHITE,WHITE}};
static const oneOpenSample whiteFourOneOpenRight = {{WHITE,WHITE,WHITE,WHITE,EMPTY}};
static const oneOpenSample whiteFourOneOpenCenter_1 = {{WHITE,WHITE,EMPTY,WHITE,WHITE}};
static const oneOpenSample whiteFourOneOpenCenter_2 = {{WHITE,EMPTY,WHITE,WHITE,WHITE}};
static const oneOpenSample whiteFourOneOpenCenter_3 = {{WHITE,WHITE,WHITE,EMPTY,WHITE}};

static const twoOpenSample whiteThreeTwoOpen_1 = {{EMPTY,WHITE,WHITE,WHITE,EMPTY,EMPTY}};
static const twoOpenSample whiteThreeTwoOpen_2 = {{EMPTY,EMPTY,WHITE,WHITE,WHITE,EMPTY}};

static const oneOpenSample whiteThreeOneOpenRight = {{WHITE,WHITE,WHITE,EMPTY,EMPTY}};
static const oneOpenSample whiteThreeOneOpenLeft = {{EMPTY,EMPTY,WHITE,WHITE,WHITE}};
static const oneOpenSample whiteThreeOneOpenCenter_1 = {{EMPTY,WHITE,EMPTY,WHITE,WHITE}};
static const oneOpenSample whiteThreeOneOpenCenter_2 = {{EMPTY,WHITE,WHITE,EMPTY,WHITE}};
static const oneOpenSample whiteThreeOneOpenCenter_3 = {{WHITE,EMPTY,WHITE,WHITE,EMPTY}};
static const oneOpenSample whiteThreeOneOpenCenter_4 = {{WHITE,WHITE,EMPTY,WHITE,EMPTY}};

static const twoOpenSample whiteTwoTwoOpen_1 = {{EMPTY,WHITE,WHITE,EMPTY,EMPTY,EMPTY}};
static const twoOpenSample whiteTwoTwoOpen_2 = {{EMPTY,EMPTY,WHITE,WHITE,EMPTY,EMPTY}};
static const twoOpenSample whiteTwoTwoOpen_3 = {{EMPTY,EMPTY,EMPTY,WHITE,WHITE,EMPTY}};
//
static const oneOpenSample whiteTwoOneOpenRight = {{WHITE,WHITE,EMPTY,EMPTY,EMPTY}};
static const oneOpenSample whiteTwoOneOpenLeft = {{EMPTY,EMPTY,EMPTY,WHITE,WHITE}};


static const int winHeuristic = 1000000;
static const int fourTwoOpenHeuristic = 100000;
static const int fourOneOpenHeuristic = 10000;
static const int threeTwoOpenHeuristic = 10000;
static const int threeOneOpenHeuristic = 1000;
static const int twoTwoOpenHeuristic = 1000;
static const int oneTwoOpenHeuristic = 100;

static const int maximumDepth = 5;
static const int moveChoseCount = 3;

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
	inline void 						generateMoveOptions(MovePtr currentMove, movePriorityQueue & movingOptions);
	void 								fillMoveOptions(MovePtr currentMove, int x_center, int y_center, movePriorityQueue & movingOptions);
	void 								defineHeuristic(MovePtr optionMove, eMoveResult result);
	int 								defineHorizontalHeuristic(std::array<typeArr, N> & gamefield, eType currentTurn,
																		t_pos const & leftTop, t_pos const & rightBottom);
    int 								defineVerticalHeuristic(std::array<typeArr, N> & gamefield, eType currentTurn,
																			t_pos const & leftTop, t_pos const & rightBottom);
	int 								defineDiagonalLeftHeuristic(std::array<typeArr, N> & gamefield, eType currentTurn,
																			t_pos const & leftTop, t_pos const & rightBottom);
	int 								defineDiagonalRightHeuristic(std::array<typeArr, N> & gamefield, eType currentTurn,
																			t_pos const & leftTop, t_pos const & rightBottom);

   inline int							resolveHeuristicFromField(std::vector<std::vector<eType>> const & heuristicField, eType currentTurn);

	inline int 							findTwoOpenHorizontal(twoOpenSample const & sampleFourTwoOpen, std::array<typeArr, N> & gamefield,
																	t_pos const & leftTop, t_pos const & rightBottom);
	inline int 							findOneOpenHorizontal(oneOpenSample const & sampleFourTwoOpen, std::array<typeArr, N> & gamefield,
																	t_pos const & leftTop, t_pos const & rightBottom);
	inline int 							defineTwoOpenHorizontalHeuristic(twoOpenSample const & blackSample, twoOpenSample const & whiteSample, std::array<typeArr, N> & gamefield,
																			const int heuristic, eType currentTurn, t_pos const & leftTop, t_pos const & rightBottom);

	inline int 								defineOneOpenHorizontalHeuristic( oneOpenSample const & blackSample,
		 													 oneOpenSample const & whiteSample, std::array<typeArr, N> & gamefield,
															  const int heuristic, eType currentTurn, t_pos const & leftTop, t_pos const & rightBottom);


	inline int 							defineTwoOpenHeuristic(twoOpenSample const & blackSample, twoOpenSample const & whiteSample, std::vector<std::vector<eType>> const & gamefield,
  																			const int heuristic, eType currentTurn);

  	inline int 							defineOneOpenHeuristic( oneOpenSample const & blackSample, oneOpenSample const & whiteSample, std::vector<std::vector<eType>> const & gamefield,
  															  const int heuristic, eType currentTurn);


  inline int 							findTwoOpen(twoOpenSample const & sampleTwoOpen, std::vector<std::vector<eType>> const & gamefield);
  inline int 							findOneOpen(oneOpenSample const & sampleOneOpen, std::vector<std::vector<eType>> const & gamefield);




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
//	void 								moveProcessing(Move* currentMove, int forAI);
	void 								moveProcessing(Move* currentMove);
	void 								moveAI_Processing(MovePtr optionMove, movePriorityQueue & movingOptions);
	void 								moveReset(Move* currentMove);

	MovePtr 								algorithmMiniMax(MovePtr currentMove, int  depth, int maxDepth);

	//void 								emptyGameField(std::array<typeArr, N> & gamefield)

	Render								*m_render;
	//std::array<typeArr, N>				m_heuristicField;
	std::vector<std::vector<eType>>		m_heuristicField;
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
