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

Gomoku::Gomoku():m_N(19)
{
	initGameField(m_N);
	m_render = &Render::Instance();
	m_render->attachSharedLibrary("lib1_sdl.so", m_N, m_N);
}

Gomoku::~Gomoku()
{
	m_render->deAttachSharedLibrary();
}

void 	Gomoku::initGameField(int N)
{
	vecInt* row;
	//m_gameField = new std::vector<vecInt>;
	for (int i = 0; i < N; i++){
		row = new vecInt();
		for (int j = 0; j < m_N; j++) {
			row->push_back(0);
		}
		m_gameField.push_back(row);
	}
}

void 	Gomoku::render()
{
	m_render->drawField(&m_gameField);
}
