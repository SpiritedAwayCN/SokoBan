#include "Calculator.h"
#include<iostream>

using namespace std;

int depth_limit = 300;

bool BOX_CALCULATOR::judge_finished(const STATE &status) {
	for (int i = 0; i < Calc_Map.Goals_num; i++)
		if (!Calc_Map.Game_Goal[(status.box[i] >> 4) / (MAX_LEN - 2) + 1][(status.box[i] >> 4) % (MAX_LEN - 2) + 1])
			return false;
	//���е����Ӷ���λ
	judge_no_solution = false;
	return true;
}
bool BOX_CALCULATOR::judge_feasible(const STATE &status) {
	//��֦
	//��״̬�ظ�����ֱ�ӷ���
	map<STATE, short>::iterator iter;
	iter = searched.find(status);
	if (search_mode != 2) {
		if (iter != searched.end()) return false;
	}else{
		if (iter != searched.end()) {
			if(iter->second >> 8 == depth_limit) return false; //�������Ѿ�������
			if ((iter->second & 127) < cnt) return false; //�����������������·��
			//�������״̬��Ӧ������������
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
                    return false;//��֦->�������ڽ���

		if (Calc_Map.Game_box[bx + 1][by]) {
			if (Calc_Map.Game_Wall[bx][by - 1])
				if (Calc_Map.Game_Wall[bx + 1][by - 1] || Calc_Map.Game_Wall[bx + 1][by + 1] || Calc_Map.Game_box[bx + 1][by - 1])
                    if(!Calc_Map.Game_Goal[bx][by]||!Calc_Map.Game_Goal[bx+1][by]||(Calc_Map.Game_box[bx + 1][by - 1] && !Calc_Map.Game_Goal[bx+1][by-1]))
                        return false;//��֦->x��������������ǽ��ǽ����
			if (Calc_Map.Game_Wall[bx][by + 1])
				if (Calc_Map.Game_Wall[bx + 1][by - 1] || Calc_Map.Game_Wall[bx + 1][by + 1] || Calc_Map.Game_box[bx + 1][by + 1])
                    if(!Calc_Map.Game_Goal[bx][by]||!Calc_Map.Game_Goal[bx+1][by]|| (Calc_Map.Game_box[bx + 1][by + 1] && !Calc_Map.Game_Goal[bx + 1][by + 1]))
                        return false;//��֦->x��������������ǽ��ǽ����
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
                    return false;//��֦->2*2����
        }
		if (Calc_Map.Game_box[bx][by + 1]) {
			if (Calc_Map.Game_Wall[bx - 1][by])
				if (Calc_Map.Game_Wall[bx - 1][by + 1] || Calc_Map.Game_Wall[bx + 1][by + 1])
					if (!Calc_Map.Game_Goal[bx][by] || !Calc_Map.Game_Goal[bx][by + 1])
                        return false;//��֦->y��������������ǽ��ǽ����
			if (Calc_Map.Game_Wall[bx + 1][by])
				if (Calc_Map.Game_Wall[bx - 1][by + 1] || Calc_Map.Game_Wall[bx + 1][by + 1] || Calc_Map.Game_box[bx + 1][by + 1])
					if (!Calc_Map.Game_Goal[bx][by] || !Calc_Map.Game_Goal[bx][by + 1] || (Calc_Map.Game_box[bx + 1][by + 1] && !Calc_Map.Game_Goal[bx + 1][by + 1]))
                        return false;//��֦->y��������������ǽ��ǽ����
			
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
			//���Խ���ǰ����������
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
			//���Խ���ǰ����������
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
			//���Խ���ǰ����������
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
			//���Խ���ǰ����������
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
		//��һ���ȵڶ���
		if (rev_searched.find(status) == rev_searched.end()) return false;
	}else {
		//�ڶ����ȵ�һ��
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
			//���԰����������
			temp.box[i] >>= 4;
			temp.box[i] += MAX_LEN - 2;
			temp.box[i] <<= 4;
			//������µ�ͼ����
			Calc_Map.update_box(temp.box, bx + 2, by);
			if (BFS_judge_finished(temp, false)) {
				rev_searched.insert(pair<STATE, short>(temp, oprt | (1 << 3)));
				fqt_iter = searched.find(temp);
				rqt_iter = rev_searched.find(temp);
				return true;
			}
			if (!BFS_judge_finished(temp, true)) {
				//���ش������һ���߷������д������һ���߷�
				rev_searched.insert(pair<STATE, short>(temp, oprt | (1 << 3)));
				Calc_Map.update_rev_box(temp.box, bx + 2, by, true);
				srh_rvs.push(temp);
			}
			temp = current;
		}
		if (temp.box[i] & (1 << 2)) {
			//���԰����������
			temp.box[i] >>= 4;
			temp.box[i] -= MAX_LEN - 2;
			temp.box[i] <<= 4;
			//������µ�ͼ����
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
			//���԰����������
			temp.box[i] >>= 4;
			temp.box[i] +=  1;
			temp.box[i] <<= 4;
			//������µ�ͼ����
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
			//���԰����������
			temp.box[i] >>= 4;
			temp.box[i] -= 1;
			temp.box[i] <<= 4;
			//������µ�ͼ����
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
				//���Խ���ǰ����������
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
				//���Խ���ǰ����������
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
				//���Խ���ǰ����������
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
				//���Խ���ǰ����������
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
		//�����Ƿ�������
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

	//���£���������״̬
	for (int i = Calc_Map.Map_uper; i < Calc_Map.Map_uper+Calc_Map.Map_length; i++)
		for (int j = Calc_Map.Map_left; j < Calc_Map.Map_left+Calc_Map.Map_width; j++) {
			if (Calc_Map.Game_Goal[i][j])
				last_state.box[index++] = ((MAX_LEN - 2) * (i - 1) + (j - 1)) << 4;
		}
	

	memset(Calc_Map.current_map, 0, sizeof(Calc_Map.current_map));
	//���£��������п��ܵ���һ��
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
				//���Խ���ǰ����������
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
				//���Խ���ǰ����������
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
				//���Խ���ǰ����������
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
				//���Խ���ǰ����������
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
		//�����Ƿ�������
		if (srh_rvs.empty()) return false;
		temp_state = srh_rvs.front();
		srh_rvs.pop();
		//����ö��

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
	//���Ƴ���һ��
	short bx, by;
	for (int i = 0; i < Calc_Map.Goals_num; i++) {
		bx = (temp.box[i] >> 4) / (MAX_LEN - 2) + 1, by = (temp.box[i] >> 4) % (MAX_LEN - 2) + 1;
		temp.box[i] >>= 4;
		if (iter->second & (1 << 3)) {
			//�����ƹ���
			temp.box[i] += MAX_LEN - 2;
			if ((iter->second) >> 4 == temp.box[i]) {
				bx += 2;
				temp.box[i] <<= 4;
				break;
			}
			temp.box[i] -= MAX_LEN - 2;
		}
		else if (iter->second & (1 << 2)) {
			//�����ƹ���
			temp.box[i] -= MAX_LEN - 2; 
			if ((iter->second) >> 4 == temp.box[i]) {
				bx -= 2;
				temp.box[i] <<= 4;
				break;
			}
			temp.box[i] += MAX_LEN - 2;
		}
		else if (iter->second & (1 << 1)) {
			//�����ƹ���
			temp.box[i] += 1;
			if ((iter->second) >> 4 == temp.box[i]) {
				by += 2;
				temp.box[i] <<= 4;
				break;
			}
			temp.box[i] -= 1;
		}
		else {
			//�����ƹ���
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
	//�ݹ鵼����
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
			//��n+1����n���������������ģ���������n+1ʱ
			temp.box[i] -= MAX_LEN - 2;
			if ((iter->second) >> 4 == temp.box[i]) {
				//������������������
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
			//����������
			temp.box[i] -= 1;
			if ((iter->second) >> 4 == temp.box[i]) {
				oprt |= 1 << 1;
				temp.box[i] <<= 4;
				break;
			}
			temp.box[i] += 1;
		}
		else {
			//����������
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
				//����ֵ��((MAX_LEN - 2)*(x-1)+(y-1))<<4
				init_status.box[index++] = ((MAX_LEN - 2) * (i - 1) + (j - 1)) << 4;
				//�Ƚ�ʱ�ǵ�����
			}
	if (judge_finished(init_status)) {
		trivial = true;
		return;
	}
	st = clock();
	Calc_Map.update_box(init_status.box, Calc_Map.player_x, Calc_Map.player_y);//���¿��ƶ������ӷ�������

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
