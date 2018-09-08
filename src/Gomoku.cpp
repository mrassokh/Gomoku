/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Gomoku.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrassokh <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/25 11:35:36 by mrassokh          #+#    #+#             */
/*   Updated: 2018/08/25 11:35:40 by mrassokh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Gomoku.hpp"

Gomoku::Gomoku(std::string input):m_N(18), m_exit(0)
{
	initGameField(m_N);
	m_currentMove.currentTurn = BLACK;
	m_render = &Render::Instance();
	m_render->attachSharedLibrary("lib1_sdl.so", m_N + 1, m_N + 1);
	m_event.event = DEFAULT;
	m_event.x = -100;
	m_event.x = -100;
	m_render->init();
	if (input == "AI"){
		m_AI = 1;
		printf("AI mode\n");
	} else {
		printf("Two user mode\n");
		m_AI = 0;
	}
	m_windowCondition.startCondition = 1;
	m_windowCondition.gameCondition = 0;
	m_windowCondition.gameOverCondition = 0;
}

Gomoku::~Gomoku()
{

}

void 	Gomoku::initGameField(int N)
{
	vecInt* row;
	//m_gameField = new std::vector<vecInt>;
	for (int i = 0; i < N; i++){
		row = new vecInt();
		for (int j = 0; j < N; j++) {
			row->push_back(0);
		}
		m_currentMove.gameField.push_back(row);
	}
}

void 	Gomoku::render()
{
	m_render->renderConfigure(&m_currentMove.gameField, &m_currentMove.currentTurn, &m_event, &m_exit, &m_turnTime, m_AI, &m_windowCondition);
	//clock_t start = clock();
	//double timeFromLastTurn = 0.0;
	m_turnTime =0.0;
	m_start = clock();
	while (!m_exit) {
		m_render->mainLoop();

		if (m_event.event == PUSH_SQUARE){
			//start = clock();
			printf("Push point with x =%d and y =%d\n", m_event.x, m_event.y);
			moving(&m_currentMove);
		// 	if((*m_currentMove.gameField[m_event.y])[m_event.x] == EMPTY){
		// 	(*m_currentMove.gameField[m_event.y])[m_event.x] = m_currentMove.currentTurn;
		// 	if (m_currentMove.currentTurn == BLACK)
		// 		m_currentMove.currentTurn = WHITE;
		// 	else
		// 		m_currentMove.currentTurn = BLACK;
		// 	m_event.event = DEFAULT;
		// 	m_event.x = -100;
		// 	m_event.x = -100;
		// 	if (m_AI && m_currentMove.currentTurn == BLACK){
		// 	m_turnTime =  0.0;//timeFromLastTurn;
		// 	m_start = clock();
		// 	for (int i = 0; i < 1000000; i++){
		// 	double x = pow(sqrt(0.56789) / sqrt(1.234), 2);
		// 	printf("%f\n", x);
		// 	m_turnTime = static_cast<double>((clock() - m_start ))/ CLOCKS_PER_SEC;
		// 	}
		// }
		// } else {
		// 	printf("Push point with x =%d and y =%d denied - qquare is not empty\n", m_event.x, m_event.y);
		// }
		}
	}
	m_render->deAttachSharedLibrary();
}

void 		Gomoku::moving(t_move* currentMove)
{
	moveChecking(currentMove);
	moveProcessing(currentMove);
}

void 		Gomoku::moveChecking(t_move* currentMove)
{
	if ((*currentMove->gameField[m_event.y])[m_event.x] != EMPTY){
		currentMove->moveResult = NON_EMPTY;
		printf("Push point with x =%d and y =%d denied - qquare is not empty\n", m_event.x, m_event.y);
	} else {
		(*currentMove->gameField[m_event.y])[m_event.x] = m_currentMove.currentTurn;
		currentMove->moveResult = DEF;
	}
}

void 		Gomoku::moveProcessing(t_move* currentMove)
{
	if (currentMove->moveResult == DEF){
		currentMove->moveResult = NO_RESULT;
		if (currentMove->currentTurn == BLACK)
			currentMove->currentTurn = WHITE;
		else
			currentMove->currentTurn = BLACK;
		m_event.event = DEFAULT;
		m_event.x = -100;
		m_event.x = -100;

	} else if (currentMove->moveResult == WIN) {
		m_windowCondition.gameCondition = 0;
		m_windowCondition.gameOverCondition = 1;
	}
}
