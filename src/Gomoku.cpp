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

static void printMove(MovePtr currentMove) {
	std::array<typeArr, N>const & gamefield = currentMove->getGameField();
	for (auto const & row : gamefield) {
		for (auto const &cell : row) {
			std::cout << cell << " ";
		}
		std::cout << "\n";
	}
	std::cout << "Move heuristic = " << currentMove->getHeuristic() << std::endl;
}

Gomoku::Gomoku(std::string input):m_N(18), m_exit(0)
{
	m_render = &Render::Instance();
	m_render->attachSharedLibrary("lib1_sdl.so", m_N + 1, m_N + 1);
	m_event.event = DEFAULT;
	m_event.x = -100;
	m_event.y = -100;
	m_render->init();
	//emptyGameField(m_heuristicField);
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



MovePtr 			Gomoku::algorithmMiniMax(MovePtr currentMove, int depth, int maxDepth)
{
	movePriorityQueue movingOptions;

	MovePtr findedMove;
	generateMoveOptions(currentMove,movingOptions);
	int counter = 0;
	if (depth == maxDepth - 1) {
		++depth;
		printf("Leaf node finded; depth = %d; maxDepth = %d; current turn = %d\n", depth, maxDepth, movingOptions.top()->getCurrentType());

		moveProcessing(movingOptions.top());
		findedMove = new Move(*movingOptions.top());

		printMove(findedMove);

		counter++;
	} else {
		int moveCounter = 0;
		//int currentDepth = ++depth;
		++depth;
		MovePtr checkingMove;
		MovePtr furtherMove;
		MovePtr returnedMove;
		movePriorityQueue checkMovingOptions;
		//printf("depth = %d; maxDepth = %d\n", depth, maxDepth);


		printf("Layer with depth = %d; maxDepth = %d; current turn = %d processed\n", depth, maxDepth, movingOptions.top()->getCurrentType());


		//depth++;
		while (moveCounter < 5 && !movingOptions.empty()) {

			checkingMove = movingOptions.top();
			counter++;
			if (checkingMove->getResult() == WIN) {
				//if (checkingMove->getCurrentType() == WHITE) {
					checkMovingOptions.push(checkingMove);
					movingOptions.pop();
					printf("FIND WIN\n");
					break;
				// } else {
				// 	checkMovingOptions.push(checkingMove);
				// 	movingOptions.pop();
				// 	continue;
				// }
			}
			furtherMove = new Move(*checkingMove);
			moveProcessing(furtherMove);
			//
			printf("moveCounter = %d; print processed furtherMove \n", moveCounter);
			printMove(furtherMove);


			//printf("print returnedMove on Layer with depth = %d; maxDepth = %d; current turn = %d processed\n", depth, maxDepth, movingOptions.top()->getCurrentType());
			returnedMove = algorithmMiniMax(furtherMove, depth, maxDepth);
			//printMove(returnedMove);
			//furtherMove->setHeuristic(returnedMove->getHeuristic());
			checkingMove->setHeuristic(returnedMove->getHeuristic());
			//checkMovingOptions.push(furtherMove);
			checkMovingOptions.push(checkingMove);
			movingOptions.pop();
		//	printf("delete returnedMove");
			delete returnedMove;
			delete furtherMove;
			//printf("movingOptions.top()");
			//delete movingOptions.top();
			//movingOptions.pop();
			moveCounter++;
		}
		findedMove = new Move(*checkMovingOptions.top());
		moveProcessing(findedMove);
		 printf("print findedMove in depth =  %d counter = %d\n", depth - 1, counter);
		 printMove(findedMove);
		while (!checkMovingOptions.empty()) {
			//printf("delete checkMovingOptions.top()");
			delete checkMovingOptions.top();
			checkMovingOptions.pop();
		}
		// for (auto & move : movingOptions) {
		// 	if (move->getResult() == WIN) {
		// 		findedMove
		// 	}
		// }

	}
	//printf("counter = %d\n\n\n", counter);

	// } else {
	//
	// }

	while (!movingOptions.empty()) {
		//	printf("delete movingOptions\n");
		//printf(" heuristic of deletenig move is = %d\n\n\n\n", movingOptions.top()->getHeuristic());
		//printMove(movingOptions.top());
		delete movingOptions.top();
		movingOptions.pop();
	}
	return findedMove;
}


void 	Gomoku::emptyGameField(std::array<typeArr, N> & gamefield)
{
	for (int i = 0; i < N; i++){
		gamefield[i].fill(EMPTY);
	}
}
//void 		Gomoku::moving(Move* currentMove)
void 		Gomoku::moving(Move *currentMove)
{
	moveChecking(currentMove, m_event.x, m_event.y);
	moveProcessing(currentMove);
	if (currentMove->getResult() == WIN) {
		std::string result_str =  currentMove->getCurrentType() == BLACK ? "BLACK WIN!!!" : "WHITE WIN!!!";
		currentMove->emptyGameField();
		std::cout << result_str << std::endl;
		//printf()
		//emptyGameField(m_currentMove.gameField);
		currentMove->setCurrentType(BLACK);
		t_pos & leftTop = const_cast<t_pos &>(currentMove->getLeftTop());
		t_pos & rightBottom = const_cast<t_pos &>(currentMove->getRightBottom());
		leftTop.x = -1;
		leftTop.y = -1;
		rightBottom.x = -1;
		rightBottom.y = -1;
		currentMove->setWhiteCapture(0);
		currentMove->setBlackCapture(0);
		m_render->GameOver(result_str);
	}
}

void 		Gomoku::AI_Move(Move* currentMove)
{

	m_turnTime =  0.0;//timeFromLastTurn;
	m_start = clock();
	int depth = 0;
	MovePtr bestMove = algorithmMiniMax(currentMove, depth, 3);
	if (bestMove->getResult() == WIN) {
		std::string result_str =  bestMove->getCurrentType() == BLACK ? "BLACK WIN!!!" : "WHITE WIN!!!";
		bestMove->emptyGameField();
		std::cout << result_str << std::endl;
		//printf()
		//emptyGameField(m_currentMove.gameField);
		bestMove->setCurrentType(BLACK);
		t_pos & leftTop = const_cast<t_pos &>(bestMove->getLeftTop());
		t_pos & rightBottom = const_cast<t_pos &>(bestMove->getRightBottom());
		leftTop.x = -1;
		leftTop.y = -1;
		rightBottom.x = -1;
		rightBottom.y = -1;
		bestMove->setWhiteCapture(0);
		bestMove->setBlackCapture(0);
		sleep(2);
		m_render->GameOver(result_str);
	}
	m_currentMove = *bestMove;
	delete bestMove;
	m_turnTime = static_cast<double>((clock() - m_start ))/ CLOCKS_PER_SEC;

}

void 		Gomoku::generateMoveOptions(MovePtr currentMove, movePriorityQueue & movingOptions)
{
	int lextX = currentMove->getLeftTop().x;
	int topY = currentMove->getLeftTop().y;
	int rightX = currentMove->getRightBottom().x;
	int bottomY = currentMove->getRightBottom().y;
	//movePriorityQueue movingOptions;
	std::array<typeArr, N> & gamefield = const_cast<std::array<typeArr, N> &>(currentMove->getGameField());
	m_virtualGameField = gamefield;
	//emptyGameField(virtualGameField);
	for (int y =  topY; y <= bottomY; ++y) {
		for (int x = lextX; x <= rightX; ++x){
			//printf("Checked x = %d; y = %d; virtualGameField[y][x] = %d\n\n", x, y, virtualGameField[y][x]);
	 	if (gamefield[y][x] != EMPTY ){
			//if (virtualGameField[y][x] != EMPTY)
			//&& virtualGameField[y - 1][x -1] != EMPTY;
				fillMoveOptions(currentMove, x, y, movingOptions, m_virtualGameField);

			}
		}
	}
}

void 		Gomoku::fillMoveOptions(MovePtr currentMove, int x_center, int y_center, movePriorityQueue & movingOptions, std::array<typeArr, N> &virtualGameField)
{
	for (int y = y_center - 1; y <= y_center + 1; ++y) {
		if (y < 0 || y > 17)
			continue;
		for (int x = x_center - 1; x <= x_center + 1; ++x) {
			if (x < 0 || x > 17 || (y == y_center && x == x_center))
				continue;
			if (virtualGameField[y][x] != EMPTY){
				//printf("x = %d; y = %d is not EMPTY\n", x, y);
				continue;
			}
			MovePtr newMove = new Move(*currentMove);
			moveChecking(newMove, x, y);
			virtualGameField[y][x] = currentMove->getCurrentType();
			//printf("Checked after fill x = %d; y = %d; virtualGameField[y][x] = %d\n\n", x, y, virtualGameField[y][x]);
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
		//printf("Push point with x =%d and y =%d denied - qquare is not empty\n", x, y);
	} else if (checkWin(currentMove, x, y)){
		//printf("Push point with x =%d and y =%d and win!\n", x, y);
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
	//printf("Left top x = %d; y = %d; Right bottom x = %d; y = %d\n\n\n", currentMove->getLeftTop().x, currentMove->getLeftTop().y, currentMove->getRightBottom().x, currentMove->getRightBottom().y);
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
	// else if (result == WIN && !forAI) {
	// 	std::string result_str =  currentMove->getCurrentType() == BLACK ? "BLACK WIN!!!" : "WHITE WIN!!!";
	// 	currentMove->emptyGameField();
	// 	std::cout << result_str << std::endl;
	// 	//printf()
	// 	//emptyGameField(m_currentMove.gameField);
	// 	currentMove->setCurrentType(BLACK);
	// 	t_pos & leftTop = const_cast<t_pos &>(currentMove->getLeftTop());
	// 	t_pos & rightBottom = const_cast<t_pos &>(currentMove->getRightBottom());
	// 	leftTop.x = -1;
	// 	leftTop.y = -1;
	// 	rightBottom.x = -1;
	// 	rightBottom.y = -1;
	// 	currentMove->setWhiteCapture(0);
	// 	currentMove->setBlackCapture(0);
	// 	m_render->GameOver(result_str);
	// }
}

void 		Gomoku::moveAI_Processing(MovePtr optionMove, movePriorityQueue & movingOptions)
{
	eMoveResult	const & result = optionMove->getResult();
	if (result == NON_EMPTY || result == DOUBLE_FREE_TREE){
		delete optionMove;
		return ;
	}
	defineHeuristic(optionMove, result);
	// printf("push move option \n");
	// printMove(optionMove);
	movingOptions.push(optionMove);
}

int 		Gomoku::defineHorizontalHeuristic(std::array<typeArr, N> & gamefield, eType currentTurn,
													t_pos const & leftTop, t_pos const & rightBottom)
{
	int horizontalHeur = 0;
	//return resolveHeuristicFromField(gamefield, currentTurn);
	//FIND FOUR ALLIGNMENT
	if ((horizontalHeur = defineTwoOpenHorizontalHeuristic(blackFourTwoOpen, whiteFourTwoOpen, gamefield, fourTwoOpenHeuristic, currentTurn, leftTop, rightBottom))){
		//printf("FourTwoOpen heuristic = %d\n\n\n\n", horizontalHeur);
		return horizontalHeur;
	}
	if ((horizontalHeur = defineOneOpenHorizontalHeuristic(blackFourOneOpenLeft, whiteFourOneOpenLeft, gamefield, fourOneOpenHeuristic, currentTurn, leftTop, rightBottom))
		|| (horizontalHeur = defineOneOpenHorizontalHeuristic(blackFourOneOpenRight, whiteFourOneOpenRight, gamefield, fourOneOpenHeuristic, currentTurn, leftTop, rightBottom))
		|| (horizontalHeur = defineOneOpenHorizontalHeuristic(blackFourOneOpenCenter_1, whiteFourOneOpenCenter_1, gamefield, fourOneOpenHeuristic, currentTurn, leftTop, rightBottom))
		|| (horizontalHeur = defineOneOpenHorizontalHeuristic(blackFourOneOpenCenter_2, whiteFourOneOpenCenter_2, gamefield, fourOneOpenHeuristic, currentTurn, leftTop, rightBottom))
		|| (horizontalHeur = defineOneOpenHorizontalHeuristic(blackFourOneOpenCenter_3, whiteFourOneOpenCenter_3, gamefield, fourOneOpenHeuristic, currentTurn, leftTop, rightBottom))) {
		//printf("FourOneOpen heuristic = %d\n\n\n\n", horizontalHeur);
		return horizontalHeur;
	}
	//FIND THREE ALLIGNMENT
	if ((horizontalHeur = defineTwoOpenHorizontalHeuristic(blackThreeTwoOpen_1, whiteThreeTwoOpen_1, gamefield, threeTwoOpenHeuristic, currentTurn, leftTop, rightBottom))
		|| (horizontalHeur = defineTwoOpenHorizontalHeuristic(blackThreeTwoOpen_2, whiteThreeTwoOpen_2, gamefield, threeTwoOpenHeuristic, currentTurn, leftTop, rightBottom))){
		//printf("ThreeTwoOpen heuristic = %d\n\n\n\n", horizontalHeur);
		return horizontalHeur;
	}


	if ((horizontalHeur = defineOneOpenHorizontalHeuristic(blackThreeOneOpenRight, whiteThreeOneOpenRight, gamefield, threeOneOpenHeuristic, currentTurn, leftTop, rightBottom))
		|| (horizontalHeur = defineOneOpenHorizontalHeuristic(blackThreeOneOpenLeft, whiteThreeOneOpenLeft, gamefield, threeOneOpenHeuristic, currentTurn, leftTop, rightBottom))
		|| (horizontalHeur = defineOneOpenHorizontalHeuristic(blackThreeOneOpenCenter_1, whiteThreeOneOpenCenter_1, gamefield, threeOneOpenHeuristic, currentTurn, leftTop, rightBottom))
		|| (horizontalHeur = defineOneOpenHorizontalHeuristic(blackThreeOneOpenCenter_2, whiteThreeOneOpenCenter_2, gamefield, threeOneOpenHeuristic, currentTurn, leftTop, rightBottom))
		|| (horizontalHeur = defineOneOpenHorizontalHeuristic(blackThreeOneOpenCenter_3, whiteThreeOneOpenCenter_3, gamefield, threeOneOpenHeuristic, currentTurn, leftTop, rightBottom))
		|| (horizontalHeur = defineOneOpenHorizontalHeuristic(blackThreeOneOpenCenter_4, whiteThreeOneOpenCenter_4, gamefield, threeOneOpenHeuristic, currentTurn, leftTop, rightBottom))) {
		//printf("ThreeOneOpen heuristic = %d\n\n\n\n", horizontalHeur);
		return horizontalHeur;
	}

	//FIND TWO ALLIGNMENT
	if ((horizontalHeur = defineTwoOpenHorizontalHeuristic(blackTwoTwoOpen_1, whiteTwoTwoOpen_1, gamefield, twoTwoOpenHeuristic, currentTurn, leftTop, rightBottom))
		|| (horizontalHeur = defineTwoOpenHorizontalHeuristic(blackTwoTwoOpen_2, whiteTwoTwoOpen_2, gamefield, twoTwoOpenHeuristic, currentTurn, leftTop, rightBottom))
		|| (horizontalHeur = defineTwoOpenHorizontalHeuristic(blackTwoTwoOpen_3, whiteTwoTwoOpen_3, gamefield, twoTwoOpenHeuristic, currentTurn, leftTop, rightBottom))){
		//printf("TwoTwoOpen heuristic = %d\n\n\n\n", horizontalHeur);
		return horizontalHeur;
	}


	if ((horizontalHeur = defineOneOpenHorizontalHeuristic(blackTwoOneOpenRight, whiteTwoOneOpenRight, gamefield, oneTwoOpenHeuristic, currentTurn, leftTop, rightBottom))
		|| (horizontalHeur = defineOneOpenHorizontalHeuristic(blackTwoOneOpenLeft, whiteTwoOneOpenLeft, gamefield, oneTwoOpenHeuristic, currentTurn, leftTop, rightBottom))
	) {
	//	printf("TwoOneOpen heuristic = %d\n\n\n\n", horizontalHeur);
		return horizontalHeur;
	}

	return 0;
}


int 							Gomoku::defineVerticalHeuristic(std::array<typeArr, N> & gamefield, eType currentTurn,
																		t_pos const & leftTop, t_pos const & rightBottom)
{
	m_heuristicField.clear();
	int start = leftTop.x > 0 ? leftTop.x - 1 : 0;
	int end = rightBottom.x < 17 ? rightBottom.x + 1 : 17;
	for (int x = start; x < end; ++x) {
		std::vector<eType> column;
		for (int y = 0; y < 18; ++y) {
			column.push_back(gamefield[y][x]);
		}
		m_heuristicField.push_back(column);
	}
	return resolveHeuristicFromField(m_heuristicField, currentTurn);
}

int 							Gomoku::defineDiagonalLeftHeuristic(std::array<typeArr, N> & gamefield, eType currentTurn,
																		t_pos const & leftTop, t_pos const & rightBottom)
{
	m_heuristicField.clear();
	int diagonalX;
	int diagonalY;
	//printf("defineDiagonalLeftHeuristic left top x = %d y = %d rightBottom x = %d y = %d:\n", leftTop.x, leftTop.y, rightBottom.x, rightBottom.y);
	for (int i = leftTop.x; i <= rightBottom.x; ++i) {
		if (i < leftTop.y) {
			diagonalX = 0;
			diagonalY = leftTop.y - i;
		} else {
			diagonalX = i - leftTop.y;
			diagonalY = 0;
		}
		//printf("i = %d top diagonalX = %d diagonalY = %d :\n", i,diagonalX, diagonalY);
		std::vector<eType> diagonal;
		while (diagonalX < 18 && diagonalY < 18) {
		//	printf("top diagonalX = %d diagonalY = %d elem = %d:\n", diagonalX, diagonalY, gamefield[diagonalY][diagonalX]);
			diagonal.push_back(gamefield[diagonalY][diagonalX]);
			diagonalX++;
			diagonalY++;
		}
		m_heuristicField.push_back(diagonal);
	}

	for (int i = leftTop.y + 1; i <= rightBottom.y; ++i) {
		if (i < leftTop.x) {
			diagonalX = leftTop.x - i;
			diagonalY = 0;
		} else {
			diagonalX = 0;
			diagonalY = i - leftTop.x;
		}
		std::vector<eType> diagonal;
		while (diagonalX < 18 && diagonalY < 18) {
			diagonal.push_back(gamefield[diagonalY][diagonalX]);
			diagonalX++;
			diagonalY++;
		}
		m_heuristicField.push_back(diagonal);
	}
	//printf("m_heuristicField:\n");
	// for (auto && row : m_heuristicField) {
	// 	for (auto && element : row) {
	// 		printf("%d ",  element);
	// 	}
	// 	printf(";\n ");
	// }
	return resolveHeuristicFromField(m_heuristicField, currentTurn);
}

int 							Gomoku::defineDiagonalRightHeuristic(std::array<typeArr, N> & gamefield, eType currentTurn,
																		t_pos const & leftTop, t_pos const & rightBottom)
{
	m_heuristicField.clear();
	int diagonalX;
	int diagonalY;
	int rightX = 17 - leftTop.y;
	//printf("definediagonalRightHeuristic left top x = %d y = %d rightBottom x = %d y = %d:\n", leftTop.x, leftTop.y, rightBottom.x, rightBottom.y);
	for (int i = leftTop.x; i <= rightBottom.x; ++i) {
		if (i < rightX) {
			diagonalX = leftTop.y + i < 18 ? leftTop.y + i : 17;
			diagonalY = 0; //leftTop.y - i;
		} else {
			diagonalX = 17;
			diagonalY = leftTop.y - (17 - i);//leftTop.y - i > 0 ? leftTop.y - i : 0; //leftTop.y - i;
		}
	//	printf("i = %d top diagonalX = %d diagonalY = %d :\n", i,diagonalX, diagonalY);
		std::vector<eType> diagonal;
		while (diagonalX >= 0 && diagonalY < 18) {
		//	printf("top diagonalX = %d diagonalY = %d elem = %d:\n", diagonalX, diagonalY, gamefield[diagonalY][diagonalX]);
			diagonal.push_back(gamefield[diagonalY][diagonalX]);
			diagonalX--;
			diagonalY++;
		}
		m_heuristicField.push_back(diagonal);
	}

	for (int i = leftTop.y + 1; i <= rightBottom.y; ++i) {
		rightX = 17 - i;
		if (rightBottom.x < rightX) {
			diagonalX = rightBottom.x + i;
			diagonalY = 0;
		} else {
			diagonalX = 17;
			diagonalY = i - (17 - rightBottom.x);//i - leftTop.x;
		}
		//printf("i = %d top diagonalX = %d diagonalY = %d :\n", i,diagonalX, diagonalY);
		std::vector<eType> diagonal;
		while (diagonalX >= 0 && diagonalY < 18) {
			//printf("top diagonalX = %d diagonalY = %d elem = %d:\n", diagonalX, diagonalY, gamefield[diagonalY][diagonalX]);
			diagonal.push_back(gamefield[diagonalY][diagonalX]);
			diagonalX--;
			diagonalY++;
		}
		m_heuristicField.push_back(diagonal);
	}
	//printf("m_heuristicField:\n");
	// for (auto && row : m_heuristicField) {
	// 	for (auto && element : row) {
	// 		printf("%d ",  element);
	// 	}
	// 	printf(";\n ");
	// }
	return resolveHeuristicFromField(m_heuristicField, currentTurn);
}

int						Gomoku::resolveHeuristicFromField(std::vector<std::vector<eType>> const & heuristicField, eType currentTurn)
{
	int heuristic = 0;

	if ((heuristic = defineTwoOpenHeuristic(blackFourTwoOpen, whiteFourTwoOpen, heuristicField, fourTwoOpenHeuristic, currentTurn))){
		//printf("FourTwoOpen heuristic = %d\n\n\n\n", heuristic);
		return heuristic;
	}

	if ((heuristic = defineOneOpenHeuristic(blackFourOneOpenLeft, whiteFourOneOpenLeft, heuristicField, fourOneOpenHeuristic, currentTurn))
		|| (heuristic = defineOneOpenHeuristic(blackFourOneOpenRight, whiteFourOneOpenRight, heuristicField, fourOneOpenHeuristic, currentTurn))
		|| (heuristic = defineOneOpenHeuristic(blackFourOneOpenCenter_1, whiteFourOneOpenCenter_1, heuristicField, fourOneOpenHeuristic, currentTurn))
		|| (heuristic = defineOneOpenHeuristic(blackFourOneOpenCenter_2, whiteFourOneOpenCenter_2, heuristicField, fourOneOpenHeuristic, currentTurn))
		|| (heuristic = defineOneOpenHeuristic(blackFourOneOpenCenter_3, whiteFourOneOpenCenter_3, heuristicField, fourOneOpenHeuristic, currentTurn))) {
		//printf("FourOneOpen heuristic = %d\n\n\n\n", heuristic);
		return heuristic;
	}
	//FIND THREE ALLIGNMENT
	if ((heuristic = defineTwoOpenHeuristic(blackThreeTwoOpen_1, whiteThreeTwoOpen_1, heuristicField, threeTwoOpenHeuristic, currentTurn))
		|| (heuristic = defineTwoOpenHeuristic(blackThreeTwoOpen_2, whiteThreeTwoOpen_2, heuristicField, threeTwoOpenHeuristic, currentTurn))){
		//printf("ThreeTwoOpen heuristic = %d\n\n\n\n", heuristic);
		return heuristic;
	}

	if ((heuristic = defineOneOpenHeuristic(blackThreeOneOpenRight, whiteThreeOneOpenRight, heuristicField, threeOneOpenHeuristic, currentTurn))
		|| (heuristic = defineOneOpenHeuristic(blackThreeOneOpenLeft, whiteThreeOneOpenLeft, heuristicField, threeOneOpenHeuristic, currentTurn))
		|| (heuristic = defineOneOpenHeuristic(blackThreeOneOpenCenter_1, whiteThreeOneOpenCenter_1, heuristicField, threeOneOpenHeuristic, currentTurn))
		|| (heuristic = defineOneOpenHeuristic(blackThreeOneOpenCenter_2, whiteThreeOneOpenCenter_2, heuristicField, threeOneOpenHeuristic, currentTurn))
		|| (heuristic = defineOneOpenHeuristic(blackThreeOneOpenCenter_3, whiteThreeOneOpenCenter_3, heuristicField, threeOneOpenHeuristic, currentTurn))
		|| (heuristic = defineOneOpenHeuristic(blackThreeOneOpenCenter_4, whiteThreeOneOpenCenter_4, heuristicField, threeOneOpenHeuristic, currentTurn))) {
		//printf("ThreeOneOpen heuristic = %d\n\n\n\n", heuristic);
		return heuristic;
	}

	//FIND TWO ALLIGNMENT
	if ((heuristic = defineTwoOpenHeuristic(blackTwoTwoOpen_1, whiteTwoTwoOpen_1, heuristicField, twoTwoOpenHeuristic, currentTurn))
		|| (heuristic = defineTwoOpenHeuristic(blackTwoTwoOpen_2, whiteTwoTwoOpen_2, heuristicField, twoTwoOpenHeuristic, currentTurn))
		|| (heuristic = defineTwoOpenHeuristic(blackTwoTwoOpen_3, whiteTwoTwoOpen_3, heuristicField, twoTwoOpenHeuristic, currentTurn))){
		//printf("TwoTwoOpen heuristic = %d\n\n\n\n", heuristic);
		return heuristic;
	}

	if ((heuristic = defineOneOpenHeuristic(blackTwoOneOpenRight, whiteTwoOneOpenRight, heuristicField, oneTwoOpenHeuristic, currentTurn))
		|| (heuristic = defineOneOpenHeuristic(blackTwoOneOpenLeft, whiteTwoOneOpenLeft, heuristicField, oneTwoOpenHeuristic, currentTurn))
	) {
		//printf("TwoOneOpen heuristic = %d\n\n\n\n", heuristic);
		return heuristic;
	}

	return 0;
}

int 							Gomoku::defineTwoOpenHeuristic(twoOpenSample const & blackSample, twoOpenSample const & whiteSample, std::vector<std::vector<eType>> const & gamefield,
																		const int heuristic, eType currentTurn)
{
	int returnHeur = 0;
	if (findTwoOpen(blackSample, gamefield))
		returnHeur += currentTurn == BLACK ? heuristic / 2 : heuristic;
	if (findTwoOpen(whiteSample, gamefield))
		returnHeur -= currentTurn == BLACK ? heuristic  : heuristic / 2;
	return returnHeur;
}

int 							Gomoku::defineOneOpenHeuristic( oneOpenSample const & blackSample, oneOpenSample const & whiteSample, std::vector<std::vector<eType>> const & gamefield,
														  const int heuristic, eType currentTurn)
{
	int returnHeur = 0;
	if (findOneOpen(blackSample, gamefield))
		returnHeur += currentTurn == BLACK ? heuristic / 2 : heuristic;
	if (findOneOpen(whiteSample, gamefield))
		returnHeur -= currentTurn == BLACK ? heuristic  : heuristic / 2;
	return returnHeur;
}



int 		Gomoku::defineTwoOpenHorizontalHeuristic(std::array<eType, 6> const & blackSample, std::array<eType, 6> const & whiteSample,
														std::array<typeArr, N> & gamefield, const int heuristic, eType currentTurn, t_pos const & leftTop, t_pos const & rightBottom)
{
	int horizontalHeur = 0;
	if (findTwoOpenHorizontal(blackSample, gamefield, leftTop, rightBottom))
		horizontalHeur += currentTurn == BLACK ? heuristic / 2 : heuristic;
	if (findTwoOpenHorizontal(whiteSample, gamefield, leftTop, rightBottom))
		horizontalHeur -= currentTurn == BLACK ? heuristic  : heuristic / 2;
	return horizontalHeur;
}


int 		Gomoku::defineOneOpenHorizontalHeuristic(std::array<eType, 5> const & blackSample, std::array<eType, 5> const & whiteSample, std::array<typeArr, N> & gamefield,
															const int heuristic, eType currentTurn, t_pos const & leftTop, t_pos const & rightBottom)
{
	int horizontalHeur = 0;
	if (findOneOpenHorizontal(blackSample, gamefield, leftTop, rightBottom))
		horizontalHeur += currentTurn == BLACK ? heuristic / 2 : heuristic;
	if (findOneOpenHorizontal(whiteSample, gamefield, leftTop, rightBottom))
		horizontalHeur -= currentTurn == BLACK ? heuristic  : heuristic / 2;
	return horizontalHeur;
}

int 		Gomoku::findTwoOpen(twoOpenSample const & sampleTwoOpen, std::vector<std::vector<eType>> const & gamefield)
{
	//std::vector<eType>::iterator it;
	for (auto && column : gamefield) {
		auto it = std::search(column.begin(), column.end(), sampleTwoOpen.begin(), sampleTwoOpen.end());
		if (it != column.end())
		{
			//printf("TwoOpen find in  and y = %ld\n\n\n\n", it - column.begin());
			return 1;
		}
	}
	return 0;
}

int 		Gomoku::findOneOpen(oneOpenSample const & sampleOneOpen, std::vector<std::vector<eType>> const & gamefield)
{
	//std::vector<eType>::iterator it;
	for (auto && column : gamefield) {
		auto it = std::search(column.begin(), column.end(), sampleOneOpen.begin(), sampleOneOpen.end());
		if (it != column.end())
		{
			//printf("OneOpen find in  and y = %ld\n\n\n\n", it - column.begin());
			return 1;
		}
	}
	return 0;
}

int 		Gomoku::findTwoOpenHorizontal(std::array<eType, 6> const & sampleFourTwoOpen, std::array<typeArr, N> & gamefield, t_pos const & leftTop, t_pos const & rightBottom)
{
	std::array<eType,18>::iterator it;
	int start = leftTop.y > 0 ? leftTop.y - 1 : 0;
	int end = rightBottom.y < 17 ? rightBottom.y + 1 : 17;
	for (int i = start; i < end; ++i) {
		it = std::search(gamefield[i].begin(),gamefield[i].end(), sampleFourTwoOpen.begin(), sampleFourTwoOpen.end());
		if (it != gamefield[i].end()){
			//printf("blackTwoOpen find in x = %ld and y = %d\n\n\n\n", it - gamefield[i].begin(), i);
			return 1;
		}
	}
	return 0;
}

int 		Gomoku::findOneOpenHorizontal(std::array<eType, 5> const & sampleFourTwoOpen, std::array<typeArr, N> & gamefield, t_pos const & leftTop, t_pos const & rightBottom)
{
	std::array<eType,18>::iterator it;
	int start = leftTop.y > 0 ? leftTop.y - 1 : 0;
	int end = rightBottom.y < 17 ? rightBottom.y + 1 : 17;
	for (int i = start; i < end; ++i) {
		it = std::search(gamefield[i].begin(),gamefield[i].end(), sampleFourTwoOpen.begin(), sampleFourTwoOpen.end());
		if (it != gamefield[i].end()){
			//printf("blackOneOpen find in x = %ld and y = %d\n\n\n\n", it - gamefield[i].begin(), i);
			return 1;
		}
	}
	return 0;
}

void 		Gomoku::defineHeuristic(MovePtr optionMove, eMoveResult result)
{
	if (result == WIN) {
		optionMove->setHeuristic(optionMove->getCurrentType() == BLACK ? winHeuristic : -winHeuristic);
		return;
	}
	std::array<typeArr, N> & gamefield = const_cast<std::array<typeArr, N> &>(optionMove->getGameField());
	int heur = 0;
	heur +=	defineHorizontalHeuristic(gamefield, optionMove->getCurrentType(), optionMove->getLeftTop(), optionMove->getRightBottom());
	heur +=	defineVerticalHeuristic(gamefield, optionMove->getCurrentType(), optionMove->getLeftTop(), optionMove->getRightBottom());
	heur +=	defineDiagonalLeftHeuristic(gamefield, optionMove->getCurrentType(), optionMove->getLeftTop(), optionMove->getRightBottom());
	heur +=	defineDiagonalRightHeuristic(gamefield, optionMove->getCurrentType(), optionMove->getLeftTop(), optionMove->getRightBottom());
	// if (heur)
	// 	printf("defineHorizontalHeuristic = %d\n\n\n\n", heur);
	// int heur +=	defineVerticalHeuristic(gamefield, optionMove->getCurrentType());

	//int heur +=	defineDigonalRightHeuristic(gamefield, optionMove->getCurrentType());
	// if (result == CAPTURE) {
	// 	heur += BLACK ? -10000 : 10000;
	// }
	//printf("heur = %d\n", heur);
	heur += optionMove->getWhiteCapture() * (-15000) + optionMove->getBlackCapture() * 15000;
	//printf("heur1 = %d\n", heur);
	optionMove->setHeuristic(heur);
}

































int 			Gomoku::checkWin(Move* currentMove, int x, int y)
{
	//printf("Chek x =%d and y =%d to win\n", m_event.x, m_event.y );

	if (checkWinHorizontal(currentMove, x, y))
		return 1;
	else if (checkWinVertical(currentMove, x, y))
		return 1;
	else if (checkWinDiagonalLeft(currentMove, x, y))
		return 1;
	else if (checkWinDiagonalRight(currentMove, x, y))
		return 1;
	//printf("Chek x =%d and y =%d to win return 0\n", m_event.x, m_event.y );
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
			//printf("checkPossibleCaptureHorizontalWin for x =%d and y =%d \n", i, y);
			return 0;
		}
		if (y - 2 >= 0
			&& (currentMove->getGameField()[y + 1])[i] == findOppositeType(currType)
			&& (currentMove->getGameField()[y - 1])[i] == currType
			&& (currentMove->getGameField()[y - 2])[i] == EMPTY){
				//printf("checkPossibleCaptureHorizontalWin for x =%d and y =%d \n", i, y);
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
			//printf("checkPossibleCaptureVerticalWin for x =%d and x =%d \n", i, x);
			return 0;
		}
		if (x - 2 >= 0
			&& (currentMove->getGameField()[i])[x + 1] == findOppositeType(currentMove->getCurrentType())
			&& (currentMove->getGameField()[i])[x - 1] == currentMove->getCurrentType()
			&& (currentMove->getGameField()[i])[x - 2] == EMPTY){
				//printf("checkPossibleCaptureVerticalWin for x =%d and x =%d \n", i, x);
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
			//printf("checkPossibleCaptureDiagonalLeftWin for x =%d and x =%d \n", posX, posY);
			return 0;
		}

		if (posX + 1 <= 17 && posY - 1 >= 0 && posX - 2 >=0  && posY + 2 <= 17
			&& (currentMove->getGameField()[posY - 1])[posX + 1] == findOppositeType(currentMove->getCurrentType())
			&& (currentMove->getGameField()[posY + 1])[posX - 1] == currentMove->getCurrentType()
			&& (currentMove->getGameField()[posY + 2])[posX - 2] == EMPTY) {
			//printf("checkPossibleCaptureDiagonalLeftWin for x =%d and x =%d \n", posX, posY);
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
			//printf("checkPossibleCaptureDiagonalLeftWin for x =%d and x =%d \n", posX, posY);
			return 0;
		}
		if (posX + 1 <= 17 && posY + 1 <= 17 && posX - 2 >=0 && posY - 2 >= 0
			&& (currentMove->getGameField()[posY + 1])[posX + 1] == findOppositeType(currentMove->getCurrentType())
			&& (currentMove->getGameField()[posY - 1])[posX - 1] == currentMove->getCurrentType()
			&& (currentMove->getGameField()[posY - 2])[posX - 2] == EMPTY) {
			//printf("checkPossibleCaptureDiagonalLeftWin for x =%d and x =%d \n", posX, posY);
			return 0;
		}
		posX--;
		posY++;
	}
 	return 1;
}


int 			Gomoku::checkWinHorizontal(Move* currentMove, int x, int y)
{
		//printf("checkWinHorizontalx =%d and y =%d to win\n", x, y );
	int match = 1;
	int startX = x;
	int endX = x;
	for (int i = 1; i < 5; ++i) {
		int posX = x + i;
		if (posX > 17 || (currentMove->getGameField()[y])[posX] != currentMove->getCurrentType())
			break;
		else if(++match == 5){
			endX = posX;
			//printf("match for x =%d and y =%d is %d\n", x, y, match );
			return checkPossibleCaptureHorizontalWin(currentMove, startX, endX, y);
		}
		//printf("match for x =%d and y =%d is %d\n", x, y, match );
	}
	for (int i = 1; i < 5 ; ++i) {
		int posX = x - i;
		if (posX < 0 || (currentMove->getGameField()[y])[posX] != currentMove->getCurrentType())
			break;
		else if(++match == 5){
			startX = posX;
			//printf("match for x =%d and y =%d is %d\n", x, y, match );
			return checkPossibleCaptureHorizontalWin(currentMove, startX, endX, y);
		}
		//printf("match for x =%d and y =%d is %d\n", x, y, match );
	}
	return 0;
}



int 			Gomoku::checkWinVertical(Move* currentMove, int x, int y)
{
		//printf("checkWinVertical x =%d and y =%d to win\n", x, y );
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
	//printf("checkWinDiagonalLeft x =%d and y =%d to win\n", x, y );
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
	//printf("checkWinDiagonalRight x =%d and y =%d to win\n", x, y );
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
	//printf("ERASE TILE x =%d and y =%d x2 =%d and y2 =%d\n", startX, startY,  endX, endY);
	if (startX < 0 || startX > 17 || endX > 17
		|| startY < 0 || startY > 17 || endY > 17 )
		return;
	if (startX == endX){
		for (int i = startY; i <= endY; ++i) {
			//printf("ERASE TILE x =%d and y =%d\n", startX, i );
			(*gameField)[i][startX] = EMPTY;
		}
		return;
	}

	if (startY == endY){
		for (int i = startX; i <= endX; ++i) {
			//printf("ERASE TILE x =%d and y =%d\n", i, startY );
			(*gameField)[startY][i] = EMPTY;
		}
		return;
	}
	int j = startX;
	for (int i = startY; i <= endY; ++i) {
			//printf("ERASE TILE x =%d and y =%d\n", j, i );
			(*gameField)[i][j] = EMPTY;
			if (startX < endX)
				j++;
			else
				j--;
	}
}

int 			Gomoku::checkCapture(Move* currentMove, int x, int y)
{
	//printf("checkCapture x =%d and y =%d to win\n", x, y );
	int captures = 0;
	//printf("checkCaptureHorizontal x =%d and y =%d captures = %d\n", x, y, captures );
	captures += checkCaptureHorizontal(currentMove, x, y);
	//printf("checkCaptureVertical x =%d and y =%d captures = %d\n", x, y, captures );
	captures += checkCaptureVertical(currentMove, x, y);
	//printf("checkCaptureDiagonalLeft x =%d and y =%d captures = %d\n", x, y, captures );
	captures += checkCaptureDiagonalLeft(currentMove, x, y);
	//printf("checkCaptureDiagonalRight x =%d and y =%d captures = %d\n", x, y, captures );
	captures += checkCaptureDiagonalRight(currentMove, x, y);
	//printf("checkCapture x =%d and y =%d captures = %d\n", x, y, captures );
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
		//printf("Chek capture x =%d and y =%d\n", x, y );
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
		//printf("Chek capture x =%d and y =%d\n", x, y );
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
		//printf("Chek capture x =%d and y =%d\n", x, y );
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
		//printf("Chek capture x =%d and y =%d\n", x, y );
		eraseTiles(&(const_cast<std::array<typeArr, N>  &>(currentMove->getGameField())), x + 2 , y - 2, x  + 1, y - 1);
		capture++;
	}
	return capture;
}

int 			Gomoku::checkFreeTree(Move* currentMove, int x, int y)
{
	int free_tree = 0;
//printf("Chek free_tree x =%d and y =%d free_tree = %d\n", x, y, free_tree);
	free_tree += checkFreeTreeHorizontal(currentMove, x, y);
	free_tree += checkFreeTreeVertical(currentMove, x, y);
	free_tree += checkFreeTreeDiagonalLeft(currentMove, x, y);
	free_tree +=checkFreeTreeDiagonalRight(currentMove, x, y);
	//printf("Chek free_tree x =%d and y =%d free_tree = %d\n", x, y, free_tree);
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
		//printf ("checkcheckDoubleFreeTree current turn is %d\n", currentMove->getCurrentType());
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
			//printf ("x = %d y = %d doubleFreeThree = %d\n", x, y, doubleFreeThree);
			if (doubleFreeThree){
				//printf ("Double FREE THREE!!!!!\n");
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
