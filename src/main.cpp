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

	Gomoku *gomoku;
	if (ac == 2)
		gomoku = new Gomoku(std::string(av[1]));
	else
	 	gomoku = new Gomoku("none");
	// while (1){
	// printf(" print %d \n", 2);
	// Move m;
	// printf(" print first m\n");
	// for (int i = 0; i < N; ++i){
	// 	for (int j = 0; j < N; ++j) {
	// 		printf("move x = %d y =%d type = %d\n", j, i, (int)m.getGameField()[i][j]);
	// 	}
	// }
	// m.setMove(0,0);
	// printf(" print second m\n");
	// for (int i = 0; i < N; ++i){
	// 	for (int j = 0; j < N; ++j) {
	// 		printf("move x = %d y =%d type = %d\n", j, i, (int)m.getGameField()[i][j]);
	// 	}
	// }
	// Move m1 = m;
	// printf(" print  m1!!!\n\n");
	// for (int i = 0; i < N; ++i){
	// 	for (int j = 0; j < N; ++j) {
	// 		printf("move x = %d y =%d type = %d\n", j, i, (int)m1.getGameField()[i][j]);
	// 	}
	// }
	// m.setMove(0,1);
	// Move m2;
	// m2 = m;
	// printf(" print  m!!!\n\n");
	// for (int i = 0; i < N; ++i){
	// 	for (int j = 0; j < N; ++j) {
	// 		printf("move x = %d y =%d type = %d\n", j, i, (int)m2.getGameField()[i][j]);
	// 	}
	// }
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
