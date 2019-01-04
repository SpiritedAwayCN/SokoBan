#include "system.h"
#include<algorithm>
#include<iostream>

void GameMap::update_box(short *box_data, short player_x, short player_y) {
	memset(current_map, 0, sizeof(current_map));
	//重载地图
	for (short i = Map_uper - 1; i <= Map_uper + Map_length; i++)
		for (short j = Map_left - 1; j <= Map_left + Map_width; j++)
			current_map[i][j] = Game_Wall[i][j] ? 1 : 0;
	for (short i = 0; i < Goals_num; i++) {
		box_data[i] >>= 4;
		current_map[box_data[i] / (MAX_LEN - 2) + 1][box_data[i] % (MAX_LEN - 2) + 1] = i + 2;//地图数字=箱子id+2
		box_data[i] <<= 4;
	}	
	find_way(box_data, player_x, player_y);
	std::sort(box_data, box_data + Goals_num);//完成后排序
}

void GameMap::update_rev_box(short *box_data, short player_x, short player_y, bool clr = true){
	if(clr)
		memset(current_map, 0, sizeof(current_map));
	//重载地图
	for (short i = Map_uper - 1; i <= Map_uper + Map_length; i++)
		for (short j = Map_left - 1; j <= Map_left + Map_width; j++)
			if(current_map[i][j] != -1)
				current_map[i][j] = Game_Wall[i][j] ? 1 : 0;
	for (short i = 0; i < Goals_num; i++) {
		box_data[i] >>= 4;
		current_map[box_data[i] / (MAX_LEN - 2) + 1][box_data[i] % (MAX_LEN - 2) + 1] = i + 2;//地图数字=箱子id+2
		box_data[i] <<= 4;
	}
	find_rev(box_data, player_x, player_y);
	std::sort(box_data, box_data + Goals_num);//完成后排序
}

void GameMap::find_rev(short *box_data, short x, short y) {
	current_map[x][y] = -1;//标记已经走过

	if (current_map[x - 1][y] > 1 && current_map[x + 1][y] <= 0)
		box_data[current_map[x - 1][y] - 2] |= 1 << 3;//上一步up
	if (!current_map[x - 1][y])
		find_rev(box_data, x - 1, y);
	if (current_map[x + 1][y] > 1 && current_map[x - 1][y] <= 0)
		box_data[current_map[x + 1][y] - 2] |= 1 << 2;//down
	if (!current_map[x + 1][y])
		find_rev(box_data, x + 1, y);
	if (current_map[x][y - 1] > 1 && current_map[x][y + 1] <= 0)
		box_data[current_map[x][y - 1] - 2] |= 1 << 1;//left
	if (!current_map[x][y - 1])
		find_rev(box_data, x, y - 1);
	if (current_map[x][y + 1] > 1 && current_map[x][y - 1] <= 0)
		box_data[current_map[x][y + 1] - 2] |= 1;//right
	if (!current_map[x][y + 1])
		find_rev(box_data, x, y + 1);
}
void GameMap::find_way(short * box_data, short x, short y){
	current_map[x][y] = -1;//标记已经走过

	if (current_map[x - 1][y] > 1 && current_map[x - 2][y] <= 0)
		box_data[current_map[x - 1][y] - 2] |= 1 << 3;//up
	if (!current_map[x - 1][y])
		find_way(box_data, x - 1, y);
	if (current_map[x + 1][y] > 1 && current_map[x + 2][y] <= 0)
		box_data[current_map[x + 1][y] - 2] |= 1 << 2;//down
	if (!current_map[x + 1][y])
		find_way(box_data, x + 1, y);
	if (current_map[x][y - 1] > 1 && current_map[x][y - 2] <= 0)
		box_data[current_map[x][y - 1] - 2] |= 1 << 1;//left
	if (!current_map[x][y - 1])
		find_way(box_data, x, y - 1);
	if (current_map[x][y + 1] > 1 && current_map[x][y + 2] <= 0)
		box_data[current_map[x][y + 1] - 2] |= 1;//right
	if (!current_map[x][y + 1])
		find_way(box_data, x, y + 1);
}
void GameMap::do_translate()
{
	Map_uper = Map_left = MAX_LEN;
	short Map_bottom = 0, Map_rightside = 0;
	for (int i = 1; i <= Map_length; ++i)
		for (int j = 1; j <= Map_width; ++j) {
			if (!Game_Wall[i][j]) {
				if (i < Map_uper) Map_uper = i;
				if (i > Map_bottom) Map_bottom = i;
				if (j < Map_left) Map_left = j;
				if (j > Map_rightside) Map_rightside = j;
			}
		}
	Map_width = Map_rightside - Map_left + 1;
	Map_length = Map_bottom - Map_uper + 1;


	short delta_x, delta_y, Map_center_x, Map_center_y;
	Map_center_x = (Map_uper + Map_bottom + 1) / 2;
	Map_center_y = (Map_left + Map_rightside + 1) / 2;
	delta_x = Window_center_x - Map_center_x;
	delta_y = Window_center_y - Map_center_y;

	memset(Game_Mark, false, sizeof(Game_Mark));
	for (int i = Map_uper; i <= Map_bottom; ++i)
		for (int j = Map_left; j <= Map_rightside; ++j)
			Game_Mark[i + delta_x][j + delta_y] = Game_box[i][j];
	for (int i = 0; i < MAX_LEN; ++i)
		for (int j = 0; j < MAX_LEN; ++j)
			Game_box[i][j] = Game_Mark[i][j];

	memset(Game_Mark, false, sizeof(Game_Mark));
	for (int i = Map_uper; i <= Map_bottom; ++i)
		for (int j = Map_left; j <= Map_rightside; ++j)
			Game_Mark[i + delta_x][j + delta_y] = Game_Goal[i][j];
	for (int i = 0; i < MAX_LEN; ++i)
		for (int j = 0; j < MAX_LEN; ++j)
			Game_Goal[i][j] = Game_Mark[i][j];

	memset(Game_Mark, 1, sizeof(Game_Mark));
	for (int i = Map_uper; i <= Map_bottom; ++i)
		for (int j = Map_left; j <= Map_rightside; ++j)
			Game_Mark[i + delta_x][j + delta_y] = Game_Wall[i][j];

	for (int i = 0; i < MAX_LEN; ++i)
		for (int j = 0; j < MAX_LEN; ++j)
			Game_Wall[i][j] = Game_Mark[i][j];
	player_x += delta_x;
	player_y += delta_y;
	Map_uper += delta_x;
	Map_bottom += delta_x;
	Map_left += delta_y;
	Map_rightside += delta_y;
	memset(Game_Mark, 0, sizeof(Game_Mark));

	for (int i = Map_uper; i <= Map_bottom; ++i)
		for (int j = Map_left; j <= Map_rightside; ++j) {
			if (Game_Wall[i][j]) continue;
			for (int k = i - 1; k <= i + 1; ++k)
				for (int l = j - 1; l <= j + 1; ++l)
					if (Game_Wall[k][l])
						Game_Mark[k][l] = true;
		}
}
void GameMap::get(std::istream& ist)
{
	short line{ 1 };
	short column{ 0 };
	std::vector<short> column_in_line;
	char ch{ ' ' };

	while (!ist.eof()) {
		ch = ist.get();
		int n = int(ch);
		if (n == 13) {
			continue;
		}
		else {
			if (ist.eof()) {
				++line;
				break;
			}
			else {
				switch (ch) {
				case ' ':
					break;
				case nothing:
				{++column;
				Game_Wall[line][column] = false;
				break; }
				case player:
				{++column;
				++player_num;
				player_x = line;
				player_y = column;
				Game_Wall[line][column] = false;
				break; }
				case box:
				{++column;
				++boxes_num;
				Game_Wall[line][column] = false;
				Game_box[line][column] = true;    //这里以及后面各处都是默认数组从【1】【1】开始，有一个元素为零的值最后再全部赋为wall
				break; }
				case goal_box:
				{++column;
				++boxes_num;
				++Goals_num;
				Game_Wall[line][column] = false;
				Game_box[line][column] = true;
				Game_Goal[line][column] = true;
				break; }
				case player_goal:
				{++column;
				++player_num;
				player_x = line;
				player_y = column;
				++Goals_num;
				Game_Wall[line][column] = false;
				Game_Goal[line][column] = true;
				Game_Goal[line][column] = true;
				break; }
				case goal:
				{++column;
				++Goals_num;
				Game_Wall[line][column] = false;
				Game_Goal[line][column] = true;
				break; }
				case wall:
				{++column;
				Game_Wall[line][column] = true;
				break; }
				case '\n':
				{
					if (column != 0)
					{
						column_in_line.push_back(column);   //  column_in_line[0]=第一行的方块数；之后类同。
						column = 0;
						++line;
					}
					break;
				}
				default:
					throw std::runtime_error{ "You tricky child put some illegal characters in the *.bgdat file?!" };
					break;
				}
			}
		}
	}
	if (line > 1&&column_in_line.size()>0)
	{
		for (int i = 0; i < column_in_line.size() - 1; ++i)
		{
			if (column_in_line[i] != column_in_line[i + 1])
				throw std::runtime_error{ "You should have the same column in each line!" };
		}
	}
    if (player_num > 1) {
        throw std::runtime_error{ "More than one Player in a map!" };
    }
    if(column_in_line.size()>0)
	Map_width = column_in_line[0];
	Map_length = line - 1;
	if (Map_width > 13 || Map_length > 13) {
		throw std::runtime_error{ "The map is bigger than 13*13!" };
	}
}

void GameMap::init() {
	Goals_num = 0;
	boxes_num = 0;
	player_num = 0;
	Map_length = 0;
	Map_width = 0;
	player_x = 0;
	player_y = 0;
	memset(Game_Wall, 1, sizeof(Game_Wall));
	memset(Game_box, 0, sizeof(Game_box));
	memset(Game_Goal, 0, sizeof(Game_Goal));
}

void GameMap::judge() {
	
	if (player_num < 1)throw std::runtime_error{ "NO Player!" };
	if (boxes_num != Goals_num)throw std::runtime_error{ "Boxes don't match with goals." };
	if (boxes_num == 0)throw std::runtime_error{ "Trivial!" };
	return;
}
