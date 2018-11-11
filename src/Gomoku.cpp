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
	currentMove->whiteFreeTree.clear();
	currentMove->blackFreeTree.clear();
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
		currentMove->moveResult = (currentMove->whiteCaptures >= 5 || currentMove->blackCaptures >= 5) ? WIN : CAPTURE;
	} else if (checkFreeTree(currentMove, m_event.x, m_event.y)){
		currentMove->moveResult = DOUBLE_FREE_TREE;
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
	} else if (currentMove->moveResult == DOUBLE_FREE_TREE){
		currentMove->whiteFreeTree.clear();
		currentMove->blackFreeTree.clear();
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
	printf("Chek x =%d and y =%d to win return 0\n", m_event.x, m_event.y );
	return 0;
}

int 			Gomoku::checkWinHorizontal(t_move* currentMove, int x, int y)
{
		printf("checkWinHorizontalx =%d and y =%d to win\n", x, y );
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
		printf("checkWinVertical x =%d and y =%d to win\n", x, y );
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
	printf("checkWinDiagonalLeft x =%d and y =%d to win\n", x, y );
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
	printf("checkWinDiagonalRight x =%d and y =%d to win\n", x, y );
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
	int j = startX;
	for (int i = startY; i <= endY; ++i) {
			printf("ERASE TILE x =%d and y =%d\n", j, i );
			(*(*gameField)[i])[j] = EMPTY;
			if (startX < endX)
				j++;
			else
				j--;
		// for (int j = startX; j <= endX; ++j){
		// 	printf("ERASE TILE x =%d and y =%d\n", j, i );
		// 	(*(*gameField)[i])[j] = EMPTY;
		// }
	}
}

int 			Gomoku::checkCapture(t_move* currentMove, int x, int y)
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
	//	printf("Chek type for capture x =%d and y =%d\n type = %d current move = %d iteration = %d\n", posX, y, (int)findOppositeType(currentMove->currentTurn), (int)(*currentMove->gameField[y])[posX], cursor);
		if (cursor < 3
			&& (posX > 17 || (*currentMove->gameField[y])[posX] != findOppositeType(currentMove->currentTurn)))
				break;
		if (cursor == 3
			 &&	(posX > 17 || (*currentMove->gameField[y])[posX] != currentMove->currentTurn))
				break;
	}
	if (cursor == 4){
		//cursor = 0;
		//printf("Chek capture x =%d and y =%d\n", x, y );
		eraseTiles(&currentMove->gameField, x + 1 , y, x + 2, y);
		capture++;
	}
	for (cursor = 1; cursor < 4; ++cursor) {
		int posX = x - cursor;
		//printf("Chek type for capture to left x =%d and y =%d\n type = %d current move = %d iteration = %d\n", posX, y, (int)findOppositeType(currentMove->currentTurn), (int)(*currentMove->gameField[y])[posX], cursor);
		if (cursor < 3
			&& (posX < 0 || (*currentMove->gameField[y])[posX] != findOppositeType(currentMove->currentTurn)))
				break;
		if (cursor == 3
			&& (posX < 0 || (*currentMove->gameField[y])[posX] != currentMove->currentTurn))
				break;
	}
	if (cursor == 4){
		//printf("Chek capture x =%d and y =%d\n", x, y );
		eraseTiles(&currentMove->gameField, x - 2 , y, x -1, y);
		capture++;
	}
	return capture;
}

int 			Gomoku::checkCaptureVertical(t_move* currentMove, int x, int y)
{
	int capture = 0;
	int cursor = 0;
	for (cursor = 1; cursor < 4; ++cursor) {
		int posY = y + cursor;
		//printf("Chek type for capture x =%d and y =%d\n type = %d current move = %d iteration = %d\n", posY, y, (int)findOppositeType(currentMove->currentTurn), (int)(*currentMove->gameField[posY])[x], cursor);
		if (cursor < 3
			&& (posY > 17 || (*currentMove->gameField[posY])[x] != findOppositeType(currentMove->currentTurn)))
				break;
		if (cursor == 3
			 &&	(posY > 17 || (*currentMove->gameField[posY])[x] != currentMove->currentTurn))
				break;
	}
	if (cursor == 4){
		//cursor = 0;
		//printf("Chek capture x =%d and y =%d\n", x, y );
		eraseTiles(&currentMove->gameField, x , y + 1, x , y + 2);
		capture++;
	}
	for (cursor = 1; cursor < 4; ++cursor) {
		int posY = y - cursor;
		//printf("Chek type for capture to left x =%d and y =%d\n type = %d current move = %d iteration = %d\n", posX, y, (int)findOppositeType(currentMove->currentTurn), (int)(*currentMove->gameField[y])[posX], cursor);
		if (cursor < 3
			&& (posY < 0 || (*currentMove->gameField[posY])[x] != findOppositeType(currentMove->currentTurn)))
				break;
		if (cursor == 3
			&& (posY < 0 || (*currentMove->gameField[posY])[x] != currentMove->currentTurn))
				break;
	}
	if (cursor == 4){
		//printf("Chek capture x =%d and y =%d\n", x, y );
		eraseTiles(&currentMove->gameField, x , y - 2, x, y - 1);
		capture++;
	}
	return capture;
}
//
int 			Gomoku::checkCaptureDiagonalLeft(t_move* currentMove, int x, int y)
{
	int capture = 0;
	int cursor = 0;
	for (cursor = 1; cursor < 4; ++cursor) {
		int posX = x + cursor;
		int posY = y + cursor;
	//	printf("Chek type for capture x =%d and y =%d\n type = %d current move = %d iteration = %d\n", posX, posY, (int)findOppositeType(currentMove->currentTurn), (int)(*currentMove->gameField[posY])[posX], cursor);
		if (cursor < 3
			&& (posY > 17 || posX > 17 || (*currentMove->gameField[posY])[posX] != findOppositeType(currentMove->currentTurn)))
				break;
		if (cursor == 3
			 &&	(posY > 17 || posX > 17 || (*currentMove->gameField[posY])[posX] != currentMove->currentTurn))
				break;
	}
	if (cursor == 4){
		//cursor = 0;
		printf("Chek capture x =%d and y =%d\n", x, y );
		eraseTiles(&currentMove->gameField, x + 1 , y + 1, x + 2 , y + 2);
		capture++;
	}
	for (cursor = 1; cursor < 4; ++cursor) {
		int posX = x - cursor;
		int posY = y - cursor;
		//printf("Chek type for capture to left x =%d and y =%d\n type = %d current move = %d iteration = %d\n", posX, y, (int)findOppositeType(currentMove->currentTurn), (int)(*currentMove->gameField[y])[posX], cursor);
		if (cursor < 3
			&& (posY < 0 || posX  < 0 || (*currentMove->gameField[posY])[posX] != findOppositeType(currentMove->currentTurn)))
				break;
		if (cursor == 3
			&& (posY < 0 || posX  < 0 || (*currentMove->gameField[posY])[posX] != currentMove->currentTurn))
				break;
	}
	if (cursor == 4){
		printf("Chek capture x =%d and y =%d\n", x, y );
		eraseTiles(&currentMove->gameField, x - 2 , y - 2, x - 1, y - 1);
		capture++;
	}
	return capture;
}

int 			Gomoku::checkCaptureDiagonalRight(t_move* currentMove, int x, int y)
{
	int capture = 0;
	int cursor = 0;
	for (cursor = 1; cursor < 4; ++cursor) {
		int posX = x - cursor;
		int posY = y + cursor;
	//	printf("Chek type for capture x =%d and y =%d\n type = %d current move = %d iteration = %d\n", posX, posY, (int)findOppositeType(currentMove->currentTurn), (int)(*currentMove->gameField[posY])[posX], cursor);
		if (cursor < 3
			&& (posY > 17 || posX < 0 || (*currentMove->gameField[posY])[posX] != findOppositeType(currentMove->currentTurn)))
				break;
		if (cursor == 3
			 &&	(posY > 17 || posX < 0 || (*currentMove->gameField[posY])[posX] != currentMove->currentTurn))
				break;
	}
	if (cursor == 4){
		//cursor = 0;
		printf("Chek capture x =%d and y =%d\n", x, y );
		eraseTiles(&currentMove->gameField, x - 1 , y + 1, x - 2 , y + 2);
		capture++;
	}
	for (cursor = 1; cursor < 4; ++cursor) {
		int posX = x + cursor;
		int posY = y - cursor;
		//printf("Chek type for capture to left x =%d and y =%d\n type = %d current move = %d iteration = %d\n", posX, y, (int)findOppositeType(currentMove->currentTurn), (int)(*currentMove->gameField[y])[posX], cursor);
		if (cursor < 3
			&& (posY < 0 || posX  > 17 || (*currentMove->gameField[posY])[posX] != findOppositeType(currentMove->currentTurn)))
				break;
		if (cursor == 3
			&& (posY < 0 || posX  > 17 || (*currentMove->gameField[posY])[posX] != currentMove->currentTurn))
				break;
	}
	if (cursor == 4){
		printf("Chek capture x =%d and y =%d\n", x, y );
		eraseTiles(&currentMove->gameField, x + 2 , y - 2, x  + 1, y - 1);
		capture++;
	}
	return capture;
}

int 			Gomoku::checkFreeTree(t_move* currentMove, int x, int y)
{
	int free_tree = 0;
printf("Chek free_tree x =%d and y =%d free_tree = %d\n", x, y, free_tree);
	free_tree += checkFreeTreeHorizontal(currentMove, x, y);
	free_tree += checkFreeTreeVertical(currentMove, x, y);
	free_tree += checkFreeTreeDiagonalLeft(currentMove, x, y);
	free_tree +=checkFreeTreeDiagonalRight(currentMove, x, y);
	printf("Chek free_tree x =%d and y =%d free_tree = %d\n", x, y, free_tree);
	printf("currentMove->whiteFreeTree.size() %lu\n", currentMove->whiteFreeTree.size());
	for (size_t i = 0; i < currentMove->whiteFreeTree.size(); ++i) {
		posSet set1 = (currentMove->whiteFreeTree)[i];
	for (size_t j = 0; j < set1.size(); ++j)
	{
		printf("print whiteFreeTree\n");
		t_pos tile =set1[j];
		printf(" x =%d and y =%d is %d\n", tile.x, tile.y, tile.type);
	}
	}
	printf("currentMove->blackFreeTree.size() %lu\n", currentMove->blackFreeTree.size());
	for (size_t i = 0; i < currentMove->blackFreeTree.size(); ++i) {
		posSet set = currentMove->blackFreeTree[i];
		for (size_t j = 0; j < set.size(); ++j)
		{
			printf("print blackFreeTree\n");
			t_pos tile = set[j];
			printf(" x =%d and y =%d is %d\n", tile.x, tile.y, tile.type);
		}
	}
	if (free_tree > 1) {
		return 1;
	}  else if (free_tree == 1) {
		printf ("checkcheckDoubleFreeTree current turn is %d\n", currentMove->currentTurn);
		return checkDoubleFreeTree(*currentMove);
	}
	return 0;
}

int			Gomoku::checkDoubleFreeTree(t_move &currentMove)
{
	int doubleFreeThree = 0;
	for (int x = 0; x < 18; ++x){
		for (int y = 0; y < 18; ++y) {
			if ((*currentMove.gameField[y])[x] != (currentMove.currentTurn))//{
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
inline int		Gomoku::validFreeThreeHorisontal(t_move & currentMove, t_pos & start, t_pos & end,
													int y)
{
	if (end.x + 1 > 17 || start.x - 1 < 0 || end.x - start.x > 3)
		return 0;
	if ((*currentMove.gameField[y])[end.x + 1] != EMPTY)
		return 0;
	if  ((*currentMove.gameField[y])[start.x - 1] != EMPTY)
	{
		if  ((*currentMove.gameField[y])[start.x - 1] == currentMove.currentTurn
				&& start.x - 2 >= 0 && (*currentMove.gameField[y])[start.x + 2] == EMPTY)
		{
			start.x--;
			end.x--;
		}
		 else
			return 0;
	}
	return 1;
}

inline int		Gomoku::validFreeThreeVertical(t_move & currentMove, t_pos & start, t_pos & end,
													int x)
{
	if (end.y + 1 > 17 || start.y - 1 < 0 || end.y - start.y > 3)
		return 0;
	if ((*currentMove.gameField[end.y + 1])[x] != EMPTY)
		return 0;
	if  ((*currentMove.gameField[start.y - 1])[x] != EMPTY){
		if  ((*currentMove.gameField[start.y - 1])[x] == currentMove.currentTurn
				&& start.y - 2 >= 0
					&& (*currentMove.gameField[start.y + 2])[x] == EMPTY){
			start.y--;
			end.y--;
		}
		 else
			return 0;
		}
	return 1;
}

inline int		Gomoku::validFreeThreeDiagonalLeft(t_move & currentMove, t_pos & start, t_pos & end)
{
	if (end.y + 1 > 17 || start.y - 1 < 0 || end.y - start.y > 3)
		return 0;
	if (end.x + 1 > 17 || start.x - 1 < 0 || end.x - start.x > 3)
		return 0;
	if ((*currentMove.gameField[end.y + 1])[end.x + 1] != EMPTY)
		return 0;
	if  ((*currentMove.gameField[start.y - 1])[start.x - 1] != EMPTY)
	{
		if  ((*currentMove.gameField[start.y - 1])[start.x - 1] == currentMove.currentTurn
				&& start.y - 2 > 0 && start.x - 2 > 0
					&& (*currentMove.gameField[start.y + 2])[start.x + 2] == EMPTY){
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

inline int			Gomoku::validFreeThreeDiagonalRight(t_move & currentMove, t_pos & start, t_pos & end)
{
	if (end.y + 1 > 17 || start.y - 1 < 0 || end.y - start.y > 3)
		return 0;
	if (end.x - 1 < 0 || start.x + 1 > 17 || end.x - start.x > 3)
		return 0;
	if ((*currentMove.gameField[end.y + 1])[end.x - 1] != EMPTY)
		return 0;
	if  ((*currentMove.gameField[start.y - 1])[start.x + 1] != EMPTY){
		if  ((*currentMove.gameField[start.y - 1])[start.x + 1] == currentMove.currentTurn
				&& start.y - 2 > 0 && start.x + 2 < 17
					&& (*currentMove.gameField[start.y + 2])[start.x - 2] == EMPTY){
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

inline int				Gomoku::fillThreeSet(t_move & currentMove, posSet const & examinedSet)
{
	if (currentMove.currentTurn == BLACK) {
		for (size_t i = 0; i < currentMove.blackFreeTree.size(); ++i){
			return validMatchFreeTreeSet(currentMove.blackFreeTree[i],examinedSet) ? 0 : 1;
		}
		currentMove.blackFreeTree.push_back(examinedSet);
	} else if (currentMove.currentTurn == WHITE){
		for (size_t i = 0; i < currentMove.whiteFreeTree.size(); ++i){
			return validMatchFreeTreeSet(currentMove.whiteFreeTree[i],examinedSet) ? 0 : 1;
		}
		currentMove.whiteFreeTree.push_back(examinedSet);
	}
	return 1;
}


int 				Gomoku::fillHorisontalFreeTreeSet(t_move* currentMove, int x, int y, t_pos const & start, t_pos const & end)
{
	posSet freeTreeSet;

	for (int i = start.x - 1; i <= end.x + 1; ++i)
	{
		t_pos tile;
		tile.x = i;
		tile.y = y;
		if (tile.x == x && tile.y == y)
			tile.type = currentMove->currentTurn;
		else
			tile.type = (eType)(*currentMove->gameField[tile.y])[tile.x];
		freeTreeSet.push_back(tile);
		//
		// printf("pushTile start.x - 1 = %d; end.x + 1 = %d \n", start.x - 1 ,end.x + 1);
		// printf("pushTile tile->x = %d; tile->y = %d tile->type = %u\n", tile.x, tile.y, tile.type);
	}
	return fillThreeSet(*currentMove, freeTreeSet);
}

int 			Gomoku::fillVerticalFreeTreeSet(t_move* currentMove, int x, int y, t_pos const & start, t_pos const & end)
{
	posSet freeTreeSet;
	for (int i = start.y - 1; i <= end.y + 1; ++i)
	{
		t_pos tile;
		tile.x = x;
		tile.y = i;
		if (tile.x == x && tile.y == y)
			tile.type = currentMove->currentTurn;
		else
			tile.type = (eType)(*currentMove->gameField[tile.y])[tile.x];
		freeTreeSet.push_back(tile);
	}
	return fillThreeSet(*currentMove, freeTreeSet);
}

int				Gomoku::fillDiagonalLeftFreeTreeSet(t_move* currentMove, int x, int y, t_pos const & start, t_pos const & end)
{
	posSet freeTreeSet;
	int size = end.y - start.y + 2;
	for (int i = 0; i < size; ++i)
	{
		t_pos tile;
		tile.x = start.x + i - 1;
		tile.y = start.y + i - 1;
		if (tile.x == x && tile.y == y)
			tile.type = currentMove->currentTurn;
		else
			tile.type = (eType)(*currentMove->gameField[tile.y])[tile.x];
		freeTreeSet.push_back(tile);
	}
	return fillThreeSet(*currentMove, freeTreeSet);
}

int 			Gomoku::fillDiagonalRightFreeTreeSet(t_move* currentMove, int x, int y, t_pos const & start, t_pos const & end)
{
	posSet freeTreeSet;
	int size = end.y - start.y + 2;
	for (int i = 0; i < size; ++i)
	{
		t_pos tile;
		tile.x = start.x - i + 1;
		tile.y = start.y + i - 1;
		if (tile.x == x && tile.y == y)
			tile.type = currentMove->currentTurn;
		else
			tile.type = (eType)(*currentMove->gameField[tile.y])[tile.x];
		freeTreeSet.push_back(tile);
	}
	return fillThreeSet(*currentMove, freeTreeSet);
}

int 			Gomoku::checkFreeTreeHorizontal(t_move* currentMove, int x, int y)
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
		if (posX > 17 || (*currentMove->gameField[y])[posX] == findOppositeType(currentMove->currentTurn))
			break;
		if ((*currentMove->gameField[y])[posX] == currentMove->currentTurn) {
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
		if (posX < 0 ||  (*currentMove->gameField[y])[posX] == findOppositeType(currentMove->currentTurn))
			break;

			if ((*currentMove->gameField[y])[posX] == currentMove->currentTurn) {
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

int 			Gomoku::checkFreeTreeVertical(t_move* currentMove, int x, int y)
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
		if (posY > 17 || (*currentMove->gameField[posY])[x] == findOppositeType(currentMove->currentTurn))
			break;
		if ((*currentMove->gameField[posY])[x] == currentMove->currentTurn) {
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
		if (posY < 0 ||  (*currentMove->gameField[posY])[x] == findOppositeType(currentMove->currentTurn))
			break;

			if ((*currentMove->gameField[posY])[x] == currentMove->currentTurn) {
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

int 			Gomoku::checkFreeTreeDiagonalLeft(t_move* currentMove, int x, int y)
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
		if (posY > 17 || posX > 17 || (*currentMove->gameField[posY])[posX] == findOppositeType(currentMove->currentTurn))
			break;
		if ((*currentMove->gameField[posY])[posX] == currentMove->currentTurn) {
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
		if (posY < 0 ||  (*currentMove->gameField[posY])[posX] == findOppositeType(currentMove->currentTurn))
			break;
			if ((*currentMove->gameField[posY])[posX] == currentMove->currentTurn) {
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

int 			Gomoku::checkFreeTreeDiagonalRight(t_move* currentMove, int x, int y)
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
		if (posY > 17 || posX < 0 || (*currentMove->gameField[posY])[posX] == findOppositeType(currentMove->currentTurn))
			break;
		if ((*currentMove->gameField[posY])[posX] == currentMove->currentTurn) {
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
		if (posY < 0 ||  (*currentMove->gameField[posY])[posX] == findOppositeType(currentMove->currentTurn))
			break;
			if ((*currentMove->gameField[posY])[posX] == currentMove->currentTurn) {
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
