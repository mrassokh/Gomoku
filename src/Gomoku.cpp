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

Gomoku::Gomoku(std::string input):m_N(18), m_exit(0)
{
	initGameField(m_N);
	m_currentMove.currentTurn = BLACK;
	m_currentMove.whiteCaptures = 0;
	m_currentMove.blackCaptures = 0;
	m_render = &Render::Instance();
	m_render->attachSharedLibrary("lib1_sdl.so", m_N + 1, m_N + 1);
	m_event.event = DEFAULT;
	m_event.x = -100;
	m_event.x = -100;
	m_render->init();
	m_AI = input == "AI" ? 1 : 0;
	// if (input == "AI"){
	// 	m_AI = 1;
	// 	printf("AI mode\n");
	// } else {
	// 	printf("Two user mode\n");
	// 	m_AI = 0;
	// }
	m_windowCondition.startCondition = 1;
	m_windowCondition.gameCondition = 0;
	m_windowCondition.gameOverCondition = 0;
}

Gomoku::~Gomoku()
{

}

void 	Gomoku::initGameField(int N)
{
	vecInt* row;
	//m_gameField = new std::vector<vecInt>;
	for (int i = 0; i < N; i++){
		row = new vecInt();
		for (int j = 0; j < N; j++) {
			row->push_back(0);
		}
		m_currentMove.gameField.push_back(row);
	}
}

void 		Gomoku::moveReset(t_move* currentMove)
{
	currentMove->moveResult = NO_RESULT;
	if (currentMove->currentTurn == BLACK)
		currentMove->currentTurn = WHITE;
	else
		currentMove->currentTurn = BLACK;
	m_event.event = DEFAULT;
	m_event.x = -100;
	m_event.x = -100;
}
void 	Gomoku::clearGameField(int N)
{
	// vecInt* row;
	//m_gameField = new std::vector<vecInt>;
	for (int i = 0; i < N; i++){
		for (int j = 0; j < N; j++) {
			(*m_currentMove.gameField[i])[j] = EMPTY;
		}
	}
}

void 	Gomoku::render()
{
	m_render->renderConfigure(&m_currentMove, &m_event, &m_exit, &m_turnTime, m_AI, &m_windowCondition);
	//clock_t start = clock();
	//double timeFromLastTurn = 0.0;
	m_turnTime =0.0;
	m_start = clock();
	while (!m_exit) {
		m_render->mainLoop();

		if (m_event.event == PUSH_SQUARE){
			//start = clock();
			printf("Push point with x =%d and y =%d\n", m_event.x, m_event.y);
			moving(&m_currentMove);
		// 	if((*m_currentMove.gameField[m_event.y])[m_event.x] == EMPTY){
		// 	(*m_currentMove.gameField[m_event.y])[m_event.x] = m_currentMove.currentTurn;
		// 	if (m_currentMove.currentTurn == BLACK)
		// 		m_currentMove.currentTurn = WHITE;
		// 	else
		// 		m_currentMove.currentTurn = BLACK;
		// 	m_event.event = DEFAULT;
		// 	m_event.x = -100;
		// 	m_event.x = -100;
		// 	if (m_AI && m_currentMove.currentTurn == BLACK){
		// 	m_turnTime =  0.0;//timeFromLastTurn;
		// 	m_start = clock();
		// 	for (int i = 0; i < 1000000; i++){
		// 	double x = pow(sqrt(0.56789) / sqrt(1.234), 2);
		// 	printf("%f\n", x);
		// 	m_turnTime = static_cast<double>((clock() - m_start ))/ CLOCKS_PER_SEC;
		// 	}
		// }
		// } else {
		// 	printf("Push point with x =%d and y =%d denied - qquare is not empty\n", m_event.x, m_event.y);
		// }
		}
	}
	m_render->deAttachSharedLibrary();
}

void 		Gomoku::moving(t_move* currentMove)
{
	moveChecking(currentMove);
	moveProcessing(currentMove);
}

void 		Gomoku::moveChecking(t_move* currentMove)
{
	if ((*currentMove->gameField[m_event.y])[m_event.x] != EMPTY){
		currentMove->moveResult = NON_EMPTY;
		printf("Push point with x =%d and y =%d denied - qquare is not empty\n", m_event.x, m_event.y);
	} else if (checkWin(currentMove, m_event.x, m_event.y)){
		printf("Push point with x =%d and y =%d and win!\n", m_event.x, m_event.y);
		(*currentMove->gameField[m_event.y])[m_event.x] = m_currentMove.currentTurn;
		currentMove->moveResult = WIN;
	} else 	if (checkCapture(currentMove, m_event.x, m_event.y)){
		(*currentMove->gameField[m_event.y])[m_event.x] = m_currentMove.currentTurn;

		currentMove->moveResult = CAPTURE;
	} else {
		(*currentMove->gameField[m_event.y])[m_event.x] = m_currentMove.currentTurn;
		currentMove->moveResult = DEF;
	}
}

void 		Gomoku::moveProcessing(t_move* currentMove)
{
	if (currentMove->moveResult == DEF){
		moveReset(currentMove);

	} else if (currentMove->moveResult == CAPTURE){
		moveReset(currentMove);
	}
	else if (currentMove->moveResult == WIN) {
		// m_windowCondition.gameCondition = 0;
		// m_windowCondition.gameOverCondition = 1;
		std::string result = currentMove->currentTurn == BLACK ? "BLACK WIN!!!" : "WHITE WIN!!!";
		clearGameField(m_N);
		//moveReset(currentMove);
		m_render->GameOver(result);

	}
}

int 			Gomoku::checkWin(t_move* currentMove, int x, int y)
{
	printf("Chek x =%d and y =%d to win\n", m_event.x, m_event.y );
	if (checkWinHorizontal(currentMove, x, y))
		return 1;
	else if (checkWinVertical(currentMove, x, y))
		return 1;
	else if (checkWinDiagonalLeft(currentMove, x, y))
		return 1;
	else if (checkWinDiagonalRight(currentMove, x, y))
		return 1;
	return 0;
}

int 			Gomoku::checkWinHorizontal(t_move* currentMove, int x, int y)
{
	int match = 1;
	for (int i = 1; i < 5; ++i) {
		int posX = x + i;
		if (posX > 17 || (*currentMove->gameField[y])[posX] != currentMove->currentTurn)
			break;
		else if(++match == 5){
			printf("match for x =%d and y =%d is %d\n", x, y, match );
			return 1;
		}
		printf("match for x =%d and y =%d is %d\n", x, y, match );
	}
	for (int i = 1; i < 5 ; ++i) {
		int posX = x - i;
		if (posX < 0 || (*currentMove->gameField[y])[posX] != currentMove->currentTurn)
			break;
		else if(++match == 5){
			printf("match for x =%d and y =%d is %d\n", x, y, match );
			return 1;
		}
		printf("match for x =%d and y =%d is %d\n", x, y, match );
	}
	return 0;
}

int 			Gomoku::checkWinVertical(t_move* currentMove, int x, int y)
{
	int match = 1;
	for (int i = 1; i < 5; ++i) {
		int posY = y + i;
		if (posY > 17 || (*currentMove->gameField[posY])[x] != currentMove->currentTurn)
			break;
		else if(++match == 5) {
			return 1;
		}
	}
	for (int i = 1; i < 5 ; ++i){
		int posY = y - i;
		if (posY < 0 || (*currentMove->gameField[posY])[x] != currentMove->currentTurn)
			break;
		else if (++match == 5){
			return 1;
		}
	}
	return 0;

}

int 			Gomoku::checkWinDiagonalLeft(t_move* currentMove, int x, int y)
{
	int match = 1;
	for (int i = 1; i < 5; ++i) {
		int posX = x + i;
		int posY = y + i;
		if (posX > 17 || posY > 17 || (*currentMove->gameField[posY])[posX] != currentMove->currentTurn)
			break;
		else if (++match == 5) {
			return 1;
		}
	}
	for (int i = 1; i < 5 ; ++i){
		int posX = x - i;
		int posY = y - i;
		if (posX < 0 || posY < 0 || (*currentMove->gameField[posY])[posX] != currentMove->currentTurn)
			break;
		else if (++match == 5){
			return 1;
		}
	}
	return 0;
}

int 			Gomoku::checkWinDiagonalRight(t_move* currentMove, int x, int y)
{
	int match = 1;
	for (int i = 1; i < 5; ++i) {
		int posX = x - i;
		int posY = y + i;
		if (posX < 0 || posY > 17 || (*currentMove->gameField[posY])[posX] != currentMove->currentTurn)
			break;
		else if(++match == 5) {
			return 1;
		}
	}
	for (int i = 1; i < 5 ; ++i){
		int posX = x + i;
		int posY = y - i;
		if (posX > 17 || posY < 0 || (*currentMove->gameField[posY])[posX] != currentMove->currentTurn)
			break;
		else if (++match == 5){
			return 1;
		}
	}
	return 0;
}

eType			Gomoku::findOppositeType(eType type)
{
	return type == BLACK ? WHITE : (type == WHITE ? BLACK : EMPTY);
}

void			Gomoku::eraseTiles(std::vector<vecInt *>	*gameField, int startX, int startY, int endX, int endY)
{
	printf("ERASE TILE x =%d and y =%d x2 =%d and y2 =%d\n", startX, startY,  endX, endY);
	if (startX < 0 || startX > 17 || endX > 17
		|| startY < 0 || startY > 17 || endY > 17 )
		return;
	if (startX == endX){
		for (int i = startY; i <= endY; ++i) {
			printf("ERASE TILE x =%d and y =%d\n", startX, i );
			(*(*gameField)[i])[startX] = EMPTY;
		}
		return;
	}

	if (startY == endY){
		for (int i = startX; i <= endX; ++i) {
			printf("ERASE TILE x =%d and y =%d\n", i, startY );
			(*(*gameField)[startY])[i] = EMPTY;
		}
		return;
	}
	for (int i = startY; i <= endY; ++i) {
		for (int j = startX; j <= endX; ++j){
			printf("ERASE TILE x =%d and y =%d\n", j, i );
			(*(*gameField)[i])[j] = EMPTY;
		}
	}
}

int 			Gomoku::checkCapture(t_move* currentMove, int x, int y)
{

	int captures = 0;
	captures += checkCaptureHorizontal(currentMove, x, y);
	if (currentMove->currentTurn == WHITE)
		currentMove->whiteCaptures += captures;
	else
	 	currentMove->blackCaptures += captures;
	// if (checkCaptureHorizontal(currentMove, x, y)){
	// 	printf("Chek capture x =%d and y =%d\n", m_event.x, m_event.y );
	// 			return 1;
	// }

	// else if (checkCaptureVertical(currentMove, x, y))
	// 	return 1;
	// else if (checkCaptureDiagonalLeft(currentMove, x, y))
	// 	return 1;
	// else if (checkCaptureDiagonalRight(currentMove, x, y))
	// 	return 1;
	return captures;
}

int 			Gomoku::checkCaptureHorizontal(t_move* currentMove, int x, int y)
{
	int capture = 0;
	int cursor = 0;
	for (cursor = 1; cursor < 4; ++cursor) {
		int posX = x + cursor;
		printf("Chek type for capture x =%d and y =%d\n type = %d current move = %d iteration = %d\n", posX, y, (int)findOppositeType(currentMove->currentTurn), (int)(*currentMove->gameField[y])[posX], cursor);
		if (cursor < 3
			&& (posX > 17 || (*currentMove->gameField[y])[posX] != findOppositeType(currentMove->currentTurn)))
				break;
		if (cursor == 3
			 &&	(posX > 17 || (*currentMove->gameField[y])[posX] != currentMove->currentTurn))
				break;
	}
	if (cursor == 4){
		//cursor = 0;
		printf("Chek capture x =%d and y =%d\n", x, y );
		eraseTiles(&currentMove->gameField, x + 1 , y, x + 2, y);
		capture++;
	}
	for (cursor = 1; cursor < 4; ++cursor) {
		int posX = x - cursor;
		printf("Chek type for capture to left x =%d and y =%d\n type = %d current move = %d iteration = %d\n", posX, y, (int)findOppositeType(currentMove->currentTurn), (int)(*currentMove->gameField[y])[posX], cursor);
		if (cursor < 3
			&& (posX < 0 || (*currentMove->gameField[y])[posX] != findOppositeType(currentMove->currentTurn)))
				break;
		if (cursor == 3
			&& (posX < 0 || (*currentMove->gameField[y])[posX] != currentMove->currentTurn))
				break;
	}
	if (cursor == 4){
		printf("Chek capture x =%d and y =%d\n", x, y );
		eraseTiles(&currentMove->gameField, x - 2 , y, x -1, y);
		capture++;
	}
	return capture;
}

// int 			Gomoku::checkCaptureVertical(t_move* currentMove, int x, int y)
// {
//
// }
//
// int 			Gomoku::checkCaptureDiagonalLeft(t_move* currentMove, int x, int y)
// {
//
// }
//
// int 			Gomoku::checkCaptureDiagonalRight(t_move* currentMove, int x, int y)
// {
//
// }

// int 								checkWinDiagonalRight(t_move* currentMove, int x, int y);
// int 								checkCapture(t_move* currentMove);
// int 								checkFreeTree(t_move* currentMove);
