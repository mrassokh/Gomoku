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

Gomoku::Gomoku():m_N(18), m_currentTurn(BLACK), m_exit(0)
{
	initGameField(m_N);
	m_render = &Render::Instance();
	m_render->attachSharedLibrary("lib1_sdl.so", m_N + 1, m_N + 1);
	m_event.event = DEFAULT;
	m_event.x = -100;
	m_event.x = -100;
	m_render->init();
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
		m_gameField.push_back(row);
	}
}

void 	Gomoku::render()
{
	m_render->renderConfigure(&m_gameField, &m_currentTurn, &m_event, &m_exit, &m_turnTime);
	clock_t start = clock();
	while (!m_exit) {
		m_render->mainLoop();
		m_turnTime = static_cast<double>((clock() - start ))/ CLOCKS_PER_SEC;
		if (m_event.event == PUSH_SQUARE){
			m_turnTime = 0;
			start = clock();
			printf("Push point with x =%d and y =%d\n", m_event.x, m_event.y);
			if((*m_gameField[m_event.y])[m_event.x] == EMPTY){
			(*m_gameField[m_event.y])[m_event.x] = m_currentTurn;
			if (m_currentTurn == BLACK)
				m_currentTurn = WHITE;
			else
				m_currentTurn = BLACK;
			m_event.event = DEFAULT;
			m_event.x = -100;
			m_event.x = -100;
		} else {
			printf("Push point with x =%d and y =%d denied - qquare is not empty\n", m_event.x, m_event.y);
		}
		}
	}
	m_render->deAttachSharedLibrary();
}
