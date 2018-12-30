/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Render.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrassokh <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/25 17:49:51 by mrassokh          #+#    #+#             */
/*   Updated: 2018/08/25 17:49:54 by mrassokh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RENDER_HPP
# define RENDER_HPP
# include "IWindow.hpp"
# include <vector>
# include <array>
# include <dlfcn.h>
# include <ctime>
# include "Move.hpp"

typedef	enum windowState
{
	START,
	GAME,
	GAME_OVER
}			eWindowState;


class Render final
{
public:
	Render();
	virtual ~Render();

	void 			renderConfigure(Move *currentMove,
		 									event *ev, int *ex, double *turnTime, int AI, windowCondition *condition, eType	*AIColor, int *step);
	void 			drawField(std::array<typeArr, N> const & gameField) const;
	void 			init() const;
	void 			attachSharedLibrary(const char* sharedLibrary, int height, int weight);
	void 			deAttachSharedLibrary();
	void 			mainLoop();
	void 			GameOver(std::string const & message);

	void 			eventHandling();

private:
	Render(Render const & rhs) = delete;
	Render & operator = (Render const & rhs) = delete;

	void 			dlerror_wrapper();

	IWindow 				*(*m_windowCreator)(int width, int height);
	void					(*m_windowDestructor)(IWindow *);
	void 					*dl_handle;
	IWindow 				*m_newWindow;

	double 					*m_turnTime;

	std::array<void (Render::*)(), 5>	m_eventFunctions;
	std::array<void (Render::*)(), 3>	m_windowStateFunctions;

	void 			drawStart();
	void 			drawGame();
	void 			drawGameOver();

	void 			handleExit();
	void 			handlePushSquare();
	void 			handleNewGame();
	void 			handleChangeMove();
	void 			handleChangeAIColor();

	Move			*m_currentMove;
	windowCondition *m_windowCondition;
	event 			*m_event;
	int 			*m_exit;
	int 			m_AI;
	eWindowState	m_windowState;
	std::string		m_gameOverMessage;
	eType			*m_AIColor;
	int 			*m_step; 			
};

#endif
