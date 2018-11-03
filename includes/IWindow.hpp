/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IWindow.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrassokh <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/19 13:48:51 by mrassokh          #+#    #+#             */
/*   Updated: 2018/05/19 13:48:54 by mrassokh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WINDOW_HPP
# define WINDOW_HPP
# include <iostream>
# include <sstream>
# include <ctime>
# include <vector>

typedef std::vector<int> vecInt;

typedef enum  	type
{
	EMPTY,
	BLACK,
	WHITE
}				eType;

enum EVENTS
{
	PUSH_SQUARE,
	NEW_GAME,
	EXIT,
	DEFAULT
};

typedef struct  s_event
{
	EVENTS	event;
	int 	x;
	int		y;
}				event;

typedef struct  s_pos
{
	int 	x;
	int		y;
	eType	type;
}				t_pos;

typedef struct  s_windowCondition
{
	int			startCondition;
	int			gameCondition;
	int			gameOverCondition;
}				windowCondition;

typedef enum  	moveResult
{
	NO_RESULT,
	NON_EMPTY,
	DOUBLE_FREE_TREE,
	CAPTURE,
	WIN,
	LOSE,
	DEF
}				eMoveResult;

typedef std::vector<t_pos>  posSet;
typedef struct  			s_move
{
	eType					currentTurn;
	std::vector<vecInt *>	gameField;
	eMoveResult				moveResult;
	int 					whiteCaptures;
	int 					blackCaptures;
	std::vector <posSet>	whiteFreeTree;
	std::vector <posSet>	blackFreeTree;
	t_pos					leftTop;
	t_pos					rightBottom;
}							t_move;

class IWindow
{
public:
	virtual void 			getEvent(event *ev)  = 0;
	virtual void 			drawTile(int x, int y, eType type) = 0;
	virtual void 			drawLine(int i, int j) = 0;
	virtual void 			drawTime(eType type, int wCaptures, int bCaptures, double *turnTime, int AI) = 0;
	virtual void 			drawStart() = 0;
 	virtual void 			drawGameOver(std::string const & finishMessage) = 0;
	virtual void 			startCycl() = 0;
	virtual void 			init(void) = 0;
	virtual void 			endCycl(void) = 0;
};


# endif
