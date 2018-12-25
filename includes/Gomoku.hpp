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
#include <unistd.h>
# include <dlfcn.h>
# include <vector>
# include <ctime>
# include <cmath>
# include <Render.hpp>
# include <algorithm>
# include <functional>
# include <queue>
# include "Checker.hpp"
# include "HeuristicSolver.hpp"
# include <memory>



static const int alfaInf = -10000000;
static const int betaInf = 10000000;
struct alfaBeta {
	alfaBeta() {alpha = alfaInf; beta = betaInf;}
	int		alpha;
	int		beta;
};


static const int maximumDepth = 3;
static const int moveChoseCount = 7;

//typedef std::priority_queue<MovePtr, std::deque<MovePtr>, MoveCmp>	movePriorityQueue;

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
	inline void 						emptyGameField(std::array<typeArr, N> &	gamefield);
	void 								moving(MovePtr currentMove);
	void 								AI_Move(MovePtr currentMove);
	inline void 						generateMoveOptions(MovePtr currentMove, movePriorityQueue & movingOptions);
	inline void 						fillMoveOptions(MovePtr currentMove, int x_center, int y_center,
															movePriorityQueue & movingOptions,  std::array<typeArr, N> &virtualGameField);
  	inline bool 						cutOff(MovePtr checkingMove,alfaBeta & ab, int & value);
  	inline void 						clearQueue(movePriorityQueue & Queue);
  	inline MovePtr 						findMove(MovePtr move);
	void 								moveProcessing(MovePtr currentMove);
	void 								moveAI_Processing(MovePtr optionMove, movePriorityQueue & movingOptions);
	void 								moveReset(MovePtr currentMove);
	MovePtr 							algorithmMiniMax(MovePtr currentMove, int  depth, int maxDepth, alfaBeta ab);

	std::unique_ptr<Render>				m_render;
	std::unique_ptr<Checker>			m_checker;
	std::unique_ptr<HeuristicSolver>	m_heuristicSolver;
	std::vector<std::vector<eType>>		m_heuristicField;
	std::array<typeArr, N>  			m_virtualGameField;
	int 								m_N;
	Move								m_currentMove;
	event								m_event;
	int									m_exit;
	int									m_AI;
	double 								m_turnTime;
	clock_t 							m_start;
	windowCondition 					m_windowCondition;
};


#endif
