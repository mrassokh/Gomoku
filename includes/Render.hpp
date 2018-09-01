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

typedef	enum windowState
{
	START,
	GAME,
	GAME_OVER
}			eWindowState;

typedef std::vector<std::vector<int> *> vecVecInt;

class Render
{
public:
	static Render	&Instance();
	void 			renderConfigure(vecVecInt 	*gameField, eType *currentTurn, event *ev, int *ex);
	void 			drawField(vecVecInt  *gamefield) const;
	void 			init() const;
	void 			attachSharedLibrary(const char* sharedLibrary, int height, int weight);
	void 			deAttachSharedLibrary();
	void 			mainLoop();

	void 			eventHandling();

	// void			getEvent();

private:
	Render();
	Render(Render const & rhs) = delete;
	Render & operator = (Render const & rhs) = delete;
	virtual ~Render();

	void 			dlerror_wrapper();

	IWindow 		*(*m_windowCreator)(int width, int height);
	void			(*m_windowDestructor)(IWindow *);
	void 			*dl_handle;
	IWindow 		*m_newWindow;

	vecVecInt 		*m_gameField;
	eType			*m_currentTurn;

	std::array<void (Render::*)(), 3>	m_eventFunctions;
	std::array<void (Render::*)(), 3>	m_windowStateFunctions;

	void 			drawStart();
	void 			drawGame();
	void 			drawGameOver();

	void 			handleExit();
	void 			handlePushSquare();
	void 			handleNewGame();

	int				m_startCondition;
	int				m_gameCondition;
	int				m_gameOverCondition;
	event 			*m_event;
	int 			*m_exit;
	eWindowState	m_windowState;

};

#endif
