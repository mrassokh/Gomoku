/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Render.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrassokh <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/25 17:49:51 by mrassokh          #+#    #+#             */
/*   Updated: 2018/08/25 17:49:54 by mrassokh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RENDER_HPP
# define RENDER_HPP
# include "IWindow.hpp"
# include <vector>
# include <dlfcn.h>

typedef std::vector<std::vector<int> *> vecVecInt;

class Render
{
public:
	static Render	&Instance();
	void 			drawField(vecVecInt  *gamefield) const;
	void 			attachSharedLibrary(const char* sharedLibrary, int height, int weight);
	void 			deAttachSharedLibrary();

private:
	Render();
	Render(Render const & rhs) = delete;
	Render & operator = (Render const & rhs) = delete;
	virtual ~Render();

	void 			dlerror_wrapper();

	IWindow 		*(*m_windowCreator)(int width, int height);
	void			(*m_windowDestructor)(IWindow *);
	void 			*dl_handle;
	IWindow 		*m_newWindow;
};

#endif
