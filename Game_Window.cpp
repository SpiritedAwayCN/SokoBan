#include "system.h"
#include <fstream>
#include <iostream>
#include <queue>
int Game_Window::handle(int event) {
	{

		int x=0, y=0, button = 0,remember_step;
		int ret = Fl_Double_Window::handle(event);  // assume the buttons won't handle the keyboard events
		switch (event) {
		case FL_FOCUS:
		case FL_UNFOCUS:
			ret = 1;                // enables receiving keyboard events
			break;
		case FL_PUSH: // 这个是鼠标点击事件
		//case FL_DRAG: //这个是鼠标拖动事件，会在拖动时随时返回，我感觉用不到
			button = Fl::event_button();
			x = Fl::event_x();
			y = Fl::event_y();
			break;
		case FL_SHORTCUT:           // incase widget that isn't ours has focus
		case FL_KEYDOWN:            // keyboard key pushed
		//case FL_KEYUP:              // keyboard key released
		{
			if (!creative_mode)
			{
				if (!BFS_path.empty()) break;
				if (Fl::event_key() == 'w' || Fl::event_key() == 65362) {
					//按下w键或键盘的↑键
					if (!Displaying_Map.Game_Wall[Displaying_Map.player_x - 1][Displaying_Map.player_y])//上方不是墙
					{
						if (!Displaying_Map.Game_box[Displaying_Map.player_x - 1][Displaying_Map.player_y])//上方1格是平地
						{
							Displaying_Map.player_x -= 1;
							replace_image(Displaying_Map.player_x + 1, Displaying_Map.player_y, Get_File_Name(Displaying_Map, Displaying_Map.player_x + 1, Displaying_Map.player_y));
							replace_image(Displaying_Map.player_x, Displaying_Map.player_y, FILE_PLAYER);//新位置换为人
						}
						else if (!(Displaying_Map.Game_box[Displaying_Map.player_x - 2][Displaying_Map.player_y] || Displaying_Map.Game_Wall[Displaying_Map.player_x - 2][Displaying_Map.player_y]))//上方1格不是平地（即是箱子）且上方2格不是箱子或墙
						{
							Displaying_Map.player_x -= 1;
							replace_image(Displaying_Map.player_x + 1, Displaying_Map.player_y, Get_File_Name(Displaying_Map, Displaying_Map.player_x + 1, Displaying_Map.player_y));
							replace_image(Displaying_Map.player_x, Displaying_Map.player_y, FILE_PLAYER);//新位置换为人
							Displaying_Map.Game_box[Displaying_Map.player_x][Displaying_Map.player_y] = false;
							Displaying_Map.Game_box[Displaying_Map.player_x - 1][Displaying_Map.player_y] = true;//箱子向上推1格
							replace_image(Displaying_Map.player_x - 1, Displaying_Map.player_y, Get_File_Name(Displaying_Map, Displaying_Map.player_x - 1, Displaying_Map.player_y));
							Update_MsgBox("push the box at (" + std::to_string(Displaying_Map.player_x) + "," + std::to_string(Displaying_Map.player_y) + ") up", 1);
							remember_step = (((Displaying_Map.player_x - 1 - 1)*(MAX_LEN - 2) + (Displaying_Map.player_y - 1)) << 4) | (1 << 3);
							Displaying_Map.remember_path.push(remember_step);
							update_Undo_button(true);
						}
					}
				}
				else if (Fl::event_key() == 's' || Fl::event_key() == 65364) {
					//按下s键或键盘的↓键
					if (!Displaying_Map.Game_Wall[Displaying_Map.player_x + 1][Displaying_Map.player_y])//下方不是墙
					{
						if (!Displaying_Map.Game_box[Displaying_Map.player_x + 1][Displaying_Map.player_y])//下方1格是平地
						{
							Displaying_Map.player_x += 1;
							replace_image(Displaying_Map.player_x - 1, Displaying_Map.player_y, Get_File_Name(Displaying_Map, Displaying_Map.player_x - 1, Displaying_Map.player_y));
							replace_image(Displaying_Map.player_x, Displaying_Map.player_y, FILE_PLAYER);//新位置换为人
						}
						else if (!(Displaying_Map.Game_box[Displaying_Map.player_x + 2][Displaying_Map.player_y] || Displaying_Map.Game_Wall[Displaying_Map.player_x + 2][Displaying_Map.player_y]))//下方1格不是平地（即是箱子）且下方2格不是箱子或墙
						{
							Displaying_Map.player_x += 1;
							replace_image(Displaying_Map.player_x - 1, Displaying_Map.player_y, Get_File_Name(Displaying_Map, Displaying_Map.player_x - 1, Displaying_Map.player_y));
							replace_image(Displaying_Map.player_x, Displaying_Map.player_y, FILE_PLAYER);//新位置换为人
							Displaying_Map.Game_box[Displaying_Map.player_x][Displaying_Map.player_y] = false;
							Displaying_Map.Game_box[Displaying_Map.player_x + 1][Displaying_Map.player_y] = true;//箱子向下推1格
							replace_image(Displaying_Map.player_x + 1, Displaying_Map.player_y, Get_File_Name(Displaying_Map, Displaying_Map.player_x + 1, Displaying_Map.player_y));
							Update_MsgBox("push the box at (" + std::to_string(Displaying_Map.player_x) + "," + std::to_string(Displaying_Map.player_y) + ") down", 1);
							remember_step = (((Displaying_Map.player_x + 1 - 1)*(MAX_LEN - 2) + (Displaying_Map.player_y - 1)) << 4) | (1 << 2);
							Displaying_Map.remember_path.push(remember_step);
							update_Undo_button(true);
						}
					}
				}
				else if (Fl::event_key() == 'a' || Fl::event_key() == 65361) {
					//按下a键或键盘的←键
					if (!Displaying_Map.Game_Wall[Displaying_Map.player_x][Displaying_Map.player_y - 1])//左方不是墙
					{
						if (!Displaying_Map.Game_box[Displaying_Map.player_x][Displaying_Map.player_y - 1])//左方1格是平地
						{
							Displaying_Map.player_y -= 1;
							replace_image(Displaying_Map.player_x, Displaying_Map.player_y + 1, Get_File_Name(Displaying_Map, Displaying_Map.player_x, Displaying_Map.player_y + 1));
							replace_image(Displaying_Map.player_x, Displaying_Map.player_y, FILE_PLAYER);//新位置换为人
						}
						else if (!(Displaying_Map.Game_box[Displaying_Map.player_x][Displaying_Map.player_y - 2] || Displaying_Map.Game_Wall[Displaying_Map.player_x][Displaying_Map.player_y - 2]))//左方1格不是平地（即是箱子）且左方2格不是箱子或墙
						{
							Displaying_Map.player_y -= 1;
							replace_image(Displaying_Map.player_x, Displaying_Map.player_y + 1, Get_File_Name(Displaying_Map, Displaying_Map.player_x, Displaying_Map.player_y + 1));
							replace_image(Displaying_Map.player_x, Displaying_Map.player_y, FILE_PLAYER);//新位置换为人
							Displaying_Map.Game_box[Displaying_Map.player_x][Displaying_Map.player_y] = false;
							Displaying_Map.Game_box[Displaying_Map.player_x][Displaying_Map.player_y - 1] = true;//箱子向左推1格
							replace_image(Displaying_Map.player_x, Displaying_Map.player_y - 1, Get_File_Name(Displaying_Map, Displaying_Map.player_x, Displaying_Map.player_y - 1));
							Update_MsgBox("push the box at (" + std::to_string(Displaying_Map.player_x) + "," + std::to_string(Displaying_Map.player_y) + ") left", 1);
							remember_step = (((Displaying_Map.player_x - 1)*(MAX_LEN - 2) + (Displaying_Map.player_y - 1 - 1)) << 4) | (1 << 1);
							Displaying_Map.remember_path.push(remember_step);
							update_Undo_button(true);
						}
					}
				}
				else if (Fl::event_key() == 'd' || Fl::event_key() == 65363) {
					//按下d键或键盘的→键
					if (!Displaying_Map.Game_Wall[Displaying_Map.player_x][Displaying_Map.player_y + 1])//右方不是墙
					{
						if (!Displaying_Map.Game_box[Displaying_Map.player_x][Displaying_Map.player_y + 1])//右方1格是平地
						{
							Displaying_Map.player_y += 1;
							replace_image(Displaying_Map.player_x, Displaying_Map.player_y - 1, Get_File_Name(Displaying_Map, Displaying_Map.player_x, Displaying_Map.player_y - 1));
							replace_image(Displaying_Map.player_x, Displaying_Map.player_y, FILE_PLAYER);//新位置换为人
						}
						else if (!(Displaying_Map.Game_box[Displaying_Map.player_x][Displaying_Map.player_y + 2] || Displaying_Map.Game_Wall[Displaying_Map.player_x][Displaying_Map.player_y + 2]))//右方1格不是平地（即是箱子）且右方2格不是箱子或墙
						{
							Displaying_Map.player_y += 1;
							replace_image(Displaying_Map.player_x, Displaying_Map.player_y - 1, Get_File_Name(Displaying_Map, Displaying_Map.player_x, Displaying_Map.player_y - 1));
							replace_image(Displaying_Map.player_x, Displaying_Map.player_y, FILE_PLAYER);//新位置换为人
							Displaying_Map.Game_box[Displaying_Map.player_x][Displaying_Map.player_y] = false;
							Displaying_Map.Game_box[Displaying_Map.player_x][Displaying_Map.player_y + 1] = true;//箱子向右推1格
							replace_image(Displaying_Map.player_x, Displaying_Map.player_y + 1, Get_File_Name(Displaying_Map, Displaying_Map.player_x, Displaying_Map.player_y + 1));
							Update_MsgBox("push the box at (" + std::to_string(Displaying_Map.player_x) + "," + std::to_string(Displaying_Map.player_y) + ") right", 1);
							remember_step = (((Displaying_Map.player_x - 1)*(MAX_LEN - 2) + (Displaying_Map.player_y + 1 - 1)) << 4) | 1;
							Displaying_Map.remember_path.push(remember_step);
							update_Undo_button(true);
						}
					}
				}
				else if (Fl::event_key() == 'u')
					Game_Window::do_undo(false);

				else if (Fl::event_key() == 'r')
					Game_Window::do_retry();

				else if (Fl::event_key()) {
					//这一段可用来测试按键数值（如上下左右），将输出在后台
					//最终这段将被删除，并删除头文件<iostream>
					std::cout << Fl::event_key() << std::endl;
				}
				isend();
				ret = 1;
				break;
			}
		}
		}
		switch (button) {
		case 1: //左键
		{
			if(creative_mode)do_click_event(x, y);
			if (!creative_mode)
			{
				short click_position_x{ 0 }, click_position_y{ 0 };
				if (x <= 460 && y <= 460)
				{
					click_position_x = (y - 10) / 30;
					click_position_y = (x - 10) / 30;
					//std::cout << click_position_x << " " << click_position_y << " " << Displaying_Map.current_map[click_position_x][click_position_y] << "\n";
					if (Displaying_Map.Game_box[click_position_x][click_position_y])
					{
						if (Displaying_Map.player_x == click_position_x - 1 && Displaying_Map.player_y == click_position_y)
						{
							if (!(Displaying_Map.Game_box[click_position_x + 1][click_position_y] || Displaying_Map.Game_Wall[click_position_x + 1][click_position_y]))//down
							{
								Displaying_Map.player_x = click_position_x;
								Displaying_Map.Game_box[click_position_x][click_position_y] = false;
								Displaying_Map.Game_box[click_position_x + 1][click_position_y] = true;
								replace_image(Displaying_Map.player_x - 1, Displaying_Map.player_y, Get_File_Name(Displaying_Map, Displaying_Map.player_x - 1, Displaying_Map.player_y));
								replace_image(Displaying_Map.player_x + 1, Displaying_Map.player_y, Get_File_Name(Displaying_Map, Displaying_Map.player_x + 1, Displaying_Map.player_y));
								replace_image(Displaying_Map.player_x, Displaying_Map.player_y, Get_File_Name(Displaying_Map, Displaying_Map.player_x, Displaying_Map.player_y));
								Update_MsgBox("push the box at (" + std::to_string(Displaying_Map.player_x) + "," + std::to_string(Displaying_Map.player_y) + ") down", 1);
								remember_step = (((Displaying_Map.player_x + 1 - 1)*(MAX_LEN - 2) + (Displaying_Map.player_y - 1)) << 4) | 1 << 2;
								Displaying_Map.remember_path.push(remember_step);
								update_Undo_button(true);
							}
						}
						else if (Displaying_Map.player_x == click_position_x + 1 && Displaying_Map.player_y == click_position_y)
						{
							if (!(Displaying_Map.Game_box[click_position_x - 1][click_position_y] || Displaying_Map.Game_Wall[click_position_x - 1][click_position_y]))//up
							{
								Displaying_Map.player_x = click_position_x;
								Displaying_Map.Game_box[click_position_x][click_position_y] = false;
								Displaying_Map.Game_box[click_position_x - 1][click_position_y] = true;
								replace_image(Displaying_Map.player_x - 1, Displaying_Map.player_y, Get_File_Name(Displaying_Map, Displaying_Map.player_x - 1, Displaying_Map.player_y));
								replace_image(Displaying_Map.player_x + 1, Displaying_Map.player_y, Get_File_Name(Displaying_Map, Displaying_Map.player_x + 1, Displaying_Map.player_y));
								replace_image(Displaying_Map.player_x, Displaying_Map.player_y, Get_File_Name(Displaying_Map, Displaying_Map.player_x, Displaying_Map.player_y));
								Update_MsgBox("push the box at (" + std::to_string(Displaying_Map.player_x) + "," + std::to_string(Displaying_Map.player_y) + ") up", 1);
								remember_step = (((Displaying_Map.player_x - 1 - 1)*(MAX_LEN - 2) + (Displaying_Map.player_y - 1)) << 4) | 1 << 3;
								Displaying_Map.remember_path.push(remember_step);
								update_Undo_button(true);
							}
						}
						else if (Displaying_Map.player_x == click_position_x && Displaying_Map.player_y == click_position_y + 1)
						{
							if (!(Displaying_Map.Game_box[click_position_x][click_position_y - 1] || Displaying_Map.Game_Wall[click_position_x][click_position_y - 1]))//left
							{
								Displaying_Map.player_y = click_position_y;
								Displaying_Map.Game_box[click_position_x][click_position_y] = false;
								Displaying_Map.Game_box[click_position_x][click_position_y - 1] = true;
								replace_image(Displaying_Map.player_x, Displaying_Map.player_y - 1, Get_File_Name(Displaying_Map, Displaying_Map.player_x, Displaying_Map.player_y - 1));
								replace_image(Displaying_Map.player_x, Displaying_Map.player_y + 1, Get_File_Name(Displaying_Map, Displaying_Map.player_x, Displaying_Map.player_y + 1));
								replace_image(Displaying_Map.player_x, Displaying_Map.player_y, Get_File_Name(Displaying_Map, Displaying_Map.player_x, Displaying_Map.player_y));
								Update_MsgBox("push the box at (" + std::to_string(Displaying_Map.player_x) + "," + std::to_string(Displaying_Map.player_y) + ") left", 1);
								remember_step = (((Displaying_Map.player_x - 1)*(MAX_LEN - 2) + (Displaying_Map.player_y - 1 - 1)) << 4) | 1 << 1;
								Displaying_Map.remember_path.push(remember_step);
								update_Undo_button(true);
							}
						}
						else if (Displaying_Map.player_x == click_position_x && Displaying_Map.player_y == click_position_y - 1)
						{
							if (!(Displaying_Map.Game_box[click_position_x][click_position_y + 1] || Displaying_Map.Game_Wall[click_position_x][click_position_y + 1]))//right
							{
								Displaying_Map.player_y = click_position_y;
								Displaying_Map.Game_box[click_position_x][click_position_y] = false;
								Displaying_Map.Game_box[click_position_x][click_position_y + 1] = true;
								replace_image(Displaying_Map.player_x, Displaying_Map.player_y - 1, Get_File_Name(Displaying_Map, Displaying_Map.player_x, Displaying_Map.player_y - 1));
								replace_image(Displaying_Map.player_x, Displaying_Map.player_y + 1, Get_File_Name(Displaying_Map, Displaying_Map.player_x, Displaying_Map.player_y + 1));
								replace_image(Displaying_Map.player_x, Displaying_Map.player_y, Get_File_Name(Displaying_Map, Displaying_Map.player_x, Displaying_Map.player_y));
								Update_MsgBox("push the box at (" + std::to_string(Displaying_Map.player_x) + "," + std::to_string(Displaying_Map.player_y) + ") right", 1);
								remember_step = (((Displaying_Map.player_x - 1)*(MAX_LEN - 2) + (Displaying_Map.player_y + 1 - 1)) << 4) | 1;
								Displaying_Map.remember_path.push(remember_step);
								update_Undo_button(true);
							}
						}
						isend();
					}
					else if (!Displaying_Map.Game_Wall[click_position_x][click_position_y])
					{
						My_BFS(Displaying_Map.player_x, Displaying_Map.player_y, click_position_x, click_position_y);
					}
				}
			}
		}
		
		}
		return(ret);
	}
}

void Game_Window::replace_image(short x,short y,const std::string& FILE_NAME ) {
	if (x < 0 || y < 0) return;
	detach(*img_list[x][y]);
	
	delete img_list[x][y];
	
	img_list[x][y] = new Graph_lib::Image{ Point{Grid_bitmap_y + Grid_bitmap_step * (y - 1) ,Grid_bitmap_x + Grid_bitmap_step * (x - 1)},
				FILE_NAME };
	
	attach(*img_list[x][y]);
	
	put_text_ontop();
	redraw();
}
void Game_Window::analyse_string(std::string &File_Name) {
    int i = File_Name.length();
	const std::string temp = File_Name;
	int num = 0, index = 0, cnt = 0;
	for (i -= 1; i >= 0; i--) {
		if (File_Name[i] == '.' || isdigit(File_Name[i])) break;
	}
	if ((File_Name[i] != '.' && !isdigit(File_Name[i])) || (i == 0 && File_Name[i] == '.')) {
		//没有找到.或数字 或者最后一个.是第一个字符
		File_Name += '1';
		return;
	}
	if (File_Name[i] == '.') i--;
	index = i;//最后一个数字下标
	while (i >= 0 && isdigit(File_Name[i])) i--;
	i++;//最后一个数字串的 第一个数字字符下标
	for (int j = i; j <= index; j++) {
		cnt *= 10;
		cnt += File_Name[j] - '0';
	}
	cnt++;
	File_Name = "";
	for (int j = 0; j < i; j++) {
		//数字之前的不变
		File_Name += temp[j];
	}
	File_Name += std::to_string(cnt);
	for (index += 1; index < (int)temp.length(); index++) {
		File_Name += temp[index];
	}
	return;
}
void Game_Window::do_load(bool auto_load = false) {
	std::string File_Name{Displaying_Map.Puzzle_File};
   
	if (Displaying_Map.Puzzle_File.length() == 0) {
		auto_load = false;
		File_Name += "Data/";
	}
		
	if (!auto_load) {
		const char* name = fl_file_chooser("test", "*bgdat",File_Name.c_str() , 1);
		if (!name)return;
		File_Name = name;
	} else {
		analyse_string(File_Name);
	}
	
	try { Get_Game_Map(File_Name); }
	catch (std::exception&e) {
		fl_alert(e.what());
		return;
	}
	for (int i = 0; i < MAX_LEN; i++)
		for (int j = 0; j < MAX_LEN; j++) {
			replace_image(i, j, Get_File_Name(Displaying_Map, i, j));
		}
	try { Displaying_Map.judge(); }
	catch (std::exception&e) {
		fl_alert(e.what());
		return;
	}
    update_create_mode(false);
}

void Game_Window::Get_Game_Map(const string &FILE_NAME) {

	std::ifstream ist{ FILE_NAME };
	if (!ist)
		throw std::runtime_error{"Cannot find File: " + FILE_NAME};

	Displaying_Map.init();
    Displaying_Map.Puzzle_File = FILE_NAME;
    while (!Displaying_Map.remember_path.empty()) Displaying_Map.remember_path.pop();
	Displaying_Map.get(ist);
	Displaying_Map.do_translate();
	ist.close();

}

void Game_Window::Update_MsgBox(const string& Msg, int color_id = 1){
	if (Text_Msg) {
		detach(*Text_Msg);
		delete Text_Msg;
	}
	Text_Msg = new Graph_lib::Text{ Point {x_max() - 200 , y_max() - 20} , Msg };
	Text_Msg->set_color(Graph_lib::Color{ color_id });
	attach(*Text_Msg);

	redraw();
}
void Game_Window::update_Calc_button(bool active){
	if (active) {
		BBFS_button->activate();
		BFS_button->activate();
		DFS_button->activate();
		IDDFS_button->activate();
		Terminate_button->deactivate();
	}else {
		BBFS_button->deactivate();
		BFS_button->deactivate();
		DFS_button->deactivate();
		IDDFS_button->deactivate();
		Terminate_button->activate();
	}
}
void Game_Window::update_Undo_button(bool active){
	if (active) {
        undo_button->activate();
		retry_button->activate();
	}
	else {
		undo_button->deactivate();
		retry_button->deactivate();
	}
}
Game_Window::Game_Window(Point xy, int w, int h, const string& title)
	: Graph_lib::Window(xy, w, h, title)
	, quit_button{ Point { x_max() - 130, 20 }, 70, 20, "Quit", cb_quit }
	, load_button{ Point { x_max() - 130, 50 }, 70, 20, "Load", cb_play }
	, creat_button{ Point { x_max() - 130, 80 }, 70, 20, "Confirm", cb_test }
	, Solver_Sidebar{Point {x_max() - 180, 355}, "Solver:"}
	, Widgets_Sidebar{ Point {x_max() - 180, 195}, "Widgets:" }
	, Text_Msg{ NULL }
	, Hide_Grid{ Point { x_max() - 155, 320 }, 110, 20, "Hide Grid", cb_HideGrid }
	, creative_mode{ true }
    ,option{' '}
{
    //create some tiny
    //Graph_lib::Image tiblack{Graph_lib::Point{x_max()-20,200},"Res/tinybox.png"};
    //attach(tiblack);
    //
    Displaying_Map.init();
	attach(quit_button);
	attach(load_button);
	attach(creat_button);
	attach(Hide_Grid);
	attach(Widgets_Sidebar);
	create_map();
    create_sign();
	Solver_Sidebar.set_color(0);
	Widgets_Sidebar.set_color(0);
	//计算器三大件
	attach(Solver_Sidebar);
	Text_cordi[0] = new Graph_lib::Text{ Point{Grid_bitmap_y - Grid_bitmap_step + 3 , Grid_bitmap_x - Grid_bitmap_step + 13},
			"0" };
	Text_cordi[0]->set_color(1);
	Text_cordi[0]->set_font_size(14);
	attach(*Text_cordi[0]);
	for (int i = 1; i < MAX_LEN; i++) {
        Text_cordi[i] = new Graph_lib::Text{ Point{Grid_bitmap_y + (i - 1)*Grid_bitmap_step + 8 , Grid_bitmap_x - Grid_bitmap_step + 13},
			std::to_string(i) };
		Text_cordi[i]->set_color(1);
		Text_cordi[i]->set_font_size(14);
		
		attach(*Text_cordi[i]);
	}
	for (int i = MAX_LEN; i < 2*MAX_LEN-1; i++) {
		Text_cordi[i] = new Graph_lib::Text{ Point{Grid_bitmap_y - Grid_bitmap_step + 3 , Grid_bitmap_x + (i - MAX_LEN)*Grid_bitmap_step + 19},
			std::to_string(i-MAX_LEN + 1) };
		Text_cordi[i]->set_color(1);
		Text_cordi[i]->set_font_size(14);

		attach(*Text_cordi[i]);
	}
	//这个函数可以修改右下角信息内容，可在游戏过程中使用(内容，颜色编号)
	Update_MsgBox("Welcome to Sokoban Game!", 1);
	begin();
	undo_button = new Fl_Button{ x_max() - 180 ,105, 70, 20, "Undo(U)" };
	undo_button->callback(reinterpret_cast<Fl_Callback*>(cb_undo), this);
	retry_button = new Fl_Button{ x_max() - 100 ,105, 70, 20, "Retry(R)" };
	retry_button->callback(reinterpret_cast<Fl_Callback*>(cb_retry), this);
    box_button = new Fl_Button{x_max()-170,240,20,20,""};
    box_button ->callback(reinterpret_cast<Fl_Callback*>(cb_box), this);
    goal_button = new Fl_Button{x_max()-150,240,20,20,""};
    goal_button->callback(reinterpret_cast<Fl_Callback*>(cb_goal), this);
    boxgoal_button=new Fl_Button{x_max()-130,240,20,20,""};
    boxgoal_button->callback(reinterpret_cast<Fl_Callback*>(cb_box_goal), this);
    black_button =new Fl_Button{x_max()-110,240,20,20,""};
    black_button->callback(reinterpret_cast<Fl_Callback*>(cb_black), this);
    player_button =new Fl_Button{x_max()-90,240,20,20,""};
    player_button->callback(reinterpret_cast<Fl_Callback*>(cb_player), this);
    wall_button =new Fl_Button{x_max()-70,240,20,20,""};
    wall_button->callback(reinterpret_cast<Fl_Callback*>(cb_wall), this);
    clear_button =new Fl_Button{x_max()-170,260,140,20,"Clear"};
    clear_button->callback(reinterpret_cast<Fl_Callback*>(cb_clear), this);
    floor_button= new Fl_Button{x_max()-50,240,20,20,""};
    floor_button->callback(reinterpret_cast<Fl_Callback*>(cb_floor), this);
    DFS_button = new Fl_Button{ x_max() - 100, 360 , 70, 20, "DFS" };
	DFS_button->callback(reinterpret_cast<Fl_Callback*>(cb_calc), this);
	BFS_button = new Fl_Button{ x_max() - 100, 380 , 70, 20, "BFS" };
	BFS_button->callback(reinterpret_cast<Fl_Callback*>(cb_calc_BFS), this);
	BBFS_button = new Fl_Button{ x_max() - 180, 360 , 70, 20, "BBFS"};
	BBFS_button->callback(reinterpret_cast<Fl_Callback*>(cb_calc_BBFS), this);
	IDDFS_button = new Fl_Button{ x_max() - 180, 380, 70, 20, "IDDFS" };
	IDDFS_button->callback(reinterpret_cast<Fl_Callback*>(cb_calc_ID), this);
	Terminate_button = new Fl_Button{ x_max() - 155, 403 , 110, 20, "Stop Calculation" };
	Terminate_button->callback(reinterpret_cast<Fl_Callback*>(cb_calc_stop), this);
	end();
	update_Undo_button(false);
    update_option_button(true);
	Terminate_button->deactivate();
}
Game_Window::~Game_Window(){
	//Game_Window析构函数
	delete Text_Msg;
	delete[] &Text_cordi;
	delete undo_button,BFS_button,BBFS_button,DFS_button,IDDFS_button,Terminate_button,retry_button;
	for (int i = 0; i < MAX_LEN; i++) {
		for (int j = 0; j < MAX_LEN; j++)
			delete img_list[i][j];
		delete[] &img_list[i];
	}
 delete[] &img_list;
 }
void Game_Window::do_calc() {
	if (win2->shown() || computing) {
		fl_alert("Sorry, current computing task hasn't completed yet!");
		return;
	}
	const char* cont;
	cont = fl_input("Please set a depth limitation.", "300");
	if (cont) {	
		depth_limit = atoi(cont);
		if (depth_limit < 100 || depth_limit > 800) {
			fl_alert("The input must be an integer ranging from 100 to 800.");
			return;
		}
	}else {
		return;
	}
	refresh_map();
	Update_MsgBox("Computing...", 1);
	update_Calc_button(false);
	std::thread thd(second_thread, Displaying_Map, 1);
	thd.detach();
}

void Game_Window::do_calc_id() {
	if (win2->shown() || computing) {
		fl_alert("Sorry, current computing task hasn't completed yet!");
		return;
	}
	refresh_map();
	update_Calc_button(false);
	std::thread thd(second_thread, Displaying_Map, 2);
	thd.detach();
}
void Game_Window::do_HideGrid(){
	if (Hide_Grid.label == "Hide Grid") {
		Hide_Grid.set_label("Show Grid");
		for (int i = 0; i < 2 * MAX_LEN - 1; i++) {
			detach(*Text_cordi[i]);
		}
	}else {
		Hide_Grid.set_label("Hide Grid");
		for (int i = 0; i < 2 * MAX_LEN - 1; i++) {
			attach(*Text_cordi[i]);
			put_on_top(*Text_cordi[i]);
		}
	}
	redraw();
}
void Game_Window::do_calc_BFS() {
	if (win2->shown() || computing) {
		fl_alert("Sorry, current computing task hasn't completed yet!");
		return;
	}
	refresh_map();
	Update_MsgBox("Computing...", 1);
	update_Calc_button(false);
	std::thread thd(second_thread, Displaying_Map, 3);
	thd.detach();
}
void Game_Window::do_calc_BBFS() {
	if (win2->shown() || computing) {
		fl_alert("Sorry, current computing task hasn't completed yet!");
		return;
	}
	refresh_map();
	Update_MsgBox("Computing...", 1);
	update_Calc_button(false);
	std::thread thd(second_thread, Displaying_Map, 4);
	thd.detach();
}
void Game_Window::do_calc_stop() {
	if (!computing) {
		fl_alert("Nothing happened.");
		return;
	}
	switch (fl_ask("Are you sure to stop the calculation? The processes won't be saved."))
	{
	case 0: //no
		return;
	case 1: //yes
		if(computing) force_stop = true;
		update_Calc_button(true);
	}
}

bool Game_Window::next_step(){
	if (BFS_path.empty()) {
		Fl::remove_timeout(cb_timeout, this);
		return false;
	}
	short px = Displaying_Map.player_x, py = Displaying_Map.player_y;
	Displaying_Map.player_x = BFS_path.top() / (MAX_LEN - 2) + 1;
	Displaying_Map.player_y = BFS_path.top() % (MAX_LEN - 2) + 1;
	BFS_path.pop();
	replace_image(px, py, Get_File_Name(Displaying_Map, px, py));
	replace_image(Displaying_Map.player_x, Displaying_Map.player_y, FILE_PLAYER);
	return true;
}



void Game_Window::create_map(){
	for (int i = 0; i < MAX_LEN; i++)
		for (int j = 0; j < MAX_LEN; j++) {
			
			img_list[i][j] = new Graph_lib::Image{ Point{Grid_bitmap_y + Grid_bitmap_step * (j - 1) ,Grid_bitmap_x + Grid_bitmap_step * (i - 1)},
				FILE_BLANK };
			
			attach(*(img_list[i][j]));
		}
}
void Game_Window::isend()
{
	bool isend{ true };
	for (int i = 0; i < MAX_LEN; ++i)
	{
		for (int j = 0; j < MAX_LEN; ++j)
		{
			if (Displaying_Map.Game_box[i][j] != Displaying_Map.Game_Goal[i][j])
			{
				isend = false;
				return;
			}
		}
	}
	if (isend)
	{
		Update_MsgBox("Congratulations! You win!", 2);
		//注意这里进入create模式
        update_create_mode(true);
		switch (fl_ask("Congratulation! Continue to Play the next level?"))
		{
		case 0: //no
			return;
		case 1: //yes
			do_load(true);
		}
	}
}
void Game_Window::My_BFS(short x, short y, short Goal_x, short Goal_y)
{
	//这一段，清空栈，主要用于在移动过程中点击鼠标，将忽略上一次剩下的没走的
	while (!BFS_path.empty()) BFS_path.pop();
	Fl::remove_timeout(cb_timeout, this);
	memset(Displaying_Map.current_map, -1, sizeof(Displaying_Map.current_map));
	short start = (x - 1) * (MAX_LEN - 2) + y - 1;
	Displaying_Map.current_map[x][y] = -2;
	std::queue<short> BFS_way;
	BFS_way.push(start);
	while (!(BFS_way.empty()))
	{
		short qfront = BFS_way.front();
		BFS_way.pop();
		short bx = qfront / (MAX_LEN - 2) + 1, by = qfront % (MAX_LEN - 2) + 1;
		if (bx > Displaying_Map.Map_uper && !(Displaying_Map.Game_Wall[bx - 1][by] || Displaying_Map.Game_box[bx - 1][by]))
		{
			if (Displaying_Map.current_map[bx - 1][by] == -1)
			{
				BFS_way.push(qfront - (MAX_LEN - 2));
				Displaying_Map.current_map[bx - 1][by] = qfront;
				if (Goal_x == bx - 1 && Goal_y == by)
				{
					break;
				}
			}
		}
		if (bx < Displaying_Map.Map_uper+Displaying_Map.Map_length-1 && !(Displaying_Map.Game_Wall[bx + 1][by] || Displaying_Map.Game_box[bx + 1][by]))
		{
			if (Displaying_Map.current_map[bx + 1][by] == -1)
			{
				BFS_way.push(qfront + (MAX_LEN - 2));
				Displaying_Map.current_map[bx + 1][by] = qfront;
				if (Goal_x == bx + 1 && Goal_y == by) break;
			}
		}
		if (by > Displaying_Map.Map_left && !(Displaying_Map.Game_Wall[bx][by - 1] || Displaying_Map.Game_box[bx][by - 1]))
		{
			if (Displaying_Map.current_map[bx][by - 1] == -1)
			{
				BFS_way.push(qfront - 1);
				Displaying_Map.current_map[bx][by - 1] = qfront;
				if (Goal_x == bx && Goal_y == by - 1) break;
			}
		}
		if (by < Displaying_Map.Map_left+Displaying_Map.Map_width-1 && !(Displaying_Map.Game_Wall[bx][by + 1] || Displaying_Map.Game_box[bx][by + 1]))
		{
			if (Displaying_Map.current_map[bx][by + 1] == -1)
			{
				BFS_way.push(qfront + 1);
				Displaying_Map.current_map[bx][by + 1] = qfront;
				if (Goal_x == bx && Goal_y == by + 1) break;
			}
		}
	}
	if (!BFS_way.empty()) //输出
	{
		BFS_path.push((MAX_LEN - 2) * (Goal_x - 1) + Goal_y - 1);
		//BFS_path.push(Displaying_Map.current_map[Goal_x][Goal_y]);
		short cx = Goal_x, cy = Goal_y;
		while (!(cx == x && cy == y))
		{
			//想一想，为什么要新建一个temp
			int temp = Displaying_Map.current_map[cx][cy];
			BFS_path.push(temp);
			cx = temp / 13 + 1;
			cy = temp % 13 + 1;
		}
		Fl::add_timeout(step, cb_timeout, this);
	}
	memset(Displaying_Map.current_map, 0, sizeof(Displaying_Map.current_map));
	while (!BFS_way.empty()) BFS_way.pop();
}

void Game_Window::put_text_ontop(){
		for (int i = 0; i <2* MAX_LEN-1; i++) {
			put_on_top(*Text_cordi[i]);
	}
}
void Game_Window::refresh_map(){
	if (creative_mode == false) return;

	Displaying_Map.Map_length = Displaying_Map.Map_width = MAX_LEN - 2;
	Displaying_Map.do_translate();
	for (int i = 0; i < MAX_LEN; i++)
		for (int j = 0; j < MAX_LEN; j++) {
			replace_image(i, j, Get_File_Name(Displaying_Map, i, j));
		}
	return;
}
void Game_Window::do_undo(bool is_retry)
{
	if (Displaying_Map.remember_path.empty()) return;
	short t{ Displaying_Map.remember_path.top() };
	Displaying_Map.remember_path.pop();
	short bx = (t >> 4) / (MAX_LEN - 2) + 1;
	short by = (t >> 4) % (MAX_LEN - 2) + 1;
	short px = Displaying_Map.player_x;
	short py = Displaying_Map.player_y;

	if (t & (1 << 3))
	{
		Displaying_Map.player_x = bx + 2;
		Displaying_Map.player_y = by;
		Displaying_Map.Game_box[bx][by] = false;
		Displaying_Map.Game_box[bx + 1][by] = true;
		if (!is_retry) {
			replace_image(bx, by, Get_File_Name(Displaying_Map, bx, by));
			replace_image(bx + 1, by, Get_File_Name(Displaying_Map, bx + 1, by));
			replace_image(bx + 2, by, Get_File_Name(Displaying_Map, bx + 2, by));
		}
	}
	else if (t & (1 << 2))
	{
		Displaying_Map.player_x = bx - 2;
		Displaying_Map.player_y = by;
		Displaying_Map.Game_box[bx][by] = false;
		Displaying_Map.Game_box[bx - 1][by] = true;
		if (!is_retry) {
			replace_image(bx, by, Get_File_Name(Displaying_Map, bx, by));
			replace_image(bx - 1, by, Get_File_Name(Displaying_Map, bx - 1, by));
			replace_image(bx - 2, by, Get_File_Name(Displaying_Map, bx - 2, by));
		}
	}
	else if (t & (1 << 1))
	{
		Displaying_Map.player_x = bx;
		Displaying_Map.player_y = by + 2;
		Displaying_Map.Game_box[bx][by] = false;
		Displaying_Map.Game_box[bx][by + 1] = true;
		if (!is_retry) {
			replace_image(bx, by, Get_File_Name(Displaying_Map, bx, by));
			replace_image(bx, by + 1, Get_File_Name(Displaying_Map, bx, by + 1));
			replace_image(bx, by + 2, Get_File_Name(Displaying_Map, bx, by + 2));
		}
	}
	else if (t & 1)
	{
		Displaying_Map.player_x = bx;
		Displaying_Map.player_y = by - 2;
		Displaying_Map.Game_box[bx][by] = false;
		Displaying_Map.Game_box[bx][by - 1] = true;
		if (!is_retry) {
			replace_image(bx, by, Get_File_Name(Displaying_Map, bx, by));
			replace_image(bx, by - 1, Get_File_Name(Displaying_Map, bx, by - 1));
			replace_image(bx, by - 2, Get_File_Name(Displaying_Map, bx, by - 2));
		}
	}
	replace_image(px, py, Get_File_Name(Displaying_Map, px, py));
	if (!is_retry)
		Update_MsgBox("Undo successfully!", 15);
	if (Displaying_Map.remember_path.empty()) {
		undo_button->deactivate();
		retry_button->deactivate();
	}
}

void Game_Window::do_retry()
{
	if (Displaying_Map.remember_path.empty()) return;
	while (!Displaying_Map.remember_path.empty())
		Game_Window::do_undo(true);
	for (int i = Displaying_Map.Map_uper; i < Displaying_Map.Map_uper + Displaying_Map.Map_length; i++)
		for (int j = Displaying_Map.Map_left; j < Displaying_Map.Map_left + Displaying_Map.Map_width; j++) {
			replace_image(i, j, Get_File_Name(Displaying_Map, i, j));
		}
	Update_MsgBox("Recover successfully!", 3);
}

void Game_Window::update_create_mode(bool active)
{
    
    if(active)
    {
        creative_mode=true;
        update_Undo_button(false);
        while (!Displaying_Map.remember_path.empty()) Displaying_Map.remember_path.pop();
        creat_button.set_label("Confirm");
		update_option_button(true);
        create_sign();
    }
    if(!active)
    {   creative_mode=false;
        update_option_button(false);
        creat_button.set_label("Create");
		update_option_button(false);
        for(int n=0;n<8;++n)
        {
        detach(*img_create[n]);
        delete img_create[n];
        }
        redraw();
    }
}
