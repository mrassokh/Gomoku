/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sdlWindow.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrassokh <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/19 14:21:37 by mrassokh          #+#    #+#             */
/*   Updated: 2018/05/19 14:21:39 by mrassokh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/sdlWindow.hpp"
#include <unistd.h>

void SdlWindow::showText(int x, int y, const char *text) {
	int texW = 0;
	int texH = 0;
	int posX = x;
	int posY = y;
	SDL_Color color = { 255, 255, 255, 0 };
	m_textSurface = TTF_RenderText_Blended_Wrapped(m_font, text, color, m_width / 2);
	m_textTexture = SDL_CreateTextureFromSurface(m_renderer, m_textSurface);
	SDL_QueryTexture(m_textTexture, NULL, NULL, &texW, &texH);
	if (x == 0 && y == 0) {
		posX = (m_width - texW) / 2;
		posY = (m_height - texH) / 2;
	}
	SDL_Rect dstrect = { posX, posY, texW, texH  };
	SDL_RenderCopy(m_renderer, m_textTexture, NULL, &dstrect);
	SDL_FreeSurface(m_textSurface);
	SDL_DestroyTexture(m_textTexture);
}


SdlWindow::SdlWindow()
{

}

SdlWindow::SdlWindow(int width, int height)
{
	m_width = width * SQUARE_SIZE;
	m_height = height * SQUARE_SIZE;
	m_quit = false;

}

SdlWindow::~SdlWindow()
{
	TTF_CloseFont(m_font);
	TTF_Quit();
	SDL_DestroyRenderer(m_renderer);
	SDL_DestroyWindow(m_window);
	SDL_Quit();
}

void 					SdlWindow::init()
{
	SDL_Init(SDL_INIT_VIDEO);
	TTF_Init();
	m_window = SDL_CreateWindow("SDL2 line drawing",
		SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, m_width, m_height, 0);
	m_renderer = SDL_CreateRenderer(m_window, -1, 0);

	char path[1024];
	getcwd(path, sizeof(path));
	std::string fontPath = path;
	fontPath += "/fonts/arial.ttf";
	m_font = TTF_OpenFont(fontPath.c_str(), 24);
	if (m_font == NULL) {
		printf("error: font not found\n");
		exit(EXIT_FAILURE);
	}
}

void 					SdlWindow::startCycl()
{
	SDL_SetRenderDrawColor(m_renderer, 244, 164, 96, 255);
	SDL_RenderClear(m_renderer);
}

void 					SdlWindow::endCycl()
{
	SDL_RenderPresent(m_renderer);
}

void 					SdlWindow::drawTime(double time, eType type)
{
	//
	std::ostringstream results;
	results << "Turn : "  << (type == BLACK ? "BLACK" : "WHITE") << "\nTimer: " + std::to_string(time)+ "\n";
	std::string output = results.str();
	//	std::string results("Turn : "  + std::to_string("BLACK") + "\nTimer: - " + std::to_string(time)+ "\n");
	showText(m_width - 200, 50,output.c_str());
}
//
// void 					SdlWindow::drawScore(int score, int velocity, eType type, int mult)
// {
// 	std::string results("score - " +std::to_string(score)+ "\nvelocity - " +std::to_string(velocity)+ "\n");
// 	if (!mult){
// 		showText(m_width - 200, 50, results.c_str());
// 	} else {
// 		if (type == SNAKE_HEAD) {
// 			results.insert(0, "FIRST PLAYER:\n");
// 			showText(m_width - 200, 50, results.c_str());
// 		} else if (type == SNAKE_SECOND_HEAD){
// 			results.insert(0, "SECOND PLAYER:\n");
// 			showText(m_width - 200, 150, results.c_str());
// 		}
// 	}
// }
//
// // void 					SdlWindow::quit(std::string const & finishMessage)
// // {
// // 	printf("%s\n", finishMessage.c_str());
// // }
//
void 			SdlWindow::getEvent(event *ev)
{
	while (SDL_PollEvent(&m_event)){
		if (m_event.type == SDL_MOUSEWHEEL)
			return ;
		if (m_event.type == SDL_QUIT){
			printf ("events!!!! EXIT in sdlWindow\n");
			ev->event = EXIT;
			return ;
		}
		else if (m_event.type == SDL_KEYDOWN)
		{
			handleKeyDown(m_event.key.keysym.sym, ev);
			return;
		} else if (m_event.type == SDL_MOUSEBUTTONDOWN
					&& m_event.button.button == SDL_BUTTON_LEFT
						&& m_event.button.state == SDL_PRESSED){
			handleMouseDown(m_event.button.x, m_event.button.y, ev);
			return;
		}
	}
	ev->event = DEFAULT;
	return ;
}

void 			SdlWindow::handleKeyDown(int key, event *ev) const
{
	if (key == SDLK_ESCAPE){
		ev->event = EXIT;
		return ;
	}
	if (key == SDLK_n){
		ev->event = NEW_GAME;
		return ;
	}
	if (key == SDLK_p) {
		ev->event = PUSH_SQUARE;
		ev->x = 10;
		ev->y = 10;
		return;
	}
	ev->event = DEFAULT;
	return ;
}

void 			SdlWindow::handleMouseDown(int x, int y, event *ev) const
{
	if (x < SQUARE_SIZE_HALF || x > SIDE_SIZE - SQUARE_SIZE_HALF
		|| y < SQUARE_SIZE_HALF || y > SIDE_SIZE - SQUARE_SIZE_HALF)
		return;
	ev->event = PUSH_SQUARE;
	ev->x = (x - SQUARE_SIZE_HALF) / SQUARE_SIZE;
	ev->y = (y - SQUARE_SIZE_HALF) / SQUARE_SIZE;
	return ;
}
//
void 			SdlWindow::drawLine(int i, int j)
{
	int x_start = j * SQUARE_SIZE;
	int y_start = i * SQUARE_SIZE;
	int x_end = i ? SIDE_SIZE : x_start;
	int y_end = i ? i * SQUARE_SIZE : SIDE_SIZE;
	SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 255);
	SDL_RenderDrawLine(m_renderer, x_start, y_start, x_end, y_end);
}


void 			SdlWindow::drawTile(int x, int y, eType type)
{
	if (type == EMPTY){
		return;
	}
	SDL_Rect rectangle;

	rectangle.x = x * SQUARE_SIZE + 1.5 * SQUARE_SIZE_HALF;
	rectangle.y = y * SQUARE_SIZE + 1.5 * SQUARE_SIZE_HALF;
	rectangle.w = SQUARE_SIZE_HALF;
	rectangle.h = SQUARE_SIZE_HALF;

	if (type == BLACK) {
		SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 255);
	} else if (type == WHITE) {
		SDL_SetRenderDrawColor(m_renderer, 255, 255, 255, 255);
	}
	SDL_RenderFillRect(m_renderer, &rectangle);

}
// void 			SdlWindow::drawSquare(int x, int y, eType type)
// {
// 	SDL_Rect rectangle;
//
// 	rectangle.x = x *  SQUARE_SIZE;
// 	rectangle.y = y * SQUARE_SIZE;
// 	rectangle.w = SQUARE_SIZE;
// 	rectangle.h = SQUARE_SIZE;
// 	if (type == EMPTY){
// 		SDL_SetRenderDrawColor(m_renderer, 165, 42, 42, 255);
// 	} else if (type == BLACK) {
// 		SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 255);
// 	} else if (type == WHITE) {
// 		SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 255);
// 	}
// 	SDL_RenderFillRect(m_renderer, &rectangle);
// }
//
void 				SdlWindow::drawStart()
{
	std::string text("For start new game press <<N>>\nFor Exit press <<ECS>>");

	SDL_SetRenderDrawColor(m_renderer, 32,178,170, 255);
	SDL_RenderClear(m_renderer);
	showText(0, 0, text.c_str());
	SDL_RenderPresent(m_renderer);
}

void 				SdlWindow::drawGameOver(std::string const & finishMessage)
{
	SDL_SetRenderDrawColor(m_renderer, 232, 178, 170, 255);
	SDL_RenderClear(m_renderer);

	std::string message = finishMessage;
	message += "\nFor start new game press <<N>>\nFor Exit press <<ECS>>";
	showText(0, 0, message.c_str());
	SDL_RenderPresent(m_renderer);
}

SdlWindow		*createWindow(int width, int height)
{
	return new SdlWindow(width, height);
}

void 		deleteWindow(SdlWindow *sdlWindow)
{
	delete sdlWindow;
}
