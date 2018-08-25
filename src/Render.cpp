/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Render.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrassokh <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/25 17:57:21 by mrassokh          #+#    #+#             */
/*   Updated: 2018/08/25 17:57:22 by mrassokh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Render.hpp"

Render::Render()
{

}


Render::~Render()
{

}

Render		&Render::Instance()
{
	static Render instance;
	return instance;
}


void 		Render::dlerror_wrapper(void) {
	std::cerr << "Error:" << dlerror() << std::endl;
	exit(EXIT_FAILURE);
}


void 		Render::attachSharedLibrary(const char* sharedLibrary, int height, int weight)
{
	dl_handle = dlopen(sharedLibrary, RTLD_LAZY | RTLD_LOCAL);
	if (!dl_handle) {
		dlerror_wrapper();
	}
	m_windowCreator = reinterpret_cast<IWindow *(*)(int width, int height)>(dlsym(dl_handle, "createWindow"));
	m_newWindow = m_windowCreator(height, weight);
	m_newWindow->init();
	m_windowDestructor = reinterpret_cast<void (*)(IWindow*)>(dlsym(dl_handle, "deleteWindow"));
	if (!m_windowDestructor){
		dlerror_wrapper();
	}
}

void 		Render::deAttachSharedLibrary()
{
	m_windowDestructor(m_newWindow);
	dlclose(dl_handle);
}

// void 			drawCell(int x, int y, eType type) const
// {
//
// }

void 			Render::drawField(vecVecInt 	*gamefield) const
{
	m_newWindow->startCycl();
	vecVecInt game = *(gamefield);
	for (int i = 0; i < 19; i++){
		for (int j = 0; j < 19; j++){
			//m_newWindow->drawSquare(i, j, static_cast<eType>((*gamefield)[i]->at[j]));
			m_newWindow->drawSquare(i, j, static_cast<eType>(0));
		}
	}
	m_newWindow->endCycl();
}
