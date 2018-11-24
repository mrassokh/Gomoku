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
	m_render = &Render::Instance();
	m_render->attachSharedLibrary("lib1_sdl.so", m_N + 1, m_N + 1);
	m_event.event = DEFAULT;
	m_event.x = -100;
	m_event.y = -100;
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


void 		Gomoku::moveReset(Move* currentMove)
{
	currentMove->setResult(NO_RESULT);
	if (currentMove->getCurrentType() == BLACK)
		currentMove->setCurrentType(WHITE);
	else
		currentMove->setCurrentType(BLACK);
	m_event.event = DEFAULT;
	m_event.x = -100;
	m_event.y = -100;
	currentMove->clearFreeThree();
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
		if (m_AI && m_currentMove.getCurrentType() == WHITE)
			AI_Move(&m_currentMove);

		if (m_event.event == PUSH_SQUARE){
			//start = clock();
			printf("Push point with x =%d and y =%d\n", m_event.x, m_event.y);
			moving(&m_currentMove);
		// 	if((*m_currentMove.gameField[m_event.y])[m_event.x] == EMPTY){
		// 	(*m_currentMove.gameField[m_event.y])[m_event.x] = m_currentMove.currentTurn;
		// 	if (m_ m_currentMove.getCurrentType() == BLACK)
		// 		m_currentMove.currentTurn = WHITE;
		// 	else
		// 		m_currentMove.currentTurn = BLACK;
		// 	m_event.event = DEFAULT;
		// 	m_event.x = -100;
		// 	m_event.x = -100;
		// 	if (m_AI && m_ m_currentMove.getCurrentType() == BLACK){
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

//void 		Gomoku::moving(Move* currentMove)
void 		Gomoku::moving(Move *currentMove)
{
	moveChecking(currentMove, m_event.x, m_event.y);
	moveProcessing(currentMove);
}

void 		Gomoku::AI_Move(Move* currentMove)
{

	m_turnTime =  0.0;//timeFromLastTurn;
	m_start = clock();
	int lextX = currentMove->getLeftTop().x;
	int topY = currentMove->getLeftTop().y;
	int rightX = currentMove->getRightBottom().x;
	int bottomY = currentMove->getRightBottom().y;
	movePriorityQueue movingOptions;
	//std::array<typeArr, N> & gamefield = const_cast<std::array<typeArr, N> &>(currentMove->getGameField());
	for (int y =  topY; y <= bottomY; ++y) {
		for (int x = lextX; x <= rightX; ++x){
			printf("Checked x = %d; y = %d;\n\n", x, y);
			fillMoveOptions(currentMove, x, y, movingOptions);
		}
	}
	moveProcessing(movingOptions.top());
	m_currentMove = *movingOptions.top();
	//movingOptions.clear();
	// for (int i = 0; i < 1000000; i++){
	// 	double x = pow(sqrt(0.56789) / sqrt(1.234), 2);
	// 	//printf("%f\n", x);
	// 	printf("Left top x = %d; y = %d; Right bottom x = %d; y = %d\n\n\n", currentMove->getLeftTop().x, currentMove->getLeftTop().y, currentMove->getRightBottom().x, currentMove->getRightBottom().y);
	// 	x = 0;
	// }
	m_turnTime = static_cast<double>((clock() - m_start ))/ CLOCKS_PER_SEC;

	// if (currentMove->getCurrentType() == BLACK)
	// 	currentMove->setCurrentType(WHITE);
	// else
	// 	currentMove->setCurrentType(BLACK);
	// currentMove->setMove(0,0);
}

void 		Gomoku::fillMoveOptions(MovePtr currentMove, int x_center, int y_center, movePriorityQueue & movingOptions)
{
	for (int y = y_center - 1; y <= y_center + 1; ++y) {
		if (y < 0 || y > 17)
			continue;
		for (int x = x_center - 1; x <= x_center + 1; ++x) {
			if (x < 0 || x > 17 || (y == y_center && x == x_center))
				continue;
			MovePtr newMove = new Move(*currentMove);
			moveChecking(newMove, x, y);
			moveAI_Processing(newMove, movingOptions);
		}
	}
}

void 		Gomoku::moveChecking(Move* currentMove, int x, int y)
{
	std::array<typeArr, N> & gamefield = const_cast<std::array<typeArr, N> &>(currentMove->getGameField());
	// t_pos & leftTop = const_cast<t_pos &>(currentMove->getLeftTop());
	// t_pos & rightBottom = const_cast<t_pos &>(currentMove->getRightBottom());
	if (gamefield[y][x] != EMPTY){
		currentMove->setResult(NON_EMPTY);
		printf("Push point with x =%d and y =%d denied - qquare is not empty\n", x, y);
	} else if (checkWin(currentMove, x, y)){
		printf("Push point with x =%d and y =%d and win!\n", x, y);
		//gamefield[y][x] = m_currentMove.getCurrentType();
		redefineGameArea(currentMove, gamefield, x ,y);
		//redefineGameArea(x,y,gamefield,leftTop,rightBottom);
		currentMove->setResult(WIN);
	} else 	if (checkCapture(currentMove, x, y)){
		// gamefield[y][x] = m_currentMove.getCurrentType();

		redefineGameArea(currentMove, gamefield, x, y);
		currentMove->setResult((currentMove->getWhiteCapture() >= 5 || currentMove->getBlackCapture() >= 5) ? WIN : CAPTURE);
	} else if (checkFreeTree(currentMove, x, y)){
		currentMove->setResult(DOUBLE_FREE_TREE);
	} else {
		// gamefield[y][x] = m_currentMove.getCurrentType();
		// t_pos & leftTop = const_cast<t_pos &>(currentMove->getLeftTop());
		// t_pos & rightBottom = const_cast<t_pos &>(currentMove->getRightBottom());
		redefineGameArea(currentMove, gamefield, x, y);
		currentMove->setResult(DEF);
	}
}

void 					Gomoku::redefineGameArea(Move* currentMove, std::array<typeArr, N> & gamefield, int x, int y)
{
	t_pos & leftTop = const_cast<t_pos &>(currentMove->getLeftTop());
	t_pos & rightBottom = const_cast<t_pos &>(currentMove->getRightBottom());
	if (leftTop.x == -1) {
		leftTop.x = x;
		leftTop.y = y;
		rightBottom.x = x;
		rightBottom.y = y;
	} else {
		if (x < leftTop.x)
			leftTop.x = x;
		if (y < leftTop.y)
			leftTop.y = y;
		if (x > rightBottom.x)
			rightBottom.x = x;
		if (y > rightBottom.y)
			rightBottom.y = y;
	}
	printf("Left top x = %d; y = %d; Right bottom x = %d; y = %d\n\n\n", currentMove->getLeftTop().x, currentMove->getLeftTop().y, currentMove->getRightBottom().x, currentMove->getRightBottom().y);
	gamefield[y][x] = currentMove->getCurrentType();
}

void 		Gomoku::moveProcessing(Move* currentMove)
{
	eMoveResult	const & result = currentMove->getResult();
	if (result == DEF){
		moveReset(currentMove);

	} else if (result == CAPTURE){
		moveReset(currentMove);
	} else if (result == DOUBLE_FREE_TREE){
		currentMove->clearFreeThree();
	}
	else if (result == WIN) {
		std::string result_str =  currentMove->getCurrentType() == BLACK ? "BLACK WIN!!!" : "WHITE WIN!!!";
		currentMove->emptyGameField();
		//emptyGameField(m_currentMove.gameField);
		m_currentMove.setCurrentType(BLACK);
		m_render->GameOver(result_str);
	}
}

void 		Gomoku::moveAI_Processing(MovePtr optionMove, movePriorityQueue & movingOptions)
{
	eMoveResult	const & result = optionMove->getResult();
	if (result == NON_EMPTY || result == DOUBLE_FREE_TREE){
		delete optionMove;
		return ;
	}
	if (result == DEF)
		//optionMove->defineHeuristic();
		optionMove->setHeuristic(optionMove->getCurrentType() == BLACK ? 2 : -2);
	else if (result == CAPTURE)
		//optionMove->defineHeuristic();
		optionMove->setHeuristic(optionMove->getCurrentType() == BLACK ? 10000 : -10000);
	else if (result == WIN) {
		int heur = optionMove->getCurrentType() == BLACK ? winHeuristic : -winHeuristic;
		optionMove->setHeuristic(heur);
	}
	movingOptions.push(optionMove);
}

int 			Gomoku::checkWin(Move* currentMove, int x, int y)
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
	printf("Chek x =%d and y =%d to win return 0\n", m_event.x, m_event.y );
	return 0;
}

inline int 			Gomoku::checkPossibleCaptureHorizontalWin(Move* currentMove, int startX, int endX, int y)
{
	if (y < 1 || y > 16)
		return 1;
	eType const & currType = currentMove->getCurrentType();
	for (int i = startX; i < endX; ++i)
	{
		if (y + 2 <= 17
			&& (currentMove->getGameField()[y - 1])[i] == findOppositeType(currType)
			&& (currentMove->getGameField()[y + 1])[i] == currType
			&& (currentMove->getGameField()[y + 2])[i] == EMPTY){
			printf("checkPossibleCaptureHorizontalWin for x =%d and y =%d \n", i, y);
			return 0;
		}
		if (y - 2 >= 0
			&& (currentMove->getGameField()[y + 1])[i] == findOppositeType(currType)
			&& (currentMove->getGameField()[y - 1])[i] == currType
			&& (currentMove->getGameField()[y - 2])[i] == EMPTY){
				printf("checkPossibleCaptureHorizontalWin for x =%d and y =%d \n", i, y);
				return 0;
		}
	}
	return 1;
}

inline int 			Gomoku::checkPossibleCaptureVerticalWin(Move* currentMove, int startY, int endY, int x)
{
	if (x < 1 || x > 16)
		return 1;
	for (int i = startY; i < endY; ++i)
	{
		if (x + 2 <= 17
			&& (currentMove->getGameField()[i])[x - 1] == findOppositeType(currentMove->getCurrentType())
			&& (currentMove->getGameField()[i])[x + 1] == currentMove->getCurrentType()
			&& (currentMove->getGameField()[i])[x + 2] == EMPTY){
			printf("checkPossibleCaptureVerticalWin for x =%d and x =%d \n", i, x);
			return 0;
		}
		if (x - 2 >= 0
			&& (currentMove->getGameField()[i])[x + 1] == findOppositeType(currentMove->getCurrentType())
			&& (currentMove->getGameField()[i])[x - 1] == currentMove->getCurrentType()
			&& (currentMove->getGameField()[i])[x - 2] == EMPTY){
				printf("checkPossibleCaptureVerticalWin for x =%d and x =%d \n", i, x);
				return 0;
		}
	}
	return 1;
}

inline int 		Gomoku::checkPossibleCaptureDiagonalLeftWin(Move* currentMove, t_pos const & start)
{
	int posX = start.x;
	int posY = start.y;
	for (int i = 0; i < 5; ++i)
	{
		if (posX + 2 <= 17 && posY - 2 >= 0 && posX - 1 >=0 && posY + 1 <= 17
			&& (currentMove->getGameField()[posY + 1])[posX - 1] == findOppositeType(currentMove->getCurrentType())
			&& (currentMove->getGameField()[posY - 1])[posX + 1] == currentMove->getCurrentType()
			&& (currentMove->getGameField()[posY - 2])[posX + 2] == EMPTY) {
			printf("checkPossibleCaptureDiagonalLeftWin for x =%d and x =%d \n", posX, posY);
			return 0;
		}

		if (posX + 1 <= 17 && posY - 1 >= 0 && posX - 2 >=0  && posY + 2 <= 17
			&& (currentMove->getGameField()[posY - 1])[posX + 1] == findOppositeType(currentMove->getCurrentType())
			&& (currentMove->getGameField()[posY + 1])[posX - 1] == currentMove->getCurrentType()
			&& (currentMove->getGameField()[posY + 2])[posX - 2] == EMPTY) {
			printf("checkPossibleCaptureDiagonalLeftWin for x =%d and x =%d \n", posX, posY);
			return 0;
		}
		posX++;
		posY++;
	}
 	return 1;
}

inline int 		Gomoku::checkPossibleCaptureDiagonalRightWin(Move* currentMove, t_pos const & start)
{
	int posX = start.x;
	int posY = start.y;
	for (int i = 0; i < 5; ++i)
	{
		if (posX + 2 <= 17 && posY + 2 <= 17 && posX - 1 >=0 && posY - 1 >= 0
			&& (currentMove->getGameField()[posY - 1])[posX - 1] == findOppositeType(currentMove->getCurrentType())
			&& (currentMove->getGameField()[posY + 1])[posX + 1] == currentMove->getCurrentType()
			&& (currentMove->getGameField()[posY + 2])[posX + 2] == EMPTY) {
			printf("checkPossibleCaptureDiagonalLeftWin for x =%d and x =%d \n", posX, posY);
			return 0;
		}
		if (posX + 1 <= 17 && posY + 1 <= 17 && posX - 2 >=0 && posY - 2 >= 0
			&& (currentMove->getGameField()[posY + 1])[posX + 1] == findOppositeType(currentMove->getCurrentType())
			&& (currentMove->getGameField()[posY - 1])[posX - 1] == currentMove->getCurrentType()
			&& (currentMove->getGameField()[posY - 2])[posX - 2] == EMPTY) {
			printf("checkPossibleCaptureDiagonalLeftWin for x =%d and x =%d \n", posX, posY);
			return 0;
		}
		posX--;
		posY++;
	}
 	return 1;
}


int 			Gomoku::checkWinHorizontal(Move* currentMove, int x, int y)
{
		printf("checkWinHorizontalx =%d and y =%d to win\n", x, y );
	int match = 1;
	int startX = x;
	int endX = x;
	for (int i = 1; i < 5; ++i) {
		int posX = x + i;
		if (posX > 17 || (currentMove->getGameField()[y])[posX] != currentMove->getCurrentType())
			break;
		else if(++match == 5){
			endX = posX;
			printf("match for x =%d and y =%d is %d\n", x, y, match );
			return checkPossibleCaptureHorizontalWin(currentMove, startX, endX, y);
		}
		printf("match for x =%d and y =%d is %d\n", x, y, match );
	}
	for (int i = 1; i < 5 ; ++i) {
		int posX = x - i;
		if (posX < 0 || (currentMove->getGameField()[y])[posX] != currentMove->getCurrentType())
			break;
		else if(++match == 5){
			startX = posX;
			printf("match for x =%d and y =%d is %d\n", x, y, match );
			return checkPossibleCaptureHorizontalWin(currentMove, startX, endX, y);
		}
		printf("match for x =%d and y =%d is %d\n", x, y, match );
	}
	return 0;
}



int 			Gomoku::checkWinVertical(Move* currentMove, int x, int y)
{
		printf("checkWinVertical x =%d and y =%d to win\n", x, y );
	int match = 1;
	int startY = y;
	int endY = y;
	for (int i = 1; i < 5; ++i) {
		int posY = y + i;
		if (posY > 17 || (currentMove->getGameField()[posY])[x] != currentMove->getCurrentType())
			break;
		else if(++match == 5) {
			endY = posY;
			return checkPossibleCaptureVerticalWin(currentMove, startY,endY, x);
		}
	}
	for (int i = 1; i < 5 ; ++i){
		int posY = y - i;
		if (posY < 0 || (currentMove->getGameField()[posY])[x] != currentMove->getCurrentType())
			break;
		else if (++match == 5){
			startY = posY;
			return checkPossibleCaptureVerticalWin(currentMove, startY,endY, x);
		}
	}
	return 0;

}

int 			Gomoku::checkWinDiagonalLeft(Move* currentMove, int x, int y)
{
	printf("checkWinDiagonalLeft x =%d and y =%d to win\n", x, y );
	int match = 1;
	t_pos start;
	start.x = x;
	start.y = y;

	for (int i = 1; i < 5; ++i) {
		int posX = x + i;
		int posY = y + i;
		if (posX > 17 || posY > 17 || (currentMove->getGameField()[posY])[posX] != currentMove->getCurrentType())
			break;
		else if (++match == 5) {
			return checkPossibleCaptureDiagonalLeftWin(currentMove, start);//1;
		}
	}
	for (int i = 1; i < 5 ; ++i){
		int posX = x - i;
		int posY = y - i;
		if (posX < 0 || posY < 0 || (currentMove->getGameField()[posY])[posX] != currentMove->getCurrentType())
			break;
		else if (++match == 5){
			start.x = posX;
			start.y = posY;
			return checkPossibleCaptureDiagonalLeftWin(currentMove, start);
		}
	}
	return 0;
}

int 			Gomoku::checkWinDiagonalRight(Move* currentMove, int x, int y)
{
	printf("checkWinDiagonalRight x =%d and y =%d to win\n", x, y );
	int match = 1;
	t_pos start;
	start.x = x;
	start.y = y;
	for (int i = 1; i < 5; ++i) {
		int posX = x - i;
		int posY = y + i;
		if (posX < 0 || posY > 17 || (currentMove->getGameField()[posY])[posX] != currentMove->getCurrentType())
			break;
		else if(++match == 5) {
			return checkPossibleCaptureDiagonalRightWin(currentMove, start);
		}
	}
	for (int i = 1; i < 5 ; ++i){
		int posX = x + i;
		int posY = y - i;
		if (posX > 17 || posY < 0 || (currentMove->getGameField()[posY])[posX] != currentMove->getCurrentType())
			break;
		else if (++match == 5){
			start.x = posX;
			start.y = posY;
			return  checkPossibleCaptureDiagonalRightWin(currentMove, start);
		}
	}
	return 0;
}

eType			Gomoku::findOppositeType(eType type)
{
	return type == BLACK ? WHITE : (type == WHITE ? BLACK : EMPTY);
}

void			Gomoku::eraseTiles(std::array<typeArr, N> *gameField, int startX, int startY, int endX, int endY)
{
	printf("ERASE TILE x =%d and y =%d x2 =%d and y2 =%d\n", startX, startY,  endX, endY);
	if (startX < 0 || startX > 17 || endX > 17
		|| startY < 0 || startY > 17 || endY > 17 )
		return;
	if (startX == endX){
		for (int i = startY; i <= endY; ++i) {
			printf("ERASE TILE x =%d and y =%d\n", startX, i );
			(*gameField)[i][startX] = EMPTY;
		}
		return;
	}

	if (startY == endY){
		for (int i = startX; i <= endX; ++i) {
			printf("ERASE TILE x =%d and y =%d\n", i, startY );
			(*gameField)[startY][i] = EMPTY;
		}
		return;
	}
	int j = startX;
	for (int i = startY; i <= endY; ++i) {
			printf("ERASE TILE x =%d and y =%d\n", j, i );
			(*gameField)[i][j] = EMPTY;
			if (startX < endX)
				j++;
			else
				j--;
	}
}

int 			Gomoku::checkCapture(Move* currentMove, int x, int y)
{
	printf("checkCapture x =%d and y =%d to win\n", x, y );
	int captures = 0;
	printf("checkCaptureHorizontal x =%d and y =%d captures = %d\n", x, y, captures );
	captures += checkCaptureHorizontal(currentMove, x, y);
	printf("checkCaptureVertical x =%d and y =%d captures = %d\n", x, y, captures );
	captures += checkCaptureVertical(currentMove, x, y);
	printf("checkCaptureDiagonalLeft x =%d and y =%d captures = %d\n", x, y, captures );
	captures += checkCaptureDiagonalLeft(currentMove, x, y);
	printf("checkCaptureDiagonalRight x =%d and y =%d captures = %d\n", x, y, captures );
	captures += checkCaptureDiagonalRight(currentMove, x, y);
	printf("checkCapture x =%d and y =%d captures = %d\n", x, y, captures );
	if (currentMove->getCurrentType() == WHITE)
		currentMove->setWhiteCapture(currentMove->getWhiteCapture() + captures);
	else
	 	currentMove->setBlackCapture (currentMove->getBlackCapture() + captures);
	return captures;
}

int 			Gomoku::checkCaptureHorizontal(Move* currentMove, int x, int y)
{
	int capture = 0;
	int cursor = 0;
	for (cursor = 1; cursor < 4; ++cursor) {
		int posX = x + cursor;
	//	printf("Chek type for capture x =%d and y =%d\n type = %d current move = %d iteration = %d\n", posX, y, (int)findOppositeType(currentMove->getCurrentType()), (int)(currentMove->getGameField()[y])[posX], cursor);
		if (cursor < 3
			&& (posX > 17 || (currentMove->getGameField()[y])[posX] != findOppositeType(currentMove->getCurrentType())))
				break;
		if (cursor == 3
			 &&	(posX > 17 || (currentMove->getGameField()[y])[posX] != currentMove->getCurrentType()))
				break;
	}
	if (cursor == 4){
		//cursor = 0;
		//printf("Chek capture x =%d and y =%d\n", x, y );
		eraseTiles(&(const_cast<std::array<typeArr, N>  &>(currentMove->getGameField())), x + 1 , y, x + 2, y);
		capture++;
	}
	for (cursor = 1; cursor < 4; ++cursor) {
		int posX = x - cursor;
		//printf("Chek type for capture to left x =%d and y =%d\n type = %d current move = %d iteration = %d\n", posX, y, (int)findOppositeType(currentMove->getCurrentType()), (int)(currentMove->getGameField()[y])[posX], cursor);
		if (cursor < 3
			&& (posX < 0 || (currentMove->getGameField()[y])[posX] != findOppositeType(currentMove->getCurrentType())))
				break;
		if (cursor == 3
			&& (posX < 0 || (currentMove->getGameField()[y])[posX] != currentMove->getCurrentType()))
				break;
	}
	if (cursor == 4){
		//printf("Chek capture x =%d and y =%d\n", x, y );
		eraseTiles(&(const_cast<std::array<typeArr, N>  &>(currentMove->getGameField())), x - 2 , y, x -1, y);
		capture++;
	}
	return capture;
}

int 			Gomoku::checkCaptureVertical(Move* currentMove, int x, int y)
{
	int capture = 0;
	int cursor = 0;
	for (cursor = 1; cursor < 4; ++cursor) {
		int posY = y + cursor;
		//printf("Chek type for capture x =%d and y =%d\n type = %d current move = %d iteration = %d\n", posY, y, (int)findOppositeType(currentMove->getCurrentType()), (int)(currentMove->getGameField()[posY])[x], cursor);
		if (cursor < 3
			&& (posY > 17 || (currentMove->getGameField()[posY])[x] != findOppositeType(currentMove->getCurrentType())))
				break;
		if (cursor == 3
			 &&	(posY > 17 || (currentMove->getGameField()[posY])[x] != currentMove->getCurrentType()))
				break;
	}
	if (cursor == 4){
		//cursor = 0;
		//printf("Chek capture x =%d and y =%d\n", x, y );
		eraseTiles(&(const_cast<std::array<typeArr, N>  &>(currentMove->getGameField())), x , y + 1, x , y + 2);
		capture++;
	}
	for (cursor = 1; cursor < 4; ++cursor) {
		int posY = y - cursor;
		//printf("Chek type for capture to left x =%d and y =%d\n type = %d current move = %d iteration = %d\n", posX, y, (int)findOppositeType(currentMove->getCurrentType()), (int)(currentMove->getGameField()[y])[posX], cursor);
		if (cursor < 3
			&& (posY < 0 || (currentMove->getGameField()[posY])[x] != findOppositeType(currentMove->getCurrentType())))
				break;
		if (cursor == 3
			&& (posY < 0 || (currentMove->getGameField()[posY])[x] != currentMove->getCurrentType()))
				break;
	}
	if (cursor == 4){
		//printf("Chek capture x =%d and y =%d\n", x, y );
		eraseTiles(&(const_cast<std::array<typeArr, N>  &>(currentMove->getGameField())), x , y - 2, x, y - 1);
		capture++;
	}
	return capture;
}
//
int 			Gomoku::checkCaptureDiagonalLeft(Move* currentMove, int x, int y)
{
	int capture = 0;
	int cursor = 0;
	for (cursor = 1; cursor < 4; ++cursor) {
		int posX = x + cursor;
		int posY = y + cursor;
	//	printf("Chek type for capture x =%d and y =%d\n type = %d current move = %d iteration = %d\n", posX, posY, (int)findOppositeType(currentMove->getCurrentType()), (int)(currentMove->getGameField()[posY])[posX], cursor);
		if (cursor < 3
			&& (posY > 17 || posX > 17 || (currentMove->getGameField()[posY])[posX] != findOppositeType(currentMove->getCurrentType())))
				break;
		if (cursor == 3
			 &&	(posY > 17 || posX > 17 || (currentMove->getGameField()[posY])[posX] != currentMove->getCurrentType()))
				break;
	}
	if (cursor == 4){
		//cursor = 0;
		printf("Chek capture x =%d and y =%d\n", x, y );
		eraseTiles(&(const_cast<std::array<typeArr, N>  &>(currentMove->getGameField())), x + 1 , y + 1, x + 2 , y + 2);
		capture++;
	}
	for (cursor = 1; cursor < 4; ++cursor) {
		int posX = x - cursor;
		int posY = y - cursor;
		//printf("Chek type for capture to left x =%d and y =%d\n type = %d current move = %d iteration = %d\n", posX, y, (int)findOppositeType(currentMove->getCurrentType()), (int)(currentMove->getGameField()[y])[posX], cursor);
		if (cursor < 3
			&& (posY < 0 || posX  < 0 || (currentMove->getGameField()[posY])[posX] != findOppositeType(currentMove->getCurrentType())))
				break;
		if (cursor == 3
			&& (posY < 0 || posX  < 0 || (currentMove->getGameField()[posY])[posX] != currentMove->getCurrentType()))
				break;
	}
	if (cursor == 4){
		printf("Chek capture x =%d and y =%d\n", x, y );
		eraseTiles(&(const_cast<std::array<typeArr, N>  &>(currentMove->getGameField())), x - 2 , y - 2, x - 1, y - 1);
		capture++;
	}
	return capture;
}

int 			Gomoku::checkCaptureDiagonalRight(Move* currentMove, int x, int y)
{
	int capture = 0;
	int cursor = 0;
	for (cursor = 1; cursor < 4; ++cursor) {
		int posX = x - cursor;
		int posY = y + cursor;
	//	printf("Chek type for capture x =%d and y =%d\n type = %d current move = %d iteration = %d\n", posX, posY, (int)findOppositeType(currentMove->getCurrentType()), (int)(currentMove->getGameField()[posY])[posX], cursor);
		if (cursor < 3
			&& (posY > 17 || posX < 0 || (currentMove->getGameField()[posY])[posX] != findOppositeType(currentMove->getCurrentType())))
				break;
		if (cursor == 3
			 &&	(posY > 17 || posX < 0 || (currentMove->getGameField()[posY])[posX] != currentMove->getCurrentType()))
				break;
	}
	if (cursor == 4){
		//cursor = 0;
		printf("Chek capture x =%d and y =%d\n", x, y );
		eraseTiles(&(const_cast<std::array<typeArr, N>  &>(currentMove->getGameField())), x - 1 , y + 1, x - 2 , y + 2);
		capture++;
	}
	for (cursor = 1; cursor < 4; ++cursor) {
		int posX = x + cursor;
		int posY = y - cursor;
		//printf("Chek type for capture to left x =%d and y =%d\n type = %d current move = %d iteration = %d\n", posX, y, (int)findOppositeType(currentMove->getCurrentType()), (int)(currentMove->getGameField()[y])[posX], cursor);
		if (cursor < 3
			&& (posY < 0 || posX  > 17 || (currentMove->getGameField()[posY])[posX] != findOppositeType(currentMove->getCurrentType())))
				break;
		if (cursor == 3
			&& (posY < 0 || posX  > 17 || (currentMove->getGameField()[posY])[posX] != currentMove->getCurrentType()))
				break;
	}
	if (cursor == 4){
		printf("Chek capture x =%d and y =%d\n", x, y );
		eraseTiles(&(const_cast<std::array<typeArr, N>  &>(currentMove->getGameField())), x + 2 , y - 2, x  + 1, y - 1);
		capture++;
	}
	return capture;
}

int 			Gomoku::checkFreeTree(Move* currentMove, int x, int y)
{
	int free_tree = 0;
printf("Chek free_tree x =%d and y =%d free_tree = %d\n", x, y, free_tree);
	free_tree += checkFreeTreeHorizontal(currentMove, x, y);
	free_tree += checkFreeTreeVertical(currentMove, x, y);
	free_tree += checkFreeTreeDiagonalLeft(currentMove, x, y);
	free_tree +=checkFreeTreeDiagonalRight(currentMove, x, y);
	printf("Chek free_tree x =%d and y =%d free_tree = %d\n", x, y, free_tree);
	//printf("currentMove->whiteFreeTree.size() %lu\n", currentMove->whiteFreeTree.size());
	// for (size_t i = 0; i < currentMove->whiteFreeTree.size(); ++i) {
	// 	posSet set1 = (currentMove->whiteFreeTree)[i];
	// for (size_t j = 0; j < set1.size(); ++j)
	// {
	// 	printf("print whiteFreeTree\n");
	// 	t_pos tile =set1[j];
	// 	printf(" x =%d and y =%d is %d\n", tile.x, tile.y, tile.type);
	// }
	// }
	//printf("currentMove->blackFreeTree.size() %lu\n", currentMove->blackFreeTree.size());
	// for (size_t i = 0; i < currentMove->blackFreeTree.size(); ++i) {
	// 	posSet set = currentMove->blackFreeTree[i];
	// 	for (size_t j = 0; j < set.size(); ++j)
	// 	{
	// 		printf("print blackFreeTree\n");
	// 		t_pos tile = set[j];
	// 		printf(" x =%d and y =%d is %d\n", tile.x, tile.y, tile.type);
	// 	}
	// }
	if (free_tree > 1) {
		return 1;
	}  else if (free_tree == 1) {
		printf ("checkcheckDoubleFreeTree current turn is %d\n", currentMove->getCurrentType());
		return checkDoubleFreeTree(*currentMove);
	}
	return 0;
}

int			Gomoku::checkDoubleFreeTree(Move &currentMove)
{
	int doubleFreeThree = 0;
	std::array<typeArr, N>  & gamefield = const_cast<std::array<typeArr, N>  &>(currentMove.getGameField());
	for (int x = 0; x < 18; ++x){
		for (int y = 0; y < 18; ++y) {
			if (gamefield[y][x] != (currentMove.getCurrentType()))//{
				continue;
			doubleFreeThree += checkFreeTreeHorizontal(&currentMove, x, y);
			doubleFreeThree += checkFreeTreeVertical(&currentMove, x, y);
			doubleFreeThree += checkFreeTreeDiagonalLeft(&currentMove, x, y);
			doubleFreeThree += checkFreeTreeDiagonalRight(&currentMove, x, y);
			printf ("x = %d y = %d doubleFreeThree = %d\n", x, y, doubleFreeThree);
			if (doubleFreeThree){
				printf ("Double FREE THREE!!!!!\n");
				return 1;
			}
		}
	}
	return 0;
}
inline int		Gomoku::validFreeThreeHorisontal(Move & currentMove, t_pos & start, t_pos & end,
													int y)
{
	std::array<typeArr, N>  & gamefield = const_cast<std::array<typeArr, N>  &>(currentMove.getGameField());
	if (end.x + 1 > 17 || start.x - 1 < 0 || end.x - start.x > 3)
		return 0;
	if (gamefield[y][end.x + 1] != EMPTY)
		return 0;
	if  (gamefield[y][start.x - 1] != EMPTY)
	{
		if  (gamefield[y][start.x - 1] == currentMove.getCurrentType()
				&& start.x - 2 >= 0
				&& (gamefield[y][start.x + 2] == EMPTY || gamefield[y][start.x + 1] == EMPTY))
		{
			start.x--;
			end.x--;
		}
		 else
			return 0;
	}
	return 1;
}

inline int		Gomoku::validFreeThreeVertical(Move & currentMove, t_pos & start, t_pos & end,
													int x)
{
	if (end.y + 1 > 17 || start.y - 1 < 0 || end.y - start.y > 3)
		return 0;
	std::array<typeArr, N>  & gamefield = const_cast<std::array<typeArr, N>  &>(currentMove.getGameField());
	if (gamefield[end.y + 1][x] != EMPTY)
		return 0;
	if  (gamefield[start.y - 1][x] != EMPTY){
		if  (gamefield[start.y - 1][x] == currentMove.getCurrentType()
				&& start.y - 2 >= 0
					&& (gamefield[start.y + 2][x] == EMPTY || gamefield[start.y + 1][x] == EMPTY)) {
			start.y--;
			end.y--;
		}
		 else
			return 0;
		}
	return 1;
}

inline int		Gomoku::validFreeThreeDiagonalLeft(Move & currentMove, t_pos & start, t_pos & end)
{
	if (end.y + 1 > 17 || start.y - 1 < 0 || end.y - start.y > 3)
		return 0;
	if (end.x + 1 > 17 || start.x - 1 < 0 || end.x - start.x > 3)
		return 0;
	std::array<typeArr, N>  & gamefield = const_cast<std::array<typeArr, N>  &>(currentMove.getGameField());

	if (gamefield[end.y + 1][end.x + 1] != EMPTY)
		return 0;
	if  (gamefield[start.y - 1][start.x - 1] != EMPTY)
	{
		if  (gamefield[start.y - 1][start.x - 1] == currentMove.getCurrentType()
				&& start.y - 2 > 0 && start.x - 2 > 0
					&& (gamefield[start.y + 2][start.x + 2] == EMPTY || gamefield[start.y + 1][start.x + 1] == EMPTY)){
			start.y--;
			end.y--;
			start.x--;
			end.x--;
		}
		 else
			return 0;
	}
	return 1;
}

inline int			Gomoku::validFreeThreeDiagonalRight(Move & currentMove, t_pos & start, t_pos & end)
{
	if (end.y + 1 > 17 || start.y - 1 < 0 || end.y - start.y > 3)
		return 0;
	if (end.x - 1 < 0 || start.x + 1 > 17 || end.x - start.x > 3)
		return 0;
	std::array<typeArr, N>  & gamefield = const_cast<std::array<typeArr, N>  &>(currentMove.getGameField());
	if (gamefield[end.y + 1][end.x - 1] != EMPTY)
		return 0;
	if  (gamefield[start.y - 1][start.x + 1] != EMPTY){
		if  (gamefield[start.y - 1][start.x + 1] == currentMove.getCurrentType()
				&& start.y - 2 > 0 && start.x + 2 < 17
					&& (gamefield[start.y + 2][start.x - 2] == EMPTY || gamefield[start.y + 1][start.x - 1] == EMPTY)){
			start.y--;
			end.y--;
			start.x++;
			end.x++;
		}
		 else
			return 0;
	}
	return 1;
}



std::function< bool (const t_pos & , const t_pos &) > comparator = [](const t_pos & left, const t_pos & right){
	return (left.x == right.x && left.y == right.y && left.type == right.type);
										// // Lambda function to compare 2 strings in case insensitive manner
										// return std::equal(left.begin(), left.end(), right.begin(), [](t_pos & l, t_pos & r) {
										// 															return (l.x == r.x && l.y == r.y && l.type == r.type));
										// 													});
									};

inline int 				Gomoku::validMatchFreeTreeSet(posSet const & left, posSet const & right)
{
	int match = 0;
	eType matcType = BLACK;

	for (size_t i = 0; i < left.size(); ++i){
		if (match >= 2){
			return 1;
		}

		for (size_t j = 0; j < right.size(); ++j)
		{
			if (left[i].x == right[j].x && left[i].y == right[j].y && left[i].type == right[j].type){
				matcType = left[i].type;
				match++;
			}
		}
	}

	if (match == 1 && matcType == EMPTY){
		return 1;
	}
	return 0;
}

inline int				Gomoku::fillThreeSet(Move & currentMove, posSet const & examinedSet)
{
	if ( m_currentMove.getCurrentType() == BLACK) {
		if (currentMove.getBlackFreeThree().empty())
			currentMove.setBlackFreeThree(examinedSet);
		else
			return validMatchFreeTreeSet(currentMove.getBlackFreeThree(), examinedSet) ? 0 : 1;
	} else if ( m_currentMove.getCurrentType() == WHITE){
		if (currentMove.getWhiteFreeThree().empty())
			currentMove.setWhiteFreeThree(examinedSet);
		else
			return validMatchFreeTreeSet(currentMove.getWhiteFreeThree(), examinedSet) ? 0 : 1;
	}
	return 1;
}


int 				Gomoku::fillHorisontalFreeTreeSet(Move* currentMove, int x, int y, t_pos const & start, t_pos const & end)
{
	posSet freeTreeSet;

	for (int i = start.x - 1; i <= end.x + 1; ++i)
	{
		t_pos tile;
		tile.x = i;
		tile.y = y;
		if (tile.x == x && tile.y == y)
			tile.type = currentMove->getCurrentType();
		else
			tile.type = (eType)(currentMove->getGameField()[tile.y])[tile.x];
		freeTreeSet.push_back(tile);
		//
		// printf("pushTile start.x - 1 = %d; end.x + 1 = %d \n", start.x - 1 ,end.x + 1);
		// printf("pushTile tile->x = %d; tile->y = %d tile->type = %u\n", tile.x, tile.y, tile.type);
	}
	return fillThreeSet(*currentMove, freeTreeSet);
}

int 			Gomoku::fillVerticalFreeTreeSet(Move* currentMove, int x, int y, t_pos const & start, t_pos const & end)
{
	posSet freeTreeSet;
	for (int i = start.y - 1; i <= end.y + 1; ++i)
	{
		t_pos tile;
		tile.x = x;
		tile.y = i;
		if (tile.x == x && tile.y == y)
			tile.type = currentMove->getCurrentType();
		else
			tile.type = (eType)(currentMove->getGameField()[tile.y])[tile.x];
		freeTreeSet.push_back(tile);
	}
	return fillThreeSet(*currentMove, freeTreeSet);
}

int				Gomoku::fillDiagonalLeftFreeTreeSet(Move* currentMove, int x, int y, t_pos const & start, t_pos const & end)
{
	posSet freeTreeSet;
	int size = end.y - start.y + 2;
	for (int i = 0; i < size; ++i)
	{
		t_pos tile;
		tile.x = start.x + i - 1;
		tile.y = start.y + i - 1;
		if (tile.x == x && tile.y == y)
			tile.type = currentMove->getCurrentType();
		else
			tile.type = (eType)(currentMove->getGameField()[tile.y])[tile.x];
		freeTreeSet.push_back(tile);
	}
	return fillThreeSet(*currentMove, freeTreeSet);
}

int 			Gomoku::fillDiagonalRightFreeTreeSet(Move* currentMove, int x, int y, t_pos const & start, t_pos const & end)
{
	posSet freeTreeSet;
	int size = end.y - start.y + 2;
	for (int i = 0; i < size; ++i)
	{
		t_pos tile;
		tile.x = start.x - i + 1;
		tile.y = start.y + i - 1;
		if (tile.x == x && tile.y == y)
			tile.type = currentMove->getCurrentType();
		else
			tile.type = (eType)(currentMove->getGameField()[tile.y])[tile.x];
		freeTreeSet.push_back(tile);
	}
	return fillThreeSet(*currentMove, freeTreeSet);
}

int 			Gomoku::checkFreeTreeHorizontal(Move* currentMove, int x, int y)
{
	int matchCurrentType = 1;
	int matchEmpty = 0;
	t_pos start;
	t_pos end;
	start.x = x;
	start.y = y;
	end.y = y;
	end.x = x;

	for (int i = 1; i < 4; ++i) {
		int posX = x + i;
		if (posX > 17 || (currentMove->getGameField()[y])[posX] == findOppositeType(currentMove->getCurrentType()))
			break;
		if ((currentMove->getGameField()[y])[posX] == currentMove->getCurrentType()) {
			end.x = posX;
			matchCurrentType++;
		} else {
			matchEmpty++;
		}
		if (matchEmpty == 2) {
			break;
		}
		if(matchCurrentType == 3){
			if (!validFreeThreeHorisontal(*currentMove, start, end, y))
				break;
			return fillHorisontalFreeTreeSet(currentMove, x, y, start, end);
		}
	}
	matchEmpty = 0;
	for (int i = 1; i < 4 ; ++i) {
		int posX = x - i;
		if (posX < 0 ||  (currentMove->getGameField()[y])[posX] == findOppositeType(currentMove->getCurrentType()))
			break;

			if ((currentMove->getGameField()[y])[posX] == currentMove->getCurrentType()) {
				matchCurrentType++;
				start.x = posX;
			} else {
				matchEmpty++;
			}
			if (matchEmpty == 2) {
				break;
			}

		if (matchCurrentType == 3) {
			if (!validFreeThreeHorisontal(*currentMove, start, end, y))
				break;
			return fillHorisontalFreeTreeSet(currentMove, x, y, start, end);
		}
	}
	return 0;
}

int 			Gomoku::checkFreeTreeVertical(Move* currentMove, int x, int y)
{
	int matchCurrentType = 1;
	int matchEmpty = 0;
	t_pos start;
	start.x = x;
	start.y = y;
	t_pos end;
	end.y = y;
	end.x = x;

	for (int i = 1; i < 4; ++i) {
		int posY = y + i;
		if (posY > 17 || (currentMove->getGameField()[posY])[x] == findOppositeType(currentMove->getCurrentType()))
			break;
		if ((currentMove->getGameField()[posY])[x] == currentMove->getCurrentType()) {
			end.y = posY;
			matchCurrentType++;
		} else {
			matchEmpty++;
		}
		if (matchEmpty == 2){
			break;
		}
		if(matchCurrentType == 3){
			if (!validFreeThreeVertical(*currentMove, start, end, x))
				break;
			return fillVerticalFreeTreeSet(currentMove, x, y, start, end);
		}
	}
	matchEmpty = 0;
	for (int i = 1; i < 4 ; ++i) {
		int posY = y - i;
		if (posY < 0 ||  (currentMove->getGameField()[posY])[x] == findOppositeType(currentMove->getCurrentType()))
			break;

			if ((currentMove->getGameField()[posY])[x] == currentMove->getCurrentType()) {
				matchCurrentType++;
				start.y = posY;
			} else {
				matchEmpty++;
			}
			if (matchEmpty == 2){
				break;
			}

		if (matchCurrentType == 3) {
			if (!validFreeThreeVertical(*currentMove, start, end, x))
				break;
			return fillVerticalFreeTreeSet(currentMove, x, y, start, end);
		}
	}
	return 0;
}

int 			Gomoku::checkFreeTreeDiagonalLeft(Move* currentMove, int x, int y)
{
	int matchCurrentType = 1;
	int matchEmpty = 0;
	t_pos start;
	start.x = x;
	start.y = y;
	t_pos end;
	end.y = y;
	end.x = x;

	for (int i = 1; i < 4; ++i) {
		int posY = y + i;
		int posX = x + i;
		if (posY > 17 || posX > 17 || (currentMove->getGameField()[posY])[posX] == findOppositeType(currentMove->getCurrentType()))
			break;
		if ((currentMove->getGameField()[posY])[posX] == currentMove->getCurrentType()) {
			end.y = posY;
			end.x = posX;
			matchCurrentType++;
		} else {
			matchEmpty++;
		}
		if (matchEmpty == 2){
			break;
		}
		if(matchCurrentType == 3){
			if (!validFreeThreeDiagonalLeft(*currentMove, start, end))
				break;
			return fillDiagonalLeftFreeTreeSet(currentMove, x, y, start, end);
		}
	}
	matchEmpty = 0;
	for (int i = 1; i < 4 ; ++i) {
		int posY = y - i;
		int posX = x - i;
		if (posY < 0 ||  (currentMove->getGameField()[posY])[posX] == findOppositeType(currentMove->getCurrentType()))
			break;
			if ((currentMove->getGameField()[posY])[posX] == currentMove->getCurrentType()) {
				matchCurrentType++;
				start.y = posY;
				start.x = posX;
			} else {
				matchEmpty++;
			}
			if (matchEmpty == 2){
				break;
			}

		if (matchCurrentType == 3) {
			if (!validFreeThreeDiagonalLeft(*currentMove, start, end))
				break;
			return fillDiagonalLeftFreeTreeSet(currentMove, x, y, start, end);
		}
	}
	return 0;
}

int 			Gomoku::checkFreeTreeDiagonalRight(Move* currentMove, int x, int y)
{
	int matchCurrentType = 1;
	int matchEmpty = 0;
	t_pos start;
	start.x = x;
	start.y = y;
	t_pos end;
	end.y = y;
	end.x = x;

	for (int i = 1; i < 4; ++i) {
		int posY = y + i;
		int posX = x - i;
		if (posY > 17 || posX < 0 || (currentMove->getGameField()[posY])[posX] == findOppositeType(currentMove->getCurrentType()))
			break;
		if ((currentMove->getGameField()[posY])[posX] == currentMove->getCurrentType()) {
			end.y = posY;
			end.x = posX;
			matchCurrentType++;
		} else {
			matchEmpty++;
		}
		if (matchEmpty == 2){
			break;
		}
		if(matchCurrentType == 3){
			if (!validFreeThreeDiagonalRight(*currentMove, start, end))
				break;
			return fillDiagonalRightFreeTreeSet(currentMove, x, y, start, end);
		}
	}
	matchEmpty = 0;
	for (int i = 1; i < 4 ; ++i) {
		int posY = y - i;
		int posX = x + i;
		if (posY < 0 ||  (currentMove->getGameField()[posY])[posX] == findOppositeType(currentMove->getCurrentType()))
			break;
			if ((currentMove->getGameField()[posY])[posX] == currentMove->getCurrentType()) {
				matchCurrentType++;
				start.y = posY;
				start.x = posX;
			} else {
				matchEmpty++;
			}
			if (matchEmpty == 2){
				break;
			}

		if (matchCurrentType == 3) {
			if (!validFreeThreeDiagonalRight(*currentMove, start, end))
				break;
			return fillDiagonalRightFreeTreeSet(currentMove, x, y, start, end);
		}
	}
	return 0;
}
