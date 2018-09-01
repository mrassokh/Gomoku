/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrassokh <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/25 11:35:01 by mrassokh          #+#    #+#             */
/*   Updated: 2018/08/25 11:35:05 by mrassokh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Gomoku.hpp"

int main(int ac, char **av)
{

	if (ac == 3)
		av[2][0] = 'a';
		printf(" print %d \n", 1);
	Gomoku *gomoku = new Gomoku;
	// while (1){
	printf(" print %d \n", 2);
		gomoku->render();
		printf(" print %d \n", 3);
	//}

	// dl_handle = dlopen("lib1_sdl.so", RTLD_LAZY | RTLD_LOCAL);
	// 	if (!dl_handle) {
	// 		dlerror_wrapper();
	// 	}
	// 	m_windowCreator = reinterpret_cast<IWindow *(*)(int width, int height)>(dlsym(dl_handle, "createWindow"));
	// 	if (!m_windowCreator) {
	// 		dlerror_wrapper();
	// 	}
	// 	printf("1\n");
	// 	m_newWindow = m_windowCreator(20,20);
	// 	m_newWindow->init();
	// 		printf("1_1\n");
	// 	while (1) {
	// 		m_newWindow->startCycl();
	// 		m_newWindow->endCycl();
	// 	}
	// 	printf("2\n");
	//
	// 	m_windowDestructor = reinterpret_cast<void (*)(IWindow*)>(dlsym(dl_handle, "deleteWindow"));
	// 	printf("3\n");
	// 	if (!m_windowDestructor){
	// 		dlerror_wrapper();
	// 	}
	// 	printf("4\n");
	// 	m_windowDestructor(m_newWindow);
	// 	dlclose(dl_handle);

	return 0;
}
