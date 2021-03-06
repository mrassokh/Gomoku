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

Render::Render() : m_gameOverMessage("")
{
	m_eventFunctions[0] = &Render::handlePushSquare;
	m_eventFunctions[1] = &Render::handleNewGame;
	m_eventFunctions[2] = &Render::handleExit;
	m_eventFunctions[3] = &Render::handleChangeMove;
	m_eventFunctions[4] = &Render::handleChangeAIColor;
	m_eventFunctions[5] = &Render::handlePutTwoStone;

	m_windowStateFunctions[0] = &Render::drawStart;
	m_windowStateFunctions[1] = &Render::drawGame;
	m_windowStateFunctions[2] = &Render::drawGameOver;

	m_exit = 0;
	m_windowState = START;
}


Render::~Render()
{

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


void 			Render::drawField(std::array<typeArr, N> const	&gameField) const
{
	 m_newWindow->startCycl();

	int k;
	for (int i = 0; i < 19; i++){
		m_newWindow->drawLine(i, 0);
		m_newWindow->drawLine(0, i);
	}
	for (int i = 0; i < 18; i++){
		for (int j = 0; j < 18; j++){
			k = (gameField[j])[i];
			m_newWindow->drawTile(i, j, static_cast<eType>(k));
		}
	}

	m_newWindow->drawTime(m_currentMove->getCurrentType(), m_currentMove->getWhiteCapture(),
							m_currentMove->getBlackCapture(), m_turnTime, m_AI, m_step);
	m_newWindow->endCycl();
}

void 			Render::renderConfigure(Move *currentMove,
											event *ev, int *ex, double *turnTime,
											int AI, windowCondition *condition,
											 eType	*AIColor, int *step, int *putTwoStone)
{
	m_currentMove = currentMove;
	m_event = ev;
	m_exit = ex;
	m_turnTime = turnTime;
	m_AI = AI;
	m_windowCondition = condition;
	m_AIColor = AIColor;
	m_step = step;
	m_putTwoStone = putTwoStone;
}

void 			Render::mainLoop()
{
	(this->*m_windowStateFunctions[m_windowState])();
}

void 			Render::drawStart()
{
	m_windowCondition->startCondition = 1;
	while (m_windowCondition->startCondition){
		m_newWindow->drawStart();
		eventHandling();
	}
}

void 			Render::drawGame()
{
	m_windowCondition->gameCondition = 1;
	drawField(m_currentMove->getGameField());
	eventHandling();
}

void 			Render::drawGameOver()
{
	m_windowCondition->gameOverCondition = 1;
	while (m_windowCondition->gameOverCondition){
		m_newWindow->drawGameOver(m_gameOverMessage);
		eventHandling();
	}
}

void 			Render::eventHandling()
{
 	m_newWindow->getEvent(m_event);
	if (m_event->event != DEFAULT)
		(this->*m_eventFunctions[m_event->event])();
}

void 			Render::handleExit()
{
	m_windowCondition->startCondition = 0;
	m_windowCondition->gameCondition = 0;
	m_windowCondition->gameOverCondition = 0;
	*m_exit = 1;
}

void 			Render::handlePushSquare()
{
}

void 			Render::GameOver(std::string const & message)
{
	m_windowCondition->startCondition = 0;
	m_windowCondition->gameCondition = 0;
	m_windowCondition->gameOverCondition = 1;
	m_windowState = GAME_OVER;
	m_gameOverMessage = message;
}

void Render::handleChangeMove()
{
	auto type = m_currentMove->getCurrentType();
	m_currentMove->setCurrentType(type == BLACK ? WHITE :  BLACK);
}

void 			Render::handleChangeAIColor()
{
	*m_AIColor = *m_AIColor == BLACK ? WHITE :  BLACK;
}

void 			Render::handlePutTwoStone()
{
	*m_putTwoStone = *m_putTwoStone == 1 ? 0 : 1;
}

void 			Render::handleNewGame()
{
	m_windowCondition->startCondition = 0;
	m_windowCondition->gameCondition = 0;
	m_windowCondition->gameOverCondition = 0;
	m_windowState = GAME;
}
