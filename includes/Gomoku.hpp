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
	eType								findOppositeType(eType type);
	void								eraseTiles(std::vector<vecInt *>	*gameField, int startX, int startY, int endX, int endY);
	void 								moving(t_move* currentMove);
	void 								moveChecking(t_move* currentMove);
	int 								checkWin(t_move* currentMove, int x, int y);
	int 								checkWinHorizontal(t_move* currentMove, int x, int y);
	int 								checkWinVertical(t_move* currentMove, int x, int y);
	int 								checkWinDiagonalLeft(t_move* currentMove, int x, int y);
	int 								checkWinDiagonalRight(t_move* currentMove, int x, int y);

	int 								checkCapture(t_move* currentMove, int x, int y);
	int 								checkCaptureHorizontal(t_move* currentMove, int x, int y);
	int 								checkCaptureVertical(t_move* currentMove, int x, int y);
	int 								checkCaptureDiagonalLeft(t_move* currentMove, int x, int y);
	int 								checkCaptureDiagonalRight(t_move* currentMove, int x, int y);
	int 								checkFreeTree(t_move* currentMove);

	void 								moveProcessing(t_move* currentMove);
	void 								moveReset(t_move* currentMove);

	Render								*m_render;
	//std::vector<vecInt *>				m_gameField;
	int 								m_N;
	//int 								m_AI_play;
	t_move								m_currentMove;
	//eType								m_currentTurn;
	event								m_event;
	int									m_exit;
	int									m_AI;
	double 								m_turnTime;
	clock_t 							m_start;
	windowCondition 					m_windowCondition;

};

#endif
