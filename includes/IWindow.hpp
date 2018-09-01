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
//# include "gomoku.hpp"
//
// enum STATE
// {
// 	START,
// 	GAME,
// 	GAME_OVER
// };
//
// enum EVENTS
// {
// 	LEFT_FIRST,
// 	RIGHT_FIRST,
// 	LEFT_SECOND,
// 	RIGHT_SECOND,
// 	EXIT,
// 	CHANGE_TO_SDL_WIN,
// 	CHANGE_TO_SFML_WIN,
// 	CHANGE_TO_GLFW_WIN,
// 	NEW_GAME,
// 	DEFAULT
// };

class IWindow
{
public:
	 virtual void 			getEvent(event *ev)  = 0;
	 virtual void 			drawTile(int x, int y, eType type) = 0;
	 virtual void 			drawLine(int i, int j) = 0;
	 virtual void 			drawTime(double time, eType type) = 0;
	virtual void 			drawStart() = 0;
 	virtual void 			drawGameOver(std::string const & finishMessage) = 0;
	virtual void 			startCycl() = 0;
	virtual void 			init(void) = 0;
	virtual void 			endCycl(void) = 0;
//	virtual void 			quit(std::string const & finishMessage) = 0;
};


# endif
