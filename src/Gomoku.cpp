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
	m_checker = &Checker::Instance();
	m_heuristicSolver = &HeuristicSolver::Instance();
	m_event.event = DEFAULT;
	m_event.x = -100;
	m_event.y = -100;
	m_AI = input == "AI" ? 1 : 0;
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
	printf("%s\n","debug");
	m_render->attachSharedLibrary("lib1_sdl.so", N + 1, N + 1);
	m_render->init();
	printf("%s\n","debug2");
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

bool 				Gomoku::cutOff(MovePtr checkingMove, alfaBeta & ab, int & value)
{
	if (checkingMove->getCurrentType() == BLACK){
		value = std::max(value, checkingMove->getHeuristic());
		ab.alpha = std::max(ab.alpha, value);
		//printf ("BLACK: value = %d, alfaBeta.alpha = %d, alfaBeta.beta =  %d \n",value, ab.alpha,ab.beta);
	} else {
		value = std::min(value, checkingMove->getHeuristic());
		ab.beta = std::min(ab.beta, value);
		//printf ("WHITE: value = %d, alfaBeta.alpha = %d, alfaBeta.beta =  %d \n",value, ab.alpha,ab.beta);
	}
	return ab.alpha > ab.beta;
}

void 					Gomoku::clearQueue(movePriorityQueue & Queue)
{
	while (!Queue.empty()) {
		delete Queue.top();
		Queue.pop();
	}
}

MovePtr 					Gomoku::findMove(MovePtr move)
{
	MovePtr findedMove = new Move(*move);
	moveProcessing(findedMove);
	return findedMove;
}

MovePtr 			Gomoku::algorithmMiniMax(MovePtr currentMove, int depth, int maxDepth, alfaBeta ab)
{
	movePriorityQueue movingOptions;
	MovePtr findedMove = nullptr;

	generateMoveOptions(currentMove, movingOptions);
	if (depth == maxDepth - 1) {
		//printf("Leaf node finded; depth = %d; maxDepth = %d; current turn = %d\n", depth, maxDepth, movingOptions.top()->getCurrentType());
		findedMove = findMove(movingOptions.top());
		printMove(findedMove);
	} else {
		MovePtr checkingMove;
		MovePtr furtherMove; //= nullptr;
		MovePtr returnedMove;
		movePriorityQueue checkMovingOptions;
		int moveCounter = 0;
		int value = movingOptions.top()->getCurrentType() == BLACK ? alfaInf : betaInf;
		++depth;

		// printf("Layer with depth = %d; maxDepth = %d; current turn = %d processed\n", depth, maxDepth, movingOptions.top()->getCurrentType());
		// printf ("alfaBeta.alpha = %d alfaBeta.beta =  %d\n",ab.alpha,ab.beta);

		while (moveCounter < 3 && !movingOptions.empty()) {
			checkingMove = movingOptions.top();
			if (checkingMove->getResult() == WIN) {
				checkMovingOptions.push(checkingMove);
				movingOptions.pop();
				printf("FIND WIN\n");
				break;
			}
			furtherMove = findMove(checkingMove);

			// printf("moveCounter = %d; print processed furtherMove \n", moveCounter);
			// printMove(furtherMove);

			returnedMove = algorithmMiniMax(furtherMove, depth, maxDepth,ab);
			delete furtherMove;
			moveCounter++;
			if (returnedMove == nullptr) {
				delete checkingMove;
				movingOptions.pop();
				continue ;
			}
			checkingMove->setHeuristic(returnedMove->getHeuristic());
			delete returnedMove;
			if (cutOff(checkingMove, ab, value))
				break;
 			checkMovingOptions.push(checkingMove);
			movingOptions.pop();
		}
		if (!checkMovingOptions.empty()){
			findedMove = findMove(checkMovingOptions.top());
			//printf("print findedMove in depth =  %d \n", depth - 1);
			//printMove(findedMove);
	 	}
		clearQueue(checkMovingOptions);
	}
	clearQueue(movingOptions);
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
	//moveChecking(currentMove, m_event.x, m_event.y);
	m_checker->moveChecking(currentMove, m_event.x, m_event.y);
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
	alfaBeta ab;
	printf ("initial alfaBeta.alpha = %d alfaBeta.beta =  %d\n",ab.alpha,ab.beta);
	MovePtr bestMove = algorithmMiniMax(currentMove, depth, 6, ab);
	printf ("final alfaBeta.alpha = %d alfaBeta.beta =  %d\n",ab.alpha,ab.beta);
	if (bestMove->getResult() == WIN) {
		std::string result_str =  bestMove->getCurrentType() == BLACK ? "BLACK WIN!!!" : "WHITE WIN!!!";
		printMove(bestMove);
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
	std::array<typeArr, N> & gamefield = currentMove->getGameFieldMod();
	m_virtualGameField = gamefield;
	for (int y =  topY; y <= bottomY; ++y) {
		for (int x = lextX; x <= rightX; ++x){
		 	if (gamefield[y][x] != EMPTY ){
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
				continue;
			}
			MovePtr newMove = new Move(*currentMove);
			m_checker->moveChecking(newMove, x, y);
			virtualGameField[y][x] = currentMove->getCurrentType();
			//printf("Checked after fill x = %d; y = %d; virtualGameField[y][x] = %d\n\n", x, y, virtualGameField[y][x]);
			moveAI_Processing(newMove, movingOptions);
		}
	}
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
}

void 		Gomoku::moveAI_Processing(MovePtr optionMove, movePriorityQueue & movingOptions)
{
	eMoveResult	const & result = optionMove->getResult();
	if (result == NON_EMPTY || result == DOUBLE_FREE_TREE){
		delete optionMove;
		return ;
	}
	m_heuristicSolver->defineHeuristic(optionMove, result);
	movingOptions.push(optionMove);
}
