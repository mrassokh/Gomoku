/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Render.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrassokh <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/25 17:57:21 by mrassokh          #+#    #+#             */
/*   Updated: 2018/08/25 17:57:22 by mrassokh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Render.hpp"

Render::Render()
{
	m_eventFunctions[0] = &Render::handlePushSquare;
	m_eventFunctions[1] = &Render::handleNewGame;
	m_eventFunctions[2] = &Render::handleExit;

	m_windowStateFunctions[0] = &Render::drawStart;
	m_windowStateFunctions[1] = &Render::drawGame;
	m_windowStateFunctions[2] = &Render::drawGameOver;

	m_exit = 0;
	m_windowState = START;
}


Render::~Render()
{

}

Render		&Render::Instance()
{
	static Render instance;
	return instance;
}


void 		Render::dlerror_wrapper(void) {
	std::cerr << "Error:" << dlerror() << std::endl;
	exit(EXIT_FAILURE);
}


void 		Render::attachSharedLibrary(const char* sharedLibrary, int height, int weight)
{
	dl_handle = dlopen(sharedLibrary, RTLD_LAZY | RTLD_LOCAL);
	if (!dl_handle) {
		dlerror_wrapper();
	}
	m_windowCreator = reinterpret_cast<IWindow *(*)(int width, int height)>(dlsym(dl_handle, "createWindow"));
	m_newWindow = m_windowCreator(height, weight);
}

void 			Render::init() const
{
	m_newWindow->init();
}

void 		Render::deAttachSharedLibrary()
{
	m_windowDestructor = reinterpret_cast<void (*)(IWindow*)>(dlsym(dl_handle, "deleteWindow"));
	if (!m_windowDestructor){
		dlerror_wrapper();
	}
	m_windowDestructor(m_newWindow);
	dlclose(dl_handle);
}

// void 			drawCell(int x, int y, eType type) const
// {
//
// }
EVENTS 	Render::getEvent()
{
	return m_newWindow->getEvent();
}


void 			Render::drawField(vecVecInt 	*gamefield) const
{
	// m_newWindow->startCycl();
	 vecVecInt game = *(gamefield);
	// m_newWindow->drawStart();
	for (int i = 0; i < 19; i++){
		for (int j = 0; j < 19; j++){
			//m_newWindow->drawSquare(i, j, static_cast<eType>((*gamefield)[i]->at[j]));
			m_newWindow->drawSquare(i, j, static_cast<eType>(1));
		}
	}
	m_newWindow->endCycl();
}

void 			Render::renderConfigure(vecVecInt 	*gameField, eType *currentTurn)
{
	m_gameField = gameField;
	m_currentTurn = currentTurn;
}

void 			Render::mainLoop()
{
	while (!m_exit){
		(this->*m_windowStateFunctions[m_windowState])();
	}
}
void 			Render::drawStart()
{
	m_startCondition = 1;
	while (m_startCondition){
		m_newWindow->drawStart();
		eventHandling();
	}

}

void 			drawGame()
{
	m_gameCondition = 1;
	while (m_gameCondition){
		drawField(&m_gameField);;
		eventHandling();
	}
}
void 			drawGameOver()
{
	m_gameOverCondition = 1;
	while (m_gameOverCondition){
		m_newWindow->drawGameOver("");
		eventHandling();
	}
}

void 			Render::eventHandling()
{
	EVENTS ev = m_newWindow->getEvent();
		(this->*m_eventFunctions[ev])();
}

void 			Render::handleExit()
{
	m_startCondition = 0;
	m_gameCondition = 0;
	m_gameOverCondition = 0;
	m_exit = 0;
}

void 			Render::handlePushSquare()
{

}

void 			Render::handleNewGame()
{
	m_startCondition = 0;
	m_gameCondition = 1;
	m_gameOverCondition = 0;
	m_windowState = NEW_GAME;
}
