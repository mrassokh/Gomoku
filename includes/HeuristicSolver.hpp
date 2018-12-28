/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HeuristicSolver.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrassokh <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/24 16:57:07 by mrassokh          #+#    #+#             */
/*   Updated: 2018/12/24 16:57:12 by mrassokh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HEURISTIC_HPP
# define HEURISTIC_HPP
#include "Move.hpp"

typedef std::array<eType, 6> twoOpenSample;
typedef std::array<eType, 5> oneOpenSample;

// BLACK SAMPLES
static const twoOpenSample blackFourTwoOpen = {{EMPTY,BLACK,BLACK,BLACK,BLACK,EMPTY}};
static const oneOpenSample blackFourOneOpenLeft = {{EMPTY,BLACK,BLACK,BLACK,BLACK}};
static const oneOpenSample blackFourOneOpenRight = {{BLACK,BLACK,BLACK,BLACK,EMPTY}};
static const oneOpenSample blackFourOneOpenCenter_1 = {{BLACK,BLACK,EMPTY,BLACK,BLACK}};
static const oneOpenSample blackFourOneOpenCenter_2 = {{BLACK,EMPTY,BLACK,BLACK,BLACK}};
static const oneOpenSample blackFourOneOpenCenter_3 = {{BLACK,BLACK,BLACK,EMPTY,BLACK}};

static const twoOpenSample blackThreeTwoOpen_1 = {{EMPTY,BLACK,BLACK,BLACK,EMPTY,EMPTY}};
static const twoOpenSample blackThreeTwoOpen_2 = {{EMPTY,EMPTY,BLACK,BLACK,BLACK,EMPTY}};
static const oneOpenSample blackThreeOneOpenRight = {{BLACK,BLACK,BLACK,EMPTY,EMPTY}};
static const oneOpenSample blackThreeOneOpenLeft = {{EMPTY,EMPTY,BLACK,BLACK,BLACK}};
static const oneOpenSample blackThreeOneOpenCenter_1 = {{EMPTY,BLACK,EMPTY,BLACK,BLACK}};
static const oneOpenSample blackThreeOneOpenCenter_2 = {{EMPTY,BLACK,BLACK,EMPTY,BLACK}};
static const oneOpenSample blackThreeOneOpenCenter_3 = {{BLACK,EMPTY,BLACK,BLACK,EMPTY}};
static const oneOpenSample blackThreeOneOpenCenter_4 = {{BLACK,BLACK,EMPTY,BLACK,EMPTY}};

static const twoOpenSample blackTwoTwoOpen_1 = {{EMPTY,BLACK,BLACK,EMPTY,EMPTY,EMPTY}};
static const twoOpenSample blackTwoTwoOpen_2 = {{EMPTY,EMPTY,BLACK,BLACK,EMPTY,EMPTY}};
static const twoOpenSample blackTwoTwoOpen_3 = {{EMPTY,EMPTY,EMPTY,BLACK,BLACK,EMPTY}};

static const oneOpenSample blackTwoOneOpenRight = {{BLACK,BLACK,EMPTY,EMPTY,EMPTY}};
static const oneOpenSample blackTwoOneOpenLeft = {{EMPTY,EMPTY,EMPTY,BLACK,BLACK}};


// WHITE SAMPLES
static const twoOpenSample whiteFourTwoOpen= {{EMPTY,WHITE,WHITE,WHITE,WHITE,EMPTY}};
static const oneOpenSample whiteFourOneOpenLeft = {{EMPTY,WHITE,WHITE,WHITE,WHITE}};
static const oneOpenSample whiteFourOneOpenRight = {{WHITE,WHITE,WHITE,WHITE,EMPTY}};
static const oneOpenSample whiteFourOneOpenCenter_1 = {{WHITE,WHITE,EMPTY,WHITE,WHITE}};
static const oneOpenSample whiteFourOneOpenCenter_2 = {{WHITE,EMPTY,WHITE,WHITE,WHITE}};
static const oneOpenSample whiteFourOneOpenCenter_3 = {{WHITE,WHITE,WHITE,EMPTY,WHITE}};

static const twoOpenSample whiteThreeTwoOpen_1 = {{EMPTY,WHITE,WHITE,WHITE,EMPTY,EMPTY}};
static const twoOpenSample whiteThreeTwoOpen_2 = {{EMPTY,EMPTY,WHITE,WHITE,WHITE,EMPTY}};

static const oneOpenSample whiteThreeOneOpenRight = {{WHITE,WHITE,WHITE,EMPTY,EMPTY}};
static const oneOpenSample whiteThreeOneOpenLeft = {{EMPTY,EMPTY,WHITE,WHITE,WHITE}};
static const oneOpenSample whiteThreeOneOpenCenter_1 = {{EMPTY,WHITE,EMPTY,WHITE,WHITE}};
static const oneOpenSample whiteThreeOneOpenCenter_2 = {{EMPTY,WHITE,WHITE,EMPTY,WHITE}};
static const oneOpenSample whiteThreeOneOpenCenter_3 = {{WHITE,EMPTY,WHITE,WHITE,EMPTY}};
static const oneOpenSample whiteThreeOneOpenCenter_4 = {{WHITE,WHITE,EMPTY,WHITE,EMPTY}};

static const twoOpenSample whiteTwoTwoOpen_1 = {{EMPTY,WHITE,WHITE,EMPTY,EMPTY,EMPTY}};
static const twoOpenSample whiteTwoTwoOpen_2 = {{EMPTY,EMPTY,WHITE,WHITE,EMPTY,EMPTY}};
static const twoOpenSample whiteTwoTwoOpen_3 = {{EMPTY,EMPTY,EMPTY,WHITE,WHITE,EMPTY}};

static const oneOpenSample whiteTwoOneOpenRight = {{WHITE,WHITE,EMPTY,EMPTY,EMPTY}};
static const oneOpenSample whiteTwoOneOpenLeft = {{EMPTY,EMPTY,EMPTY,WHITE,WHITE}};


static const int winHeuristic = 1000000;
static const int fourTwoOpenHeuristic = 100000;
static const int fourOneOpenHeuristic = 10000;
static const int threeTwoOpenHeuristic = 10000;
static const int threeOneOpenHeuristic = 1000;
static const int twoTwoOpenHeuristic = 1000;
static const int oneTwoOpenHeuristic = 100;


class HeuristicSolver final {
public:
	HeuristicSolver(){};
	~HeuristicSolver(){};

	void 			 			defineHeuristic(MovePtr optionMove, eMoveResult result);

private:

	HeuristicSolver(HeuristicSolver const & rhs) = delete;
	HeuristicSolver& operator = (HeuristicSolver const & rhs) = delete;


	inline int 					defineHorizontalHeuristic(std::array<typeArr, N> & gamefield, eType currentTurn,
																		t_pos const & leftTop, t_pos const & rightBottom);
	inline int 					defineVerticalHeuristic(std::array<typeArr, N> & gamefield, eType currentTurn,
																			t_pos const & leftTop, t_pos const & rightBottom);
	inline int 					defineDiagonalLeftHeuristic(std::array<typeArr, N> & gamefield, eType currentTurn,
																			t_pos const & leftTop, t_pos const & rightBottom);
	inline int 					defineDiagonalRightHeuristic(std::array<typeArr, N> & gamefield, eType currentTurn,
																			t_pos const & leftTop, t_pos const & rightBottom);

   	inline int					resolveHeuristicFromField(std::vector<std::vector<eType>> const & heuristicField, eType currentTurn);
	inline int 					defineTwoOpenHeuristic(twoOpenSample const & blackSample, twoOpenSample const & whiteSample,
																std::vector<std::vector<eType>> const & gamefield, const int heuristic, eType currentTurn);

	inline int 					defineOneOpenHeuristic( oneOpenSample const & blackSample, oneOpenSample const & whiteSample,
																std::vector<std::vector<eType>> const & gamefield, const int heuristic, eType currentTurn);
  	inline int 					findTwoOpen(twoOpenSample const & sampleTwoOpen, std::vector<std::vector<eType>> const & gamefield);
  	inline int 					findOneOpen(oneOpenSample const & sampleOneOpen, std::vector<std::vector<eType>> const & gamefield);

	std::vector<std::vector<eType>>		m_heuristicField;
};

#endif
