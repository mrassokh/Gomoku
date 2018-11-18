/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Move.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrassokh <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/18 10:20:05 by mrassokh          #+#    #+#             */
/*   Updated: 2018/11/18 10:20:07 by mrassokh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Move.hpp"

Move::Move() : m_currentTurn(BLACK), m_moveResult(NO_RESULT),
				m_whiteCaptures(0), m_blackCaptures(0)
{
	emptyGameField();
}

Move::Move(Move const & rhs) : m_currentTurn(rhs.getCurrentType()),
								m_gameField(rhs.getGameField()),
								m_moveResult(rhs.getResult()),
								m_whiteCaptures(rhs.getWhiteCapture()),
								m_blackCaptures(rhs.getBlackCapture()),
								m_leftTop(rhs.getLeftTop()),
								m_rightBottom(rhs.getRightBottom()) { printf("Copy!!\n\n\n");}

Move & Move::operator = (Move const & rhs)
{
	if (this == &rhs)
		return *this;
	m_currentTurn = rhs.getCurrentType();
	m_gameField = rhs.getGameField();
	m_moveResult = rhs.getResult();
	m_whiteCaptures = rhs.getWhiteCapture();
	m_blackCaptures = rhs.getBlackCapture();
	m_leftTop = rhs.getLeftTop();
	m_rightBottom = rhs.getRightBottom();
	printf("Assignment!!\n\n\n");
	return *this;
}
//Move::~Move() {}


void 	Move::emptyGameField()
{
	for (int i = 0; i < N; i++){
		m_gameField[i].fill(EMPTY);
	}
}
