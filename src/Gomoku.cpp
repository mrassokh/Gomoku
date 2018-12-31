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
	auto const & gamefield = currentMove->getGameField();
	for (auto const & row : gamefield) {
		for (auto const &cell : row) {
			std::cout << cell << " ";
		}
		std::cout << "\n";
	}
	std::cout << "Move heuristic = " << currentMove->getHeuristic() << std::endl;
}

Gomoku::Gomoku(std::string input, eStartGame startCondition):m_N(18), m_exit(0)
{
	m_render = std::make_unique<Render>();
	m_checker = std::make_unique<Checker>();
	m_heuristicSolver = std::make_unique<HeuristicSolver>();
	m_event.event = DEFAULT;
	m_event.x = -100;
	m_event.y = -100;
	m_AI = input == "AI" ? 1 : 0;
	m_windowCondition.startCondition = 1;
	m_windowCondition.gameCondition = 0;
	m_windowCondition.gameOverCondition = 0;
	m_AIType = WHITE;
	m_step = 0;
	m_startGame = startCondition;//SWAP_FIELD;
	m_putTwoStone = 0; //PRO_FIELD;//EMPTY_FIELD;//PRO_FIELD
	srand(time(NULL));
}

Gomoku::~Gomoku()
{

}

void 		Gomoku::moveReset(MovePtr currentMove)
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
	m_currentMove = *currentMove.get();
}

void 		Gomoku::initPRO(Move & currentMove)
{
	pushToCentre(currentMove);
	pushNearestToCentre(currentMove);

	//pushFarThenThreeFromCentre(currentMove);

}

void 		Gomoku::initSWAP(Move & currentMove)
{
	auto pushMove = std::make_shared<Move>(currentMove);
	auto & gamefield = currentMove.getGameFieldMod();
	m_virtualGameField = gamefield;
	int x = 0;
	int y = 0;
	for (int i = 0; i < 3; i++){
		while (true) {
			x = (rand() % 18);
			y = (rand() % 18);
			if (m_virtualGameField[y][x] == EMPTY)
				break;
		}
		preventInequalPush(m_virtualGameField, x, y, m_currentMove.getCurrentType());
		moving(pushMove, x, y);
		m_step++;
		m_currentMove = *pushMove.get();
	}
}
void 			Gomoku::preventInequalPush(std::array<typeArr, N> & virtualGameField, int x, int y, eType type)
{
	t_pos start;
	t_pos end;
	start.x = x - 5 > 0 ?  x - 5 : 0;
	end.x = x + 5 <= 17 ?  x + 5 : 17;
	start.y = y - 5 > 0 ?  y - 5 : 0;
	end.y = y + 5 <= 17 ?  y + 5 : 17;
	for (int i = start.x; i < end.x; ++i) {
		for (int j = start.y; i < end.y; ++i) {
			virtualGameField[j][i] = type;
		}
	}
}

void 		Gomoku::pushToCentre(Move & currentMove)
{
	auto pushMove = std::make_shared<Move>(currentMove);
	moving(pushMove, 8, 8);
	m_currentMove = *pushMove.get();
	m_step++;
}

void 		Gomoku::pushNearestToCentre(Move & currentMove)
{
	auto pushMove = std::make_shared<Move>(currentMove);

	int x = 0;
	int y = 0;
	while (true) {
		x = (rand() % 10);
		if (x < 7 || x > 10)
			continue;
		y = (rand() % 10);
		if (y < 7 || y > 10)
			continue;
		if (y == 8 && x == 8)
			continue;
		break;
	}
	moving(pushMove, x, y);
	m_step++;
	m_currentMove = *pushMove.get();
}

// void 	emptyGameField(Move & currentMove)
// {
//
// }

void 		Gomoku::pushFarThenThreeFromCentre(Move & currentMove)
{
	auto pushMove = std::make_shared<Move>(currentMove);
	auto & gamefield = currentMove.getGameFieldMod();
	// for (int i = 0; i < N; i++){
	// 	m_virtualGameField[i].fill(EMPTY);
	// }
	m_virtualGameField = gamefield;
	for (int y =  6; y < 11; ++y) {
		for (int x = 6; x < 11; ++x){
			m_virtualGameField[y][x] = currentMove.getCurrentType();
		}
	}
	int x = 0;
	int y = 0;
	while (true) {
		x = (rand() % 18);
		y = (rand() % 18);
		if (m_virtualGameField[y][x] == EMPTY)
			break;
	}
	m_step++;
	moving(pushMove, x, y);
	m_currentMove = *pushMove.get();
}

void 	Gomoku::render()
{
	m_render->attachSharedLibrary("lib1_sdl.so", N + 1, N + 1);
	m_render->init();
	m_render->renderConfigure(&m_currentMove, &m_event, &m_exit, &m_turnTime, m_AI, &m_windowCondition, &m_AIType, &m_step, &m_putTwoStone);
	m_turnTime =0.0;
	m_currentMove.emptyGameField();
	if (m_startGame == PRO_FIELD)
		initPRO(m_currentMove);
	else if (m_startGame == SWAP_FIELD || m_startGame == SWAP_TWO_FIELD)
		initSWAP(m_currentMove);
	m_start = clock();
	while (!m_exit) {
		m_render->mainLoop();
		//if (!(m_startGame == SWAP_FIELD && m_step == 3)){
		if (m_AI && m_currentMove.getCurrentType() == m_AIType){
			printf("step = %d\n", m_step);
			if (m_step == 0){
				pushToCentre(m_currentMove);
				//m_step++;
				continue;
			}
			auto currentMove = std::make_shared<Move>(m_currentMove);
			printf("AI_move\n");

			AI_Move(currentMove);
			printf("AI_move processed\n");
		}
	//}

		if (m_event.event == PUSH_SQUARE){
			printf("Push point with x =%d and y =%d\n", m_event.x, m_event.y);
			auto currentMove = std::make_shared<Move>(m_currentMove);
			m_step++;
			moving(currentMove, m_event.x, m_event.y);

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
		Queue.pop();
	}
}

//MovePtr 					Gomoku::findMove(MovePtr move)
// MovePtr 					Gomoku::findMove(MovePtr move)
// {
// 	MovePtr findedMove = new Move(*move);
// 	moveProcessing(findedMove);
// 	return findedMove;
// }

//MovePtr 			Gomoku::algorithmMiniMax(MovePtr currentMove, int depth, int maxDepth, alfaBeta ab)

MovePtr 			Gomoku::algorithmMiniMax(MovePtr currentMove, int depth, int maxDepth, alfaBeta ab)
{
	movePriorityQueue movingOptions;
	MovePtr findedMove = nullptr;
	generateMoveOptions(currentMove, movingOptions);
	if (movingOptions.empty())
	{

	}
	printf("start algo depth = %d; maxDepth = %d; current turn = %d\n\n", depth, maxDepth, movingOptions.top()->getCurrentType());
	if (depth == maxDepth - 1) {
		printf("Leaf node finded; depth = %d; maxDepth = %d; current turn = %d\n", depth, maxDepth, movingOptions.top()->getCurrentType());
		//findedMove = findMove(movingOptions.top());
		findedMove = movingOptions.top();
		moveProcessing(findedMove);
		printMove(findedMove);
	} else {

		MovePtr checkingMove;
		MovePtr furtherMove; //= nullptr;
		MovePtr returnedMove;
		movePriorityQueue checkMovingOptions;
		int moveCounter = 0;
		int value = movingOptions.top()->getCurrentType() == BLACK ? alfaInf : betaInf;
		++depth;

		printf("Layer with depth = %d; maxDepth = %d; current turn = %d processed\n", depth, maxDepth, movingOptions.top()->getCurrentType());
		printf ("alfaBeta.alpha = %d alfaBeta.beta =  %d\n",ab.alpha,ab.beta);

		while (moveCounter < 3 && !movingOptions.empty()) {
			checkingMove = movingOptions.top();
			if (checkingMove->getResult() == WIN) {
				checkMovingOptions.push(checkingMove);
				movingOptions.pop();
				printf("FIND WIN\n");
				break;
			}
			furtherMove = std::make_shared<Move>(*checkingMove.get());
			moveProcessing(furtherMove);

			printf("moveCounter = %d; print processed furtherMove \n", moveCounter);
			printMove(furtherMove);


			returnedMove = algorithmMiniMax(furtherMove, depth, maxDepth, ab);
			moveCounter++;
			if (returnedMove == nullptr) {
				movingOptions.pop();
				continue ;
			}
			checkingMove->setHeuristic(returnedMove->getHeuristic());
			if (cutOff(checkingMove, ab, value))
				break;
 			checkMovingOptions.push(checkingMove);
			movingOptions.pop();
		}
		if (!checkMovingOptions.empty()){
			findedMove = checkMovingOptions.top();
			moveProcessing(findedMove);
			printf("print findedMove in depth =  %d \n", depth - 1);
			printMove(findedMove);
	 	}
		clearQueue(checkMovingOptions);
	}
	clearQueue(movingOptions);
	return findedMove;
}

void 		Gomoku::moving(MovePtr currentMove, int x, int y)
{
	m_checker->moveChecking(*currentMove.get(), x, y);
	// if (m_startGame == PRO_FIELD && m_step == 3) {
	// 	pushFarThenThreeFromCentre(*currentMove);
	// 	return;
	// }
	if (m_startGame == PRO_FIELD && m_step == 3) {
		for (int y =  6; y < 11; ++y) {
			for (int x = 6; x < 11; ++x){
				m_virtualGameField[y][x] = currentMove->getCurrentType();
			}
		}
		if (m_virtualGameField[y][x] != EMPTY){
			m_step--;
			return;
		}
	}
	if (m_startGame == SWAP_FIELD && m_step == 4) {
		if (m_currentMove.getCurrentType() == m_AIType)
			m_AIType = m_AIType == BLACK ? WHITE :  BLACK;
		printf("m_AIType = %d\n", m_AIType);
		//currentMove->setCurrentType(m_AIType == BLACK ? WHITE :  BLACK);
	} else if (m_startGame == SWAP_TWO_FIELD && (m_step == 4)) {
		if (!m_putTwoStone) {
			if (m_currentMove.getCurrentType() == m_AIType)
				m_AIType = m_AIType == BLACK ? WHITE :  BLACK;
			}
	}
	moveProcessing(currentMove);
	if (currentMove->getResult() == WIN) {
		winProcessing(currentMove);
	}
}

void 		Gomoku::AI_Move(MovePtr currentMove)
{

	m_turnTime =  0.0;
	m_start = clock();
	int depth = 0;
	alfaBeta ab;
	auto testedMove = std::make_shared<Move>(*currentMove);
	if (m_startGame == PRO_FIELD && m_step == 2) {
		pushFarThenThreeFromCentre(*testedMove.get());
		return;
	} else if (m_startGame == SWAP_FIELD && m_step == 3) {
		// printf("m_startGame == SWAP_FIELD && m_step == 3\n");
		// auto currentMove = std::make_shared<Move>(m_currentMove);
		// m_step++;
		// moving(currentMove, m_event.x, m_event.y);
		// m_AIType = m_AIType == BLACK ? WHITE :  BLACK;
		return;
	} else if (m_startGame == SWAP_TWO_FIELD ) {
		if (m_step == 3)
			return;
		if (m_putTwoStone && m_step == 4)
			return;
	}
	// } else if (m_startGame == SWAP_TWO_FIELD && (m_step == 4)){
	//
	// }
    m_step++;
	printf("minimax\n");
	auto bestMove = algorithmMiniMax(testedMove, depth, 5, ab);
	printf("minimax finished\n");
	m_turnTime = static_cast<double>((clock() - m_start ))/ CLOCKS_PER_SEC;
	m_currentMove = *bestMove.get();

	if (bestMove->getResult() == WIN) {
		winProcessing(bestMove);
	}



}

void 		Gomoku::generateMoveOptions(MovePtr currentMove, movePriorityQueue & movingOptions)
{
	printf("minimax generateMoveOptions\n");
	int lextX = currentMove->getLeftTop().x;
	int topY = currentMove->getLeftTop().y;
	int rightX = currentMove->getRightBottom().x;
	int bottomY = currentMove->getRightBottom().y;
	auto & gamefield = currentMove->getGameFieldMod();
	m_virtualGameField = gamefield;
	for (int y =  topY; y <= bottomY; ++y) {
		for (int x = lextX; x <= rightX; ++x){
		 	if (gamefield[y][x] != EMPTY ){
				fillMoveOptions(currentMove, x, y, movingOptions, m_virtualGameField);
			}
		}
	}
	printf("minimax generateMoveOptions finish\n");
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
			auto newMove = std::make_shared<Move>(*currentMove);
			m_checker->moveChecking(*newMove.get(), x, y);
			virtualGameField[y][x] = currentMove->getCurrentType();
			moveAI_Processing(newMove, movingOptions);
		}
	}
}

void 		Gomoku::moveProcessing(MovePtr currentMove)
{
	auto const & result = currentMove->getResult();
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
	auto const & result = optionMove->getResult();
	if (result == NON_EMPTY || result == DOUBLE_FREE_TREE){
		return ;
	}
	m_heuristicSolver->defineHeuristic(optionMove, result);
	movingOptions.push(optionMove);
}

void 		Gomoku::winProcessing(MovePtr currentMove)
{
	m_currentMove =  *currentMove.get();
	m_render->drawField(m_currentMove.getGameField());
	sleep(2);
	std::string win_reason = "";
	if (currentMove->getWhiteCapture() >= 5 || currentMove->getBlackCapture() >= 5)
		win_reason = "BY CAPTURED!";
	else
		win_reason = "BY ALLIGNMENT!";
	std::string result_str =  currentMove->getCurrentType() == BLACK ? "BLACK WIN : " : "WHITE WIN :";
	result_str += win_reason;
	std::cout << result_str << std::endl;
	m_currentMove.emptyGameField();
	m_currentMove.setCurrentType(BLACK);
	t_pos & leftTop = const_cast<t_pos &>(m_currentMove.getLeftTop());
	t_pos & rightBottom = const_cast<t_pos &>(m_currentMove.getRightBottom());
	leftTop.x = -1;
	leftTop.y = -1;
	rightBottom.x = -1;
	rightBottom.y = -1;
	m_currentMove.setWhiteCapture(0);
	m_currentMove.setBlackCapture(0);
	m_render->GameOver(result_str);
	m_step = 0;
	m_AIType = WHITE;
	m_turnTime = 0.0;
	m_putTwoStone = 0;;
	if(m_startGame == PRO_FIELD)
		initPRO(m_currentMove);
	else if (m_startGame == SWAP_FIELD || m_startGame == SWAP_TWO_FIELD)
		initSWAP(m_currentMove);
}
