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
	m_startGame = startCondition;
	m_putTwoStone = 0;
	srand(time(NULL));
}

Gomoku::~Gomoku()
{

}


void 	Gomoku::render()
{
	m_render->attachSharedLibrary("lib1_sdl.so", N + 1, N + 1);
	m_render->init();
	m_render->renderConfigure(&m_currentMove, &m_event, &m_exit, &m_turnTime, m_AI, &m_windowCondition, &m_AIType, &m_step, &m_putTwoStone);
	m_turnTime = 0.0;
	m_currentMove.emptyGameField();
	if (m_startGame == PRO_FIELD)
		initPRO(m_currentMove);
	else if (m_startGame == SWAP_FIELD || m_startGame == SWAP_TWO_FIELD)
		initSWAP(m_currentMove);
	m_start = clock();

	while (!m_exit) {
		m_render->mainLoop();
		if (m_AI && m_currentMove.getCurrentType() == m_AIType){
			if (m_step == 0){
				pushToCentre(m_currentMove);
				continue;
			}
			auto currentMove = std::make_shared<Move>(m_currentMove);
			AI_Move(currentMove);
		}

		if (m_event.event == PUSH_SQUARE){
			auto currentMove = std::make_shared<Move>(m_currentMove);
			m_step++;
			moving(currentMove, m_event.x, m_event.y);

		}
	}
	m_render->deAttachSharedLibrary();
}

void 		Gomoku::moving(MovePtr currentMove, int x, int y)
{
	if (!movingStartProcessing(currentMove, x, y))
		return;
	m_checker->moveChecking(*currentMove.get(), x, y);
	moveProcessing(currentMove);
	if (currentMove->getResult() == WIN) {
		winProcessing(currentMove);
	}
}

int 		Gomoku::movingStartProcessing(MovePtr currentMove, int x, int y)
{
	if (m_startGame == PRO_FIELD && m_step == 3) {
		for (int y =  6; y < 11; ++y) {
			for (int x = 6; x < 11; ++x){
				m_virtualGameField[y][x] = currentMove->getCurrentType();
			}
		}
		if (m_virtualGameField[y][x] != EMPTY){
			m_step--;
			return 0;
		}
	}
	if (m_startGame == SWAP_FIELD && m_step == 4) {
		if (m_currentMove.getCurrentType() == m_AIType)
			m_AIType = m_AIType == BLACK ? WHITE :  BLACK;
	} else if (m_startGame == SWAP_TWO_FIELD && (m_step == 4)) {
		if (!m_putTwoStone) {
			if (m_currentMove.getCurrentType() == m_AIType)
				m_AIType = m_AIType == BLACK ? WHITE :  BLACK;
			}
	}
	return 1;
}

int 		Gomoku::movingAIStartProcessing(MovePtr testedMove)
{
	if (m_startGame == PRO_FIELD && m_step == 2) {
		pushFarThenThreeFromCentre(*testedMove.get());
		return 0;
	} else if (m_startGame == SWAP_FIELD && m_step == 3) {
		return 0;
	} else if (m_startGame == SWAP_TWO_FIELD ) {
		if (m_step == 3)
			return 0;
		if (m_putTwoStone && m_step == 4)
			return 0;
	}
	return 1;
}


void 		Gomoku::AI_Move(MovePtr currentMove)
{

	m_turnTime =  0.0;
	m_start = clock();
	int depth = 0;
	alfaBeta ab;
	auto testedMove = std::make_shared<Move>(*currentMove);

	if (!movingAIStartProcessing(testedMove))
		return ;

    m_step++;
	auto bestMove = algorithmMiniMax(testedMove, depth, maximumDepth, ab);
	m_turnTime = static_cast<double>((clock() - m_start ))/ CLOCKS_PER_SEC;
	if (!bestMove)
		return ;
	m_currentMove = *bestMove.get();

	if (bestMove->getResult() == WIN) {
		winProcessing(bestMove);
	}
}

void 		Gomoku::generateMoveOptions(MovePtr currentMove, movePriorityQueue & movingOptions)
{
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

MovePtr 			Gomoku::algorithmMiniMax(MovePtr currentMove, int depth, int maxDepth, alfaBeta ab)
{
	movePriorityQueue movingOptions;
	MovePtr findedMove = nullptr;
	generateMoveOptions(currentMove, movingOptions);
	if (movingOptions.empty())
		return findedMove;
	if (depth == maxDepth - 1) {
		findedMove = movingOptions.top();
		moveProcessing(findedMove);
	} else {

		MovePtr checkingMove;
		MovePtr furtherMove;
		MovePtr returnedMove;
		movePriorityQueue checkMovingOptions;
		int moveCounter = 0;
		int value = movingOptions.top()->getCurrentType() == BLACK ? alfaInf : betaInf;
		++depth;

		while (moveCounter < moveChoseCount && !movingOptions.empty()) {
			checkingMove = movingOptions.top();
			if (checkingMove->getResult() == WIN) {
				checkMovingOptions.push(checkingMove);
				movingOptions.pop();
				break;
			}
			furtherMove = std::make_shared<Move>(*checkingMove.get());
			moveProcessing(furtherMove);

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
	 	}
		clearQueue(checkMovingOptions);
	}
	clearQueue(movingOptions);
	return findedMove;
}

bool 				Gomoku::cutOff(MovePtr checkingMove, alfaBeta & ab, int & value)
{
	if (checkingMove->getCurrentType() == BLACK){
		value = std::max(value, checkingMove->getHeuristic());
		ab.alpha = std::max(ab.alpha, value);
	} else {
		value = std::min(value, checkingMove->getHeuristic());
		ab.beta = std::min(ab.beta, value);
	}
	return ab.alpha > ab.beta;
}

void 					Gomoku::clearQueue(movePriorityQueue & Queue)
{
	while (!Queue.empty()) {
		Queue.pop();
	}
}

void 		Gomoku::initPRO(Move & currentMove)
{
	pushToCentre(currentMove);
	pushNearestToCentre(currentMove);
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

void 		Gomoku::pushFarThenThreeFromCentre(Move & currentMove)
{
	auto pushMove = std::make_shared<Move>(currentMove);
	auto & gamefield = currentMove.getGameFieldMod();
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

void 			Gomoku::preventInequalPush(std::array<typeArr, N> & virtualGameField, int x, int y, eType type)
{
	t_pos start;
	t_pos end;
	start.x = x - 5 > 0 ?  x - 5 : 0;
	end.x = x + 5 <= 17 ?  x + 5 : 17;
	start.y = y - 5 > 0 ?  y - 5 : 0;
	end.y = y + 5 <= 17 ?  y + 5 : 17;
	for (int i = start.x; i < end.x; ++i) {
		for (int j = start.y; j < end.y; ++j) {
			virtualGameField[j][i] = type;
		}
	}
}

void 		Gomoku::winProcessing(MovePtr currentMove)
{
	m_currentMove =  *currentMove.get();
	m_render->drawField(m_currentMove.getGameField());
	sleep(1);
	std::string win_reason = "";
	if (currentMove->getWhiteCapture() >= 5 || currentMove->getBlackCapture() >= 5)
		win_reason = "BY CAPTURED!";
	else
		win_reason = "BY ALLIGNMENT!";
	std::string result_str =  currentMove->getCurrentType() == BLACK ? "BLACK WIN : " : "WHITE WIN :";
	result_str += win_reason;
	std::cout << result_str << std::endl;
	printMove(currentMove);
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
	m_putTwoStone = 0;
	clearVirtualGamefield();
	if(m_startGame == PRO_FIELD)
		initPRO(m_currentMove);
	else if (m_startGame == SWAP_FIELD || m_startGame == SWAP_TWO_FIELD)
		initSWAP(m_currentMove);
}

void 		Gomoku::clearVirtualGamefield()
{
	for (int y =  0; y < N; ++y) {
		for (int x = 0; x < N; ++x){
			m_virtualGameField[y][x] = EMPTY;
		}
	}
}
