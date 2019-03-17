#include "system.h"
#include "Calculator.h"
#include <iostream>
using std::string;

Fl_Double_Window *win2 = NULL;
Fl_Text_Buffer *buffer = NULL;
Fl_Text_Display *display = NULL;
Game_Window *win = NULL;
bool computing = false, force_stop = false;
std::ostringstream oss, oss_box;

void analyse_number(std::ostringstream &ans, int current_step, int num) {
	short bx, by, dir{ 0 };
	for (short j = 1; j <= 4; j++) {
		if (current_step & 1) dir = j;
		current_step >>= 1;
	}
	bx = current_step / (MAX_LEN - 2) + 1;
	by = current_step % (MAX_LEN - 2) + 1;
	ans << "STEP" << num + 1 << ": push the box at (" << bx << ',' << by << ") ";
	if (dir == 4) ans << "up";
	else if (dir == 3) ans << "down";
	else if (dir == 2) ans << "left";
	else if (dir == 1) ans << "right";
	ans << std::endl;
	//return ans.str();
}

void Create_window(void *p) {
	win->Update_MsgBox("Solution successfully found!" , 3);
	win->update_Calc_button(true);
	win2->begin();
	buffer = new Fl_Text_Buffer;
	display = new Fl_Text_Display(10, 10, 400, 300);
	buffer->text("Solution founded in ");
	buffer->append(oss.str().c_str());
	display->buffer(buffer);
	win2->show();
}

int second_thread(GameMap Calc_map, short mode) {
	try {
		Calc_map.judge();
        if(Calc_map.Goals_num>MAX_BOX)
            throw std::runtime_error{"Too many boxes!"};
	}catch(std::runtime_error &e){
		oss_box.str("");
		oss_box << e.what();
		Fl::lock();
		Fl::awake(update_msg, new bool(true));
		Fl::unlock();
		return 1;
	}
	computing = true;
	BOX_CALCULATOR Calc_Task{Calc_map , mode};
	Calc_Task.do_Compute();
	oss.str("");
	if (force_stop) {
		force_stop = false;
		oss_box.str("");
		oss_box << "Calculation Terminated";
		//std::cout << "test";
		Fl::lock();
		Fl::awake(update_msg, new bool(true));
		Fl::unlock();
	}else if (Calc_Task.judge_no_solution) {
		oss_box.str("");
		oss_box << "No solution?!";
		Fl::lock();
		Fl::awake(update_msg, new bool(true));
		Fl::unlock();
	}else if (Calc_Task.trivial) {
		oss_box.str("");
		oss_box << "Trivial!";
		Fl::lock();
		Fl::awake(update_msg, new bool (true));
		Fl::unlock();
	}else {
		oss << Calc_Task.total_time << "s.\n";
		for (int i = 0; i < Calc_Task.cnt; i++) {
			analyse_number(oss, Calc_Task.solution[i], i);
		}
		Fl::lock();
		Fl::awake(Create_window);
		Fl::unlock();
	}
	computing = false;
	return 0;
}

void update_msg(void *p) {
	win->Update_MsgBox(oss_box.str() , 1);
	if(*(bool*)p)
		win->update_Calc_button(true);
	delete p;
}

string Get_File_Name(const GameMap& Map, short x, short y) {
	//根据地图数组的数据返回对应图像文件名
	if (x == Map.player_x&&y == Map.player_y) return FILE_PLAYER;
	if (Map.Game_Goal[x][y] && Map.Game_box[x][y]) return FILE_RED_BOX;
	if (Map.Game_Goal[x][y]) return FILE_GOAL;
	if (Map.Game_box[x][y]) return FILE_BOX;
	if (Map.Game_Wall[x][y] && Map.Game_Mark[x][y]) return FILE_WALL;
	if (Map.Game_Wall[x][y] && !Map.Game_Mark[x][y]) return FILE_BLANK;
	return FILE_FLOOR;
}
int main() {
	// 生成一个窗口Game_Window类似乎是调用构造函数就直接显示
	
	win = new Game_Window{ Point{100, 100}, 680, 480, "SokoBan V1.0" };
	// 最先执行的地方应该是win的构造函数

	win2 = new Fl_Double_Window{ 420,320, "Results" };

	return Graph_lib::gui_main();

}
