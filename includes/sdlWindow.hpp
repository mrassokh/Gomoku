/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sdlWindow.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrassokh <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/19 14:08:52 by mrassokh          #+#    #+#             */
/*   Updated: 2018/05/19 14:09:00 by mrassokh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SDL_WINDOW_HPP
# define SDL_WINDOW_HPP
# include "IWindow.hpp"
# include "../lib1_sdl/SDL2.framework/Headers/SDL.h"
# include "../lib1_sdl/SDL2_ttf.framework/Headers/SDL_ttf.h"

# define SQUARE_SIZE 40
# define SQUARE_SIZE_HALF SQUARE_SIZE / 2
# define SIDE_SIZE SQUARE_SIZE * 19



class SdlWindow : public IWindow
{
public:
	SdlWindow(int width, int height);
	virtual ~SdlWindow();
	virtual void 					getEvent(event *ev);
	virtual void 					drawTile(int x, int y, eType type);
	virtual void 					drawLine(int i, int j);
	virtual void 					drawTime(eType type, int wCaptures, int bCaptures, double *turnTime, int AI);
	virtual void 					drawStart();
	virtual void 					drawGameOver(std::string const & finishMessage);
	virtual void 					startCycl(void);
	virtual void 					endCycl(void);
	virtual void 					init(void);

	void 							handleKeyDown(int key, event *ev) const;
	void							handleMouseDown(int x, int y, event *ev) const;
private:
	SdlWindow();
	void					showText(int x, int y, const char *text);
	void 					DrawCircle(int x, int y, int radius);
	int 					m_width;
	int 					m_height;
	SDL_Event 				m_event;
	SDL_Window 				*m_window;
	SDL_Renderer 			*m_renderer;
	SDL_Surface				*m_textSurface;
	SDL_Texture				*m_textTexture;
	TTF_Font				*m_font;
	bool 					m_quit;


};

extern "C" {
	SdlWindow		*createWindow(int width, int height);
	void 			deleteWindow(SdlWindow *sdlWindow);
}

#endif
