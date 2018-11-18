/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Move.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrassokh <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/18 10:20:29 by mrassokh          #+#    #+#             */
/*   Updated: 2018/11/18 10:20:31 by mrassokh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "IWindow.hpp"

class Move final
{
public:
	Move();
	Move(Move const & rhs);
	Move & operator = (Move const & rhs);
	~Move(){};

	eType const &					getCurrentType() const { return  m_currentTurn;}
	eMoveResult	const &				getResult() const { return m_moveResult;}
	int 							getWhiteCapture() const { return m_whiteCaptures;}
	int 							getBlackCapture() const { return m_blackCaptures;}
	t_pos const & 					getLeftTop() const {return m_leftTop;}
	t_pos const & 					getRightBottom() const {return m_rightBottom;}
	posSet const &					getWhiteFreeThree() const {return m_whiteFreeThree;}
	posSet const &					getBlackFreeThree() const {return m_blackFreeThree;}
	std::array<typeArr, N>const & 	getGameField() const {return m_gameField;}

	void							setCurrentType(eType const & type) { m_currentTurn = type;}
	void							setResult(eMoveResult const & result) { m_moveResult = result;}
	void 							setWhiteCapture(int whiteCaptures) { m_whiteCaptures = whiteCaptures;}
	void							setBlackCapture(int blackCaptures) { m_blackCaptures = blackCaptures;}
	void 							setLeftTop(t_pos const & leftTop) {m_leftTop =  leftTop;}
	void							setRightBottom(t_pos const & rightBottom){ m_rightBottom = rightBottom;}
	void							setWhiteFreeThree(posSet const & whiteFreeThree) {m_whiteFreeThree = whiteFreeThree;}
	void							setBlackFreeThree(posSet const & blackFreeThree) {m_blackFreeThree = blackFreeThree;}
	void 							setMove(int x, int y) {if (m_gameField[y][x] == EMPTY) {m_gameField[y][x] = m_currentTurn;}}

	void						emptyGameField();
	void 							clearFreeThree(){ m_whiteFreeThree.clear(); m_blackFreeThree.clear();}
private:
	eType					m_currentTurn;
	std::array<typeArr, N>	m_gameField;
	eMoveResult				m_moveResult;
	int 					m_whiteCaptures;
	int 					m_blackCaptures;
	posSet					m_whiteFreeThree;
	posSet					m_blackFreeThree;
	t_pos					m_leftTop;
	t_pos					m_rightBottom;

};
