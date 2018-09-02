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

typedef std::vector<int> vecInt;

typedef	enum windowState
{
	START,
	GAME,
	GAME_OVER
}			eWindowState;


class Render
{
public:
	static Render	&Instance();
	void 			renderConfigure(std::vector<vecInt *> *gamefield, eType *currentTurn,
		 									event *ev, int *ex, double *turnTime, int AI);
	void 			drawField(std::vector<vecInt *>  *gamefield) const;
	void 			init() const;
	void 			attachSharedLibrary(const char* sharedLibrary, int height, int weight);
	void 			deAttachSharedLibrary();
	void 			mainLoop();

	void 			eventHandling();

private:
	Render();
	Render(Render const & rhs) = delete;
	Render & operator = (Render const & rhs) = delete;
	virtual ~Render();

	void 			dlerror_wrapper();

	IWindow 				*(*m_windowCreator)(int width, int height);
	void					(*m_windowDestructor)(IWindow *);
	void 					*dl_handle;
	IWindow 				*m_newWindow;

	std::vector<vecInt *>	*m_gameField;
	eType					*m_currentTurn;
	double 					*m_turnTime;
	//clock_t 				*m_start;

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
	int 			m_AI;
	eWindowState	m_windowState;
};

#endif
