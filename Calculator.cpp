#include "Calculator.h"
#include<iostream>

using namespace std;

int depth_limit = 300;

bool BOX_CALCULATOR::judge_finished(const STATE &status) {
	for (int i = 0; i < Calc_Map.Goals_num; i++)
		if (!Calc_Map.Game_Goal[(status.box[i] >> 4) / (MAX_LEN - 2) + 1][(status.box[i] >> 4) % (MAX_LEN - 2) + 1])
			return false;
	//所有的箱子都到位
	judge_no_solution = false;
	return true;
}
bool BOX_CALCULATOR::judge_feasible(const STATE &status) {
	//剪枝
	//若状态重复，则直接返回
	map<STATE, short>::iterator iter;
	iter = searched.find(status);
	if (search_mode != 2) {
		if (iter != searched.end()) return false;
	}else{
		if (iter != searched.end()) {
			if(iter->second >> 8 == depth_limit) return false; //这个深度已经搜索过
			if ((iter->second & 127) < cnt) return false; //不是期望的最短搜索路径
			//更新这个状态对应的最大搜索深度
			iter->second &= 127;
			iter->second |= (char)depth_limit << 8;
			return true;
		}
	}
	memset(Calc_Map.Game_box, 0, sizeof(Calc_Map.Game_box));
	for (int i = 0; i < Calc_Map.Goals_num; i++)
		Calc_Map.Game_box[(status.box[i] >> 4) / (MAX_LEN - 2) + 1][(status.box[i] >> 4) % (MAX_LEN - 2) + 1] = true;
    for (int i = 0; i < Calc_Map.Goals_num; i++){
		int bx = (status.box[i] >> 4) / (MAX_LEN - 2) + 1, by = (status.box[i] >> 4) % (MAX_LEN - 2) + 1;
        if(!Calc_Map.Game_Goal[bx][by])
            if(Calc_Map.Game_Wall[bx-1][by]|| Calc_Map.Game_Wall[bx+1][by])
                if(Calc_Map.Game_Wall[bx][by-1]|| Calc_Map.Game_Wall[bx][by+1])
                    return false;//剪枝->有箱子在角落

		if (Calc_Map.Game_box[bx + 1][by]) {
			if (Calc_Map.Game_Wall[bx][by - 1])
				if (Calc_Map.Game_Wall[bx + 1][by - 1] || Calc_Map.Game_Wall[bx + 1][by + 1] || Calc_Map.Game_box[bx + 1][by - 1])
                    if(!Calc_Map.Game_Goal[bx][by]||!Calc_Map.Game_Goal[bx+1][by]||(Calc_Map.Game_box[bx + 1][by - 1] && !Calc_Map.Game_Goal[bx+1][by-1]))
                        return false;//剪枝->x并排箱子与左右墙或单墙箱子
			if (Calc_Map.Game_Wall[bx][by + 1])
				if (Calc_Map.Game_Wall[bx + 1][by - 1] || Calc_Map.Game_Wall[bx + 1][by + 1] || Calc_Map.Game_box[bx + 1][by + 1])
                    if(!Calc_Map.Game_Goal[bx][by]||!Calc_Map.Game_Goal[bx+1][by]|| (Calc_Map.Game_box[bx + 1][by + 1] && !Calc_Map.Game_Goal[bx + 1][by + 1]))
                        return false;//剪枝->x并排箱子与左右墙或单墙箱子
			if (Calc_Map.Game_box[bx][by + 1] && Calc_Map.Game_Wall[bx + 1][by + 1])
				if (!Calc_Map.Game_Goal[bx][by] || !Calc_Map.Game_Goal[bx + 1][by + 1] || !Calc_Map.Game_Goal[bx][by + 1])
					return false;
			
			if (!Calc_Map.Game_Wall[bx + 1][by + 1] && !Calc_Map.Game_Wall[bx][by + 1] && Calc_Map.Game_Wall[bx - 1][by + 1] && Calc_Map.Game_Wall[bx + 2][by + 1])
				if (Calc_Map.Game_Wall[bx][by + 2] && Calc_Map.Game_Wall[bx + 1][by + 2] && !(status.box[i] & (1 << 1)))
					if ((Calc_Map.Game_Goal[bx][by] == true) + (Calc_Map.Game_Goal[bx][by + 1] == true) + (Calc_Map.Game_Goal[bx + 1][by] == true) + (Calc_Map.Game_Goal[bx + 1][by + 1] == true) < 2)
						return false;
			if (!Calc_Map.Game_Wall[bx + 1][by - 1] && !Calc_Map.Game_Wall[bx][by - 1] && Calc_Map.Game_Wall[bx - 1][by - 1] && Calc_Map.Game_Wall[bx + 2][by - 1])
				if (Calc_Map.Game_Wall[bx][by - 2] && Calc_Map.Game_Wall[bx + 1][by - 2] && !(status.box[i] & 1))
					if ((Calc_Map.Game_Goal[bx][by] == true) + (Calc_Map.Game_Goal[bx][by - 1] == true) + (Calc_Map.Game_Goal[bx + 1][by] == true) + (Calc_Map.Game_Goal[bx + 1][by - 1] == true) < 2)
						return false;
			if (Calc_Map.Game_box[bx][by + 1] && Calc_Map.Game_box[bx + 1][by + 1])
				if (!Calc_Map.Game_Goal[bx][by] || !Calc_Map.Game_Goal[bx + 1][by] || !Calc_Map.Game_Goal[bx + 1][by + 1] || !Calc_Map.Game_Goal[bx][by + 1])
                    return false;//剪枝->2*2方格
        }
		if (Calc_Map.Game_box[bx][by + 1]) {
			if (Calc_Map.Game_Wall[bx - 1][by])
				if (Calc_Map.Game_Wall[bx - 1][by + 1] || Calc_Map.Game_Wall[bx + 1][by + 1])
					if (!Calc_Map.Game_Goal[bx][by] || !Calc_Map.Game_Goal[bx][by + 1])
                        return false;//剪枝->y并排箱子与左右墙或单墙箱子
			if (Calc_Map.Game_Wall[bx + 1][by])
				if (Calc_Map.Game_Wall[bx - 1][by + 1] || Calc_Map.Game_Wall[bx + 1][by + 1] || Calc_Map.Game_box[bx + 1][by + 1])
					if (!Calc_Map.Game_Goal[bx][by] || !Calc_Map.Game_Goal[bx][by + 1] || (Calc_Map.Game_box[bx + 1][by + 1] && !Calc_Map.Game_Goal[bx + 1][by + 1]))
                        return false;//剪枝->y并排箱子与左右墙或单墙箱子
			
			if (!Calc_Map.Game_Wall[bx + 1][by + 1] && !Calc_Map.Game_Wall[bx + 1][by] && Calc_Map.Game_Wall[bx + 1][by - 1] && Calc_Map.Game_Wall[bx + 1][by + 2])
				if (Calc_Map.Game_Wall[bx + 2][by] && Calc_Map.Game_Wall[bx + 2][by + 1] && !(status.box[i] & (1 << 3)))
					if ((Calc_Map.Game_Goal[bx][by] == true) + (Calc_Map.Game_Goal[bx][by + 1] == true) + (Calc_Map.Game_Goal[bx + 1][by] == true) + (Calc_Map.Game_Goal[bx + 1][by + 1] == true) < 2)
						return false;
			if (!Calc_Map.Game_Wall[bx - 1][by + 1] && !Calc_Map.Game_Wall[bx - 1][by] && Calc_Map.Game_Wall[bx - 1][by - 1] && Calc_Map.Game_Wall[bx - 1][by + 2])
				if (Calc_Map.Game_Wall[bx - 2][by] && Calc_Map.Game_Wall[bx - 2][by + 1] && !(status.box[i] & (1 << 2)))
					if ((Calc_Map.Game_Goal[bx][by] == true) + (Calc_Map.Game_Goal[bx][by + 1] == true) + (Calc_Map.Game_Goal[bx - 1][by] == true) + (Calc_Map.Game_Goal[bx - 1][by + 1] == true) < 2)
						return false;
        }
    }
	if (search_mode < 3)
		searched.insert(pair<STATE, short>(status, char(depth_limit << 8 | cnt)));
	judge_no_solution = false;
	return true;
}
bool BOX_CALCULATOR::search_way(STATE current) {
	if (cnt >= depth_limit - 1) return false;
	if (judge_finished(current)) return true;
	if (!judge_feasible(current)) return false;
	if (force_stop) {
		//force_stop = false;
		return true;
	}
	short bx, by;
	STATE temp = current;
	for (int i = 0; i < Calc_Map.Goals_num; i++) {
		bx = (temp.box[i] >> 4) / (MAX_LEN - 2) + 1, by = (temp.box[i] >> 4) % (MAX_LEN - 2) + 1;
		if (temp.box[i] & (1 << 3)) {
			//可以将当前箱子向上推
			temp.box[i] >>= 4;
			solution[cnt++] = (temp.box[i] << 4) | (1 << 3);
			temp.box[i] -= MAX_LEN - 2;
			temp.box[i] <<= 4;
			Calc_Map.update_box(temp.box, bx, by);
			if(search_way(temp)) return true;
			cnt--;
			temp = current;
		}
		if (temp.box[i] & (1 << 2)) {
			//可以将当前箱子向下推
			temp.box[i] >>= 4;
			solution[cnt++] = (temp.box[i] << 4) | (1 << 2);
			temp.box[i] += MAX_LEN - 2;
			temp.box[i] <<= 4;
			Calc_Map.update_box(temp.box, bx, by);
			if (search_way(temp)) return true;
			cnt--;
			temp = current;
		}
		if (temp.box[i] & (1 << 1)) {
			//可以将当前箱子向左推
			temp.box[i] >>= 4;
			solution[cnt++] = (temp.box[i] << 4) | (1 << 1);
			temp.box[i] -= 1;
			temp.box[i] <<= 4;
			Calc_Map.update_box(temp.box, bx, by);
			if (search_way(temp)) return true;
			cnt--;
			temp = current;
		}
		if (temp.box[i] & 1) {
			//可以将当前箱子向右推
			temp.box[i] >>= 4;
			solution[cnt++] = (temp.box[i] << 4) | 1;
			temp.box[i] += 1;
			temp.box[i] <<= 4;
			Calc_Map.update_box(temp.box, bx, by);
			if (search_way(temp)) return true;
			cnt--;
			temp = current;
		}
	}
	return false;
}

bool BOX_CALCULATOR::BFS_judge_finished(const STATE& status, bool srh_sec) {
	if (srh_sec) {
		//第一个比第二个
		if (rev_searched.find(status) == rev_searched.end()) return false;
	}else {
		//第二个比第一个
		if (searched.find(status) == searched.end()) return false;
	}
	return true;
}
bool BOX_CALCULATOR::BFS_Rvs(const STATE& current) {
	//if (!judge_feasible(last_status)) return;
	
	STATE temp = current;
	for (int i = 0; i < Calc_Map.Goals_num; i++) {
		short bx = (current.box[i] >> 4) / (MAX_LEN - 2) + 1, by = (current.box[i] >> 4) % (MAX_LEN - 2) + 1;
		short oprt = (current.box[i] >> 4) << 4;
		if (temp.box[i] & (1 << 3)) {
			//可以把这个向下拉
			temp.box[i] >>= 4;
			temp.box[i] += MAX_LEN - 2;
			temp.box[i] <<= 4;
			//正向更新地图数据
			Calc_Map.update_box(temp.box, bx + 2, by);
			if (BFS_judge_finished(temp, false)) {
				rev_searched.insert(pair<STATE, short>(temp, oprt | (1 << 3)));
				fqt_iter = searched.find(temp);
				rqt_iter = rev_searched.find(temp);
				return true;
			}
			if (!BFS_judge_finished(temp, true)) {
				//判重存的是下一步走法，队列存的是上一步走法
				rev_searched.insert(pair<STATE, short>(temp, oprt | (1 << 3)));
				Calc_Map.update_rev_box(temp.box, bx + 2, by, true);
				srh_rvs.push(temp);
			}
			temp = current;
		}
		if (temp.box[i] & (1 << 2)) {
			//可以把这个向上拉
			temp.box[i] >>= 4;
			temp.box[i] -= MAX_LEN - 2;
			temp.box[i] <<= 4;
			//正向更新地图数据
			Calc_Map.update_box(temp.box, bx - 2, by);
			if (BFS_judge_finished(temp, false)) {
				rev_searched.insert(pair<STATE, short>(temp, oprt | (1 << 2)));
				fqt_iter = searched.find(temp);
				rqt_iter = rev_searched.find(temp);
				return true;
			}
			if (!BFS_judge_finished(temp, true)) {
				rev_searched.insert(pair<STATE, short>(temp, oprt | (1 << 2)));
				Calc_Map.update_rev_box(temp.box, bx - 2, by, true);
				srh_rvs.push(temp);
			}
			temp = current;
		}
		if (temp.box[i] & (1 << 1)) {
			//可以把这个向右拉
			temp.box[i] >>= 4;
			temp.box[i] +=  1;
			temp.box[i] <<= 4;
			//正向更新地图数据
			Calc_Map.update_box(temp.box, bx, by + 2);
			if (BFS_judge_finished(temp, false)) {
				rev_searched.insert(pair<STATE, short>(temp, oprt | (1 << 1)));
				fqt_iter = searched.find(temp);
				rqt_iter = rev_searched.find(temp);
				return true;
			}
			if (!BFS_judge_finished(temp, true)) {
				rev_searched.insert(pair<STATE, short>(temp, oprt | (1 << 1)));
				Calc_Map.update_rev_box(temp.box, bx, by + 2, true);
				srh_rvs.push(temp);
			}
			temp = current;
		}
		if (temp.box[i] & 1) {
			//可以把这个向左拉
			temp.box[i] >>= 4;
			temp.box[i] -= 1;
			temp.box[i] <<= 4;
			//正向更新地图数据
			Calc_Map.update_box(temp.box, bx, by - 2);
			if (BFS_judge_finished(temp, false)) {
				rev_searched.insert(pair<STATE, short>(temp, oprt | 1));
				fqt_iter = searched.find(temp);
				rqt_iter = rev_searched.find(temp);
				return true;
			}
			if (!BFS_judge_finished(temp, true)) {
				rev_searched.insert(pair<STATE, short>(temp, oprt | 1));
				Calc_Map.update_rev_box(temp.box, bx, by - 2, true);
				srh_rvs.push(temp);
			}
			temp = current;
		}
	}
	return false;
}
bool BOX_CALCULATOR::BFS_Pure(const STATE& init) {
	srh_fwd.push(init);
	searched.insert(pair<STATE, short>(init, 0));
	while (!srh_fwd.empty())
	{
		const STATE current = srh_fwd.front();
		STATE temp = current;
		short oprt;
		if (force_stop) {
			return false;
		}
		srh_fwd.pop();
		for (int i = 0; i < Calc_Map.Goals_num; i++) {
			short bx = (temp.box[i] >> 4) / (MAX_LEN - 2) + 1, by = (temp.box[i] >> 4) % (MAX_LEN - 2) + 1;
			oprt = (temp.box[i] >> 4) << 4;
			if (temp.box[i] & (1 << 3)) {
				//可以将当前箱子向上推
				temp.box[i] >>= 4;
				temp.box[i] -= MAX_LEN - 2;
				temp.box[i] <<= 4;
				if (judge_finished(temp)) {
					searched.insert(pair<STATE, short>(temp, oprt | (1 << 3)));
					fqt_iter = searched.find(temp);
					return true;
				}
				Calc_Map.update_box(temp.box, bx, by);
				if (judge_feasible(temp)) {
					searched.insert(pair<STATE, short>(temp, oprt | (1 << 3)));
					srh_fwd.push(temp);
				}
				temp = current;
			}
			if (temp.box[i] & (1 << 2)) {
				//可以将当前箱子向下推
				temp.box[i] >>= 4;
				temp.box[i] += MAX_LEN - 2;
				temp.box[i] <<= 4;
				if (judge_finished(temp)) {
					searched.insert(pair<STATE, short>(temp, oprt | (1 << 2)));
					fqt_iter = searched.find(temp);
					return true;
				}
				Calc_Map.update_box(temp.box, bx, by);
				if (judge_feasible(temp)) {
					searched.insert(pair<STATE, short>(temp, oprt | (1 << 2)));
					srh_fwd.push(temp);
				}
				temp = current;
			}
			if (temp.box[i] & (1 << 1)) {
				//可以将当前箱子向左推
				temp.box[i] >>= 4;
				temp.box[i] -= 1;
				temp.box[i] <<= 4;
				if (judge_finished(temp)) {
					searched.insert(pair<STATE, short>(temp, oprt | (1 << 1)));
					fqt_iter = searched.find(temp);
					return true;
				}
				Calc_Map.update_box(temp.box, bx, by);
				if (judge_feasible(temp)) {
					searched.insert(pair<STATE, short>(temp, oprt | (1 << 1)));
					srh_fwd.push(temp);
				}
				temp = current;
			}
			if (temp.box[i] & 1) {
				//可以将当前箱子向右推
				temp.box[i] >>= 4;
				temp.box[i] += 1;
				temp.box[i] <<= 4;
				if (judge_finished(temp)) {
					searched.insert(pair<STATE, short>(temp, oprt | 1));
					fqt_iter = searched.find(temp);
					return true;
				}
				Calc_Map.update_box(temp.box, bx, by);
				if (judge_feasible(temp)) {
					searched.insert(pair<STATE, short>(temp, oprt | 1));
					srh_fwd.push(temp);
				}
				temp = current;
			}
		}
		//以下是反向搜索
	}
	return false;
}

bool BOX_CALCULATOR::BFS_Fwd(const STATE& init) {
	srh_fwd.push(init);
	searched.insert(pair<STATE, short>(init, 0));
	vector<STATE> end_status;
	vector<short> cordi_rev;
	STATE last_state, temp_state;
	memset(last_state.box, 0, sizeof(last_state.box));
	int index = 0;

	//以下，加入最终状态
	for (int i = Calc_Map.Map_uper; i < Calc_Map.Map_uper+Calc_Map.Map_length; i++)
		for (int j = Calc_Map.Map_left; j < Calc_Map.Map_left+Calc_Map.Map_width; j++) {
			if (Calc_Map.Game_Goal[i][j])
				last_state.box[index++] = ((MAX_LEN - 2) * (i - 1) + (j - 1)) << 4;
		}
	

	memset(Calc_Map.current_map, 0, sizeof(Calc_Map.current_map));
	//以下，加入所有可能的上一步
	for (int i = Calc_Map.Map_uper; i < Calc_Map.Map_uper + Calc_Map.Map_length; i++)
		for (int j = Calc_Map.Map_left; j < Calc_Map.Map_left + Calc_Map.Map_width; j++) {
			if (Calc_Map.Game_Wall[i][j] || Calc_Map.Game_Goal[i][j] || Calc_Map.current_map[i][j] == -1)
				continue;
			temp_state = last_state;
			Calc_Map.update_rev_box(temp_state.box, i, j, false);
			end_status.push_back(temp_state);
			cordi_rev.push_back((i - 1) * (MAX_LEN - 2) + j - 1);
		}

	for (int i = 0; i < cordi_rev.size() ; i++) {
		temp_state = end_status[i];
		Calc_Map.update_box(temp_state.box, cordi_rev[i] / (MAX_LEN - 2) + 1, cordi_rev[i] % (MAX_LEN - 2) + 1);
		rev_searched.insert(pair<STATE, short>(temp_state, 0));
		BFS_Rvs(end_status[i]);
	}

	end_status.clear();
	while (!srh_fwd.empty())
	{
		const STATE current = srh_fwd.front();
		STATE temp = current;
		short oprt;
		if (force_stop) {
			return false;
		}
		srh_fwd.pop();
		for (int i = 0; i < Calc_Map.Goals_num; i++) {
			short bx = (temp.box[i] >> 4) / (MAX_LEN - 2) + 1, by = (temp.box[i] >> 4) % (MAX_LEN - 2) + 1;
			oprt = (temp.box[i] >> 4) << 4;
			if (temp.box[i] & (1 << 3)) {
				//可以将当前箱子向上推
				temp.box[i] >>= 4;
				temp.box[i] -= MAX_LEN - 2;
				temp.box[i] <<= 4;
				Calc_Map.update_box(temp.box, bx, by);
				if (BFS_judge_finished(temp, true)) {
					searched.insert(pair<STATE, short>(temp, oprt | (1 << 3)));
					fqt_iter = searched.find(temp);
					rqt_iter = rev_searched.find(temp);
					return true;
				}
				if (judge_feasible(temp)) {
					searched.insert(pair<STATE, short>(temp, oprt | (1 << 3)));
					srh_fwd.push(temp);
				}
				temp = current;
			}
			if (temp.box[i] & (1 << 2)) {
				//可以将当前箱子向下推
				temp.box[i] >>= 4;
				temp.box[i] += MAX_LEN - 2;
				temp.box[i] <<= 4;
				Calc_Map.update_box(temp.box, bx, by);
				if (BFS_judge_finished(temp, true)) {
					searched.insert(pair<STATE, short>(temp, oprt | (1 << 2)));
					fqt_iter = searched.find(temp);
					rqt_iter = rev_searched.find(temp);
					return true;
				}
				if (judge_feasible(temp)) {
					searched.insert(pair<STATE, short>(temp, oprt | (1 << 2)));
					srh_fwd.push(temp);
				}
				temp = current;
			}
			if (temp.box[i] & (1 << 1)) {
				//可以将当前箱子向左推
				temp.box[i] >>= 4;
				temp.box[i] -= 1;
				temp.box[i] <<= 4;
				Calc_Map.update_box(temp.box, bx, by);
				if (BFS_judge_finished(temp, true)) {
					searched.insert(pair<STATE, short>(temp, oprt | (1 << 1)));
					fqt_iter = searched.find(temp);
					rqt_iter = rev_searched.find(temp);
					return true;
				}
				if (judge_feasible(temp)) {
					searched.insert(pair<STATE, short>(temp, oprt | (1 << 1)));
					srh_fwd.push(temp);
				}
				temp = current;
			}
			if (temp.box[i] & 1) {
				//可以将当前箱子向右推
				temp.box[i] >>= 4;
				temp.box[i] += 1;
				temp.box[i] <<= 4;
				Calc_Map.update_box(temp.box, bx, by);
				if (BFS_judge_finished(temp, true)) {
					searched.insert(pair<STATE, short>(temp, oprt | 1));
					fqt_iter = searched.find(temp);
					rqt_iter = rev_searched.find(temp);
					return true;
				}
				if (judge_feasible(temp)) {
					searched.insert(pair<STATE, short>(temp, oprt | 1));
					srh_fwd.push(temp);
				}
				temp = current;
			}
		}
		//以下是反向搜索
		if (srh_rvs.empty()) return false;
		temp_state = srh_rvs.front();
		srh_rvs.pop();
		//反向枚举

		if (BFS_Rvs(temp_state)) return true;
	}
	return false;
}
void BOX_CALCULATOR::get_solution(const STATE& current) {
	map<STATE, short>::iterator iter;
	iter = searched.find(current);
	if (iter == searched.end()) { 
		cout << "Error!\n";
		return;
	}
	if (!iter->second) return;
	STATE temp = current;
	//倒推出上一步
	short bx, by;
	for (int i = 0; i < Calc_Map.Goals_num; i++) {
		bx = (temp.box[i] >> 4) / (MAX_LEN - 2) + 1, by = (temp.box[i] >> 4) % (MAX_LEN - 2) + 1;
		temp.box[i] >>= 4;
		if (iter->second & (1 << 3)) {
			//向上推过来
			temp.box[i] += MAX_LEN - 2;
			if ((iter->second) >> 4 == temp.box[i]) {
				bx += 2;
				temp.box[i] <<= 4;
				break;
			}
			temp.box[i] -= MAX_LEN - 2;
		}
		else if (iter->second & (1 << 2)) {
			//向下推过来
			temp.box[i] -= MAX_LEN - 2; 
			if ((iter->second) >> 4 == temp.box[i]) {
				bx -= 2;
				temp.box[i] <<= 4;
				break;
			}
			temp.box[i] += MAX_LEN - 2;
		}
		else if (iter->second & (1 << 1)) {
			//向左推过来
			temp.box[i] += 1;
			if ((iter->second) >> 4 == temp.box[i]) {
				by += 2;
				temp.box[i] <<= 4;
				break;
			}
			temp.box[i] -= 1;
		}
		else {
			//向右推过来
			temp.box[i] -= 1;
			if ((iter->second) >> 4 == temp.box[i]) {
				by -= 2;
				temp.box[i] <<= 4;
				break;
			}
			temp.box[i] += 1;
		}
		temp.box[i] <<= 4;
	}
	Calc_Map.update_box(temp.box, bx, by);
	//递归导出解
	get_solution(temp);
	solution[cnt++] = iter->second;
}

void BOX_CALCULATOR::get_rev_solution(const STATE & current){
	map<STATE, short>::iterator iter;
	iter = rev_searched.find(current);
	if (iter == rev_searched.end()){
		cout << "Error!\n";
		return;
	}
	if (!iter->second) return;
	STATE temp = current;
	short bx, by,oprt;
	for (int i = 0; i < Calc_Map.Goals_num; i++) {
		bx = (temp.box[i] >> 4) / (MAX_LEN - 2) + 1, by = (temp.box[i] >> 4) % (MAX_LEN - 2) + 1;
		temp.box[i] >>= 4;
		oprt = temp.box[i]<<4;
		if (iter->second&(1 << 3)) {
			//第n+1到第n步是向下拉过来的，坐标存的是n+1时
			temp.box[i] -= MAX_LEN - 2;
			if ((iter->second) >> 4 == temp.box[i]) {
				//玩家坐标就是箱子坐标
				oprt |= 1 << 3;
				temp.box[i] <<= 4;
				break;
			}
			temp.box[i] += MAX_LEN - 2;
		}else if (iter->second & (1 << 2)) {
			temp.box[i] += MAX_LEN - 2;
			if ((iter->second) >> 4 == temp.box[i]) {
				oprt |= 1 << 2;
				temp.box[i] <<= 4;
				break;
			}
			temp.box[i] -= MAX_LEN - 2;
		}
		else if (iter->second & (1 << 1)) {
			//向右拉过来
			temp.box[i] -= 1;
			if ((iter->second) >> 4 == temp.box[i]) {
				oprt |= 1 << 1;
				temp.box[i] <<= 4;
				break;
			}
			temp.box[i] += 1;
		}
		else {
			//向左拉过来
			temp.box[i] += 1;
			if ((iter->second) >> 4 == temp.box[i]) {
				oprt |= 1;
				temp.box[i] <<= 4;
				break;
			}
			temp.box[i] -= 1;
		}
		temp.box[i] <<= 4;
	}
	Calc_Map.update_box(temp.box, bx, by);
	solution[cnt++] = oprt;
	get_rev_solution(temp);
}

void BOX_CALCULATOR::do_Compute() {
	STATE init_status;
	memset(init_status.box, 0, sizeof(init_status.box));
	int index = 0;
	for (short i = Calc_Map.Map_uper; i < Calc_Map.Map_uper + Calc_Map.Map_length; i++)
		for (short j = Calc_Map.Map_left; j < Calc_Map.Map_left + Calc_Map.Map_width; j++)
			if (Calc_Map.Game_box[i][j]) {
				//箱子值：((MAX_LEN - 2)*(x-1)+(y-1))<<4
				init_status.box[index++] = ((MAX_LEN - 2) * (i - 1) + (j - 1)) << 4;
				//比较时记得排序
			}
	if (judge_finished(init_status)) {
		trivial = true;
		return;
	}
	st = clock();
	Calc_Map.update_box(init_status.box, Calc_Map.player_x, Calc_Map.player_y);//更新可推动的箱子方向数据

	//cout << "Calculating...(Press any key to terminate)" << endl;
	if (search_mode == 1) {
		if (!search_way(init_status)) judge_no_solution = true;
	}else if(search_mode == 2){
		for (depth_limit = 2; depth_limit <= 100; depth_limit++) {
			judge_no_solution = true;
			oss_box.str("");
			oss_box << "Searching with max depth " << depth_limit - 2;
			Fl::lock();
			Fl::awake(update_msg, new bool (false));
			Fl::unlock();
			if (search_way(init_status)) break;
			if (judge_no_solution) {
				break;
			}
		}
	}else if (search_mode == 3) {

		if (BFS_Pure(init_status)) {
			get_solution(fqt_iter->first);
		}
		else
			judge_no_solution = true;

	}
	else if (search_mode == 4) {
		if (BFS_Fwd(init_status)) {
			get_solution(fqt_iter->first);
			get_rev_solution(rqt_iter->first);
		}
		else judge_no_solution = true;
		rev_searched.clear();
		while (!srh_rvs.empty())
			srh_rvs.pop();
	}
	ed = clock();
	total_time = (double)(ed - st) / CLOCKS_PER_SEC;
	searched.clear();
	while (!srh_fwd.empty())
		srh_fwd.pop();
	return;
}
