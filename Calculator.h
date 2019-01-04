#pragma once
#ifndef _CALCULATOR_H
#define _CALCULATOR_H

#include "system.h"
#include <sstream>
#include <map>
#include<time.h>
#include <queue>
class STATE {
public:
	short box[MAX_BOX];
	

	bool operator<(const STATE b) const {
		for (int i = 0; (this->box[i] || i == 0) && i < MAX_BOX; i++) {
			if (this->box[i] < b.box[i]) return true;
			if (this->box[i] > b.box[i]) return false;
		}
		return false;
	}
	
	bool operator==(const STATE b) const {
		for (int i = 0; (this->box[i] || i == 0) && i < MAX_BOX; i++)
			if (this->box[i] != b.box[i]) return false;
		return true;
	}

};

class BOX_CALCULATOR {
public:
	BOX_CALCULATOR(const GameMap& Input_Calc_map, short Input_mode)
		: search_mode{ Input_mode }
		, trivial{ false }
		, total_time {0}
	{
		Calc_Map = Input_Calc_map;
		searched.clear();
		while (!srh_fwd.empty())
			srh_fwd.pop();
		cnt = 0;
		memset(solution, 0, sizeof(solution));
		judge_no_solution = false;
	};
	void do_Compute();
	short solution[Max_Depth], cnt;
	bool judge_no_solution;
	bool trivial;
	double total_time;
protected:
	GameMap Calc_Map;
private:
	//bool BFS;
	clock_t st, ed;
	short search_mode;
	std::map<STATE, short> searched, rev_searched;
	std::map<STATE, short>::iterator fqt_iter,rqt_iter;
	std::queue<STATE> srh_fwd, srh_rvs;
	bool search_way(STATE);
	bool BFS_Fwd(const STATE&);
	bool BFS_Rvs(const STATE&);
	bool judge_finished(const STATE&);
	bool judge_feasible(const STATE&);
	void get_solution(const STATE&);
	void get_rev_solution(const STATE&);
	bool BFS_Pure(const STATE&);
	bool BFS_judge_finished(const STATE&, bool);
	
};
//short get_GoalsNum();

#endif
