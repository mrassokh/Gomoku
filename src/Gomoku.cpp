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

Gomoku::Gomoku(std::string input):m_N(18), m_exit(0)
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

void 		Gomoku::pushToCentre(Move & currentMove)
{
	auto type = currentMove.getCurrentType();
	currentMove.getGameFieldMod()[8][8] = type;
	type = BLACK ? WHITE :  BLACK;
	currentMove.setCurrentType(type);
	t_pos & leftTop = const_cast<t_pos &>(currentMove.getLeftTop());
	t_pos & rightBottom = const_cast<t_pos &>(currentMove.getRightBottom());
	leftTop.x = 8;
	leftTop.y = 8;
	rightBottom.x = 8;
	rightBottom.y = 8;
}

void 	Gomoku::render()
{
	m_render->attachSharedLibrary("lib1_sdl.so", N + 1, N + 1);
	m_render->init();
	m_render->renderConfigure(&m_currentMove, &m_event, &m_exit, &m_turnTime, m_AI, &m_windowCondition, &m_AIType, &m_step);
	m_turnTime =0.0;
	m_currentMove.emptyGameField();
	m_start = clock();
	while (!m_exit) {
		m_render->mainLoop();

		if (m_AI && m_currentMove.getCurrentType() == m_AIType){
			printf("staep = %d\n", m_step);
			if (m_step++ == 0){
				pushToCentre(m_currentMove);
				continue;
			}
			auto currentMove = std::make_shared<Move>(m_currentMove);
			printf("AI_move\n");

			AI_Move(currentMove);
			printf("AI_move processed\n");
		}

		if (m_event.event == PUSH_SQUARE){
			printf("Push point with x =%d and y =%d\n", m_event.x, m_event.y);
			auto currentMove = std::make_shared<Move>(m_currentMove);
			moving(currentMove);
			m_step++;
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

void 		Gomoku::moving(MovePtr currentMove)
{
	m_checker->moveChecking(*currentMove.get(), m_event.x, m_event.y);
	moveProcessing(currentMove);
	if (currentMove->getResult() == WIN) {
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
		m_currentMove.emptyGameField();
		std::cout << result_str << std::endl;
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
		m_turnTime =0.0;

	}
}

void 		Gomoku::AI_Move(MovePtr currentMove)
{

	m_turnTime =  0.0;
	m_start = clock();
	int depth = 0;
	alfaBeta ab;
	auto testedMove = std::make_shared<Move>(*currentMove);
	printf("minimax\n");
	auto bestMove = algorithmMiniMax(testedMove, depth, 5, ab);
	printf("minimax finished\n");
	m_turnTime = static_cast<double>((clock() - m_start ))/ CLOCKS_PER_SEC;
	m_currentMove = *bestMove.get();
	if (bestMove->getResult() == WIN) {
		m_render->drawField(m_currentMove.getGameField());
		sleep(2);
		std::string win_reason = "";
		if (bestMove->getWhiteCapture() >= 5 || bestMove->getBlackCapture() >= 5)
			win_reason = "BY CAPTURED!";
		else
			win_reason = "BY ALLIGNMENT!";
		std::string result_str =  bestMove->getCurrentType() == BLACK ? "BLACK WIN : " : "WHITE WIN :";
		result_str += win_reason;
		printMove(bestMove);
		m_currentMove.emptyGameField();
		std::cout << result_str << std::endl;
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
		m_turnTime =0.0;

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
