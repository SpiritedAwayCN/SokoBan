#pragma once
#ifndef _SYSTEM_H
#define _SYSTEM_H

constexpr int MAX_LEN = 15;
constexpr int MAX_BOX = 10;
constexpr int Max_Depth = 801;
constexpr int Window_center_x = MAX_LEN >> 1;
constexpr int Window_center_y = MAX_LEN >> 1;

constexpr int Grid_bitmap_x = 40;
constexpr int Grid_bitmap_y = 40;
constexpr int Grid_bitmap_step = 30;

constexpr char nothing = '0';
constexpr char wall = '1';
constexpr char goal = '2';
constexpr char box = '3';
constexpr char goal_box = '4';
constexpr char player = '5';
constexpr char player_goal = '6';
constexpr auto FILE_PLAYER = "Res/player.png";
constexpr auto FILE_BOX = "Res/box.png";
constexpr auto FILE_GOAL = "Res/goal.png";
constexpr auto FILE_WALL = "Res/wall.png";
constexpr auto FILE_RED_BOX = "Res/red_box.png";
constexpr auto FILE_FLOOR = "Res/floor.png";
constexpr auto FILE_BLANK = "Res/blank.png";

#include "GUI.h"
#include "Graph.h"
#include "Window.h"
#include "FL/Fl_Text_Buffer.H"
#include "FL/Fl_Text_Display.H"
#include "FL/fl_ask.H"
#include "FL/Fl_Button.H"
#include "FL/Fl_File_Chooser.H"
#include <thread>
#include <map>
#include <vector>
#include <stack>
#include <sstream>

using Graph_lib::Point;
using std::string;

class GameMap
{
public:
	short Map_length, Map_width;
	short Goals_num, boxes_num;
	short player_x, player_y, player_num;
	short Map_uper, Map_left;
	bool Game_box[MAX_LEN][MAX_LEN];
	bool Game_Goal[MAX_LEN][MAX_LEN];
	bool Game_Wall[MAX_LEN][MAX_LEN];
	bool Game_Mark[MAX_LEN][MAX_LEN];
	string Puzzle_File;
	void update_box(short *box_data, short player_x, short player_y);
	void update_rev_box(short *box_data, short player_x, short player_y, bool clr);
	void do_translate();
	void judge();
	void init();
	void get(std::istream& ist);
	std::stack<short> remember_path;
	short current_map[MAX_LEN][MAX_LEN];
private:
	void find_way(short *box_data, short x, short y);
	void find_rev(short *box_data, short x, short y);
};

class Game_Window : public Graph_lib::Window {
public:
	Game_Window(Point xy, int w, int h, const string& title);
	~Game_Window();
	int handle(int event);
	bool creative_mode;
	void Get_Game_Map(const string&);
	void Update_MsgBox(const string&, int);
	void update_Calc_button(bool active);
	void update_Undo_button(bool active);
    void update_create_mode(bool active);
    void update_option_button(bool active);
    char option;
protected:
	Graph_lib::Image* img_list[MAX_LEN][MAX_LEN];
    Graph_lib::Image* img_create[8];
	Graph_lib::Button quit_button;
	Graph_lib::Button load_button;
	Graph_lib::Button creat_button;
    Fl_Button *black_button;
    Fl_Button *wall_button;
    Fl_Button *player_button;
    Fl_Button *boxgoal_button;
    Fl_Button *clear_button;
    Fl_Button *floor_button;
    Fl_Button *goal_button;
    Fl_Button *box_button;
	Graph_lib::Button Hide_Grid;
	Graph_lib::Text Solver_Sidebar;
	Graph_lib::Text Widgets_Sidebar;
	Graph_lib::Text* Text_Msg;
	Graph_lib::Text* Text_cordi[2 * MAX_LEN];
	Fl_Button *DFS_button;
	Fl_Button *IDDFS_button;
	Fl_Button *BFS_button;
	Fl_Button *BBFS_button;
	Fl_Button *Terminate_button;
	Fl_Button *undo_button;
	Fl_Button *retry_button;
private:
	
	static void cb_undo(void*, void* pw)
	{
		static_cast<Game_Window*>(pw)->do_undo(false);
	}
	void do_undo(bool);
    
	static void cb_retry(void*, void* pw)
	{
		static_cast<Game_Window*>(pw)->do_retry();
	}
	void do_retry();
	static void cb_quit(void*, void* pw)
	{
		static_cast<Game_Window*>(pw)->quit();
	}
	void quit() { hide(); }

	static void cb_play(void*, void* pw)
	{
		static_cast<Game_Window*>(pw)->do_load(false);
	}
	void do_load(bool auto_load);

	static void cb_test(void*, void* pw)
	{
		static_cast<Game_Window*>(pw)->do_create();
	}
	void do_create();

	static void cb_calc(void*, void* pw)
	{
		static_cast<Game_Window*>(pw)->do_calc();
	}
	void do_calc();

	static void cb_calc_ID(void*, void* pw)
	{
		static_cast<Game_Window*>(pw)->do_calc_id();
	}
	void do_calc_id();

	static void cb_HideGrid(void*, void* pw)
	{
		static_cast<Game_Window*>(pw)->do_HideGrid();
	}
	void do_HideGrid();
    //Only for Create
    static void cb_black(void*, void* pw)
    {
        static_cast<Game_Window*>(pw)->choose_black();
    }
    void choose_black();
    static void cb_floor(void*, void* pw)
    {
        static_cast<Game_Window*>(pw)->choose_floor();
    }
    void choose_floor();
    static void cb_box(void*,void*pw)
    {
        static_cast<Game_Window*>(pw)->choose_box();
    }
    void choose_box();
    static void cb_player(void*,void*pw)
    {
        static_cast<Game_Window*>(pw)->choose_player();
    }
    void choose_player();
    static void cb_goal(void*,void*pw)
    {
        static_cast<Game_Window*>(pw)->choose_goal();
    }
    void choose_goal();
    static void cb_clear(void*,void*pw)
    {
        static_cast<Game_Window*>(pw)->do_clear();
    }
    void do_clear();
    static void cb_box_goal(void*,void*pw)
    {
        static_cast<Game_Window*>(pw)->choose_box_goal();
    }
    void choose_box_goal();
    static void cb_wall(void*,void*pw)
    {
        static_cast<Game_Window*>(pw)->choose_wall();
    }
    void choose_wall();
    //
	static void cb_calc_BFS(void*, void* pw)
	{
		static_cast<Game_Window*>(pw)->do_calc_BFS();
	}
	void do_calc_BFS();

	static void cb_calc_BBFS(void*, void* pw)
	{
		static_cast<Game_Window*>(pw)->do_calc_BBFS();
	}
	void do_calc_BBFS();

	static void cb_calc_stop(void*, void* pw)
	{
		static_cast<Game_Window*>(pw)->do_calc_stop();
	}
	void do_calc_stop();
	bool next_step();
	void timeout()
	{
		if(next_step())
			Fl::repeat_timeout(step, cb_timeout, this);
	}
    void deletetiny(int);
	static void cb_timeout(void* pw) { static_cast<Game_Window*>(pw)->timeout(); }
	void replace_image(short, short, const std::string&);
	void do_click_event(int x, int y);
	void create_map();
	void isend();
    void create_sign();
	void analyse_string(string&);
	void My_BFS(short x, short y, short Goal_x, short Goal_y);
	void put_text_ontop();
	void refresh_map();
	std::stack<short> BFS_path;
	static constexpr double step{ 1. / 30.};
	GameMap Displaying_Map;
};

string Get_File_Name(const GameMap&, short , short );
int second_thread(GameMap, short);
int analyse_string(const string&);
void update_msg(void *p);

extern Fl_Double_Window *win2;
extern Fl_Text_Buffer *buffer;
extern Fl_Text_Display *display;
extern Game_Window *win;
extern bool computing , force_stop;
extern int depth_limit;
extern std::ostringstream oss, oss_box;
#endif
