//
//  Create mode.cpp
//  box game$beta0.8.2
//
//  Created by 吕 on 2018/12/28.
//  Copyright © 2018 吕. All rights reserved.
//

#include <stdio.h>
#include "system.h"
#include <fstream>
#include <iostream>

void Game_Window::do_create(){
    
    
    if (!creative_mode) {
        Displaying_Map.Puzzle_File = "";
		
		/*
        Displaying_Map.init();
        for (int i = 0; i < MAX_LEN; i++)
            for (int j = 0; j < MAX_LEN; j++) {
                replace_image(j, i, FILE_BLANK);
            }
        */
        update_create_mode(true);
        
    }else {
        try{
            Displaying_Map.judge();
        }
        catch(std::exception&e){
            fl_alert(e.what());
            return;
        }
		refresh_map();
        update_create_mode(false);
		
    }
}
void Game_Window::update_option_button(bool active){
    if(active){
        black_button->show();
        wall_button->show();
        player_button->show();
        boxgoal_button->show();
        clear_button->show();
        floor_button->show();
        goal_button->show();
        box_button->show();
    }
    else{
        black_button->hide();
        wall_button->hide();
        player_button->hide();
        boxgoal_button->hide();
        clear_button->hide();
        floor_button->hide();
        goal_button->hide();
        box_button->hide();
    }
}


void Game_Window::create_sign(){
    img_create[0] = new Graph_lib::Image{ Point{x_max()-170,240},"Res/tinybox.png"};
    img_create[1] = new Graph_lib::Image{Point{x_max()-150,240},"Res/tinygoal.png"};
    img_create[2] = new Graph_lib::Image{Point{x_max()-130,240},"Res/tinyred_box.png"};
    img_create[3] = new Graph_lib::Image{Point{x_max()-110,240},"Res/tinyblank.png"};
    img_create[4] = new Graph_lib::Image{Point{x_max()-90,240},"Res/tinyplayer.png"};
    img_create[5] = new Graph_lib::Image{Point{x_max()-70,240},"Res/tinywall.png"};
    img_create[6] = new Graph_lib::Image{Point{x_max()-50,240},"Res/tinyfloor.png"};
    img_create[7] = new Graph_lib::Image{Point{x_max()-115,200},"Res/blank.png"};
    for(int n=0;n<8;++n)
    attach(*img_create[n]);
    redraw();
}

void Game_Window::do_clear(){
    Displaying_Map.init();
    for (int i = 0; i < MAX_LEN; i++)
        for (int j = 0; j < MAX_LEN; j++) {
            replace_image(j, i, FILE_BLANK);
        }
}

void Game_Window::choose_player(){
    option=player;
    detach(*img_create[7]);
    delete img_create[7];
    img_create[7]=new Graph_lib::Image{Point{x_max()-115,200},"Res/player.png"};
    attach(*img_create[7]);
    redraw();
}

void Game_Window::choose_box(){
    char box='3';
    option=box;
    detach(*img_create[7]);
    delete img_create[7];
    img_create[7]=new Graph_lib::Image{Point{x_max()-115,200},"Res/box.png"};
    attach(*img_create[7]);
    redraw();
}

void Game_Window::choose_goal(){
    option=goal;
    detach(*img_create[7]);
    delete img_create[7];
    img_create[7]=new Graph_lib::Image{Point{x_max()-115,200},"Res/goal.png"};
    attach(*img_create[7]);
    redraw();
}

void Game_Window::choose_wall(){
    option=wall;
    detach(*img_create[7]);
    delete img_create[7];
    img_create[7]=new Graph_lib::Image{Point{x_max()-115,200},"Res/wall.png"};
    attach(*img_create[7]);
    redraw();
}

void Game_Window::choose_black(){
    option='7';
    detach(*img_create[7]);
    delete img_create[7];
    img_create[7]=new Graph_lib::Image{Point{x_max()-115,200},"Res/blank.png"};
    attach(*img_create[7]);
    redraw();
}

void Game_Window::choose_floor(){
    option=nothing;
    detach(*img_create[7]);
    delete img_create[7];
    img_create[7]=new Graph_lib::Image{Point{x_max()-115,200},"Res/floor.png"};
    attach(*img_create[7]);
    redraw();
}

void Game_Window::choose_box_goal(){
    option=goal_box;
    detach(*img_create[7]);
    delete img_create[7];
    img_create[7]=new Graph_lib::Image{Point{x_max()-115,200},"Res/red_box.png"};
    attach(*img_create[7]);
    redraw();
}

void Game_Window::do_click_event(int x, int y){
    if(x<=460&&x>=10&&y<=460&&y>=10){
        int i((x-10)/30);
        int j((y-10)/30);
        if((i==0||i==14||j==0||j==14)&&option!='7'){
            fl_alert("You are not allowed to put things here.");
            return;
        }
		int px = Displaying_Map.player_x, py = Displaying_Map.player_y;
        switch (option) {
            case wall:
                //replace_image(j, i, FILE_WALL);
                Displaying_Map.Game_Wall[j][i]=true;
                if(Displaying_Map.Game_Goal[j][i]){
                    --Displaying_Map.Goals_num;
                    Displaying_Map.Game_Goal[j][i]=false;
                }
                if(Displaying_Map.Game_box[j][i]){
                    --Displaying_Map.boxes_num;
                    Displaying_Map.Game_box[j][i]=false;
                }
                if( Displaying_Map.player_x==j&&Displaying_Map.player_y==i){
                    player_button->activate();
                }
				//replace_image(j, i, Get_File_Name(Displaying_Map, j, i));
                replace_image(j, i, FILE_WALL);
                break;
            case player:
                //replace_image(j, i, FILE_PLAYER);
                Displaying_Map.Game_Wall[j][i]=false;
				/*
                if(Displaying_Map.Game_Goal[j][i]){
                    --Displaying_Map.Goals_num;
                    Displaying_Map.Game_Goal[j][i]=false;
                }
				*/
                if(Displaying_Map.Game_box[j][i]){
                    --Displaying_Map.boxes_num;
                    Displaying_Map.Game_box[j][i]=false;
                }
                Displaying_Map.player_x=j;
                Displaying_Map.player_y=i;
                Displaying_Map.player_num=1;
				replace_image(px, py, Get_File_Name(Displaying_Map, px, py));
                //player_button->deactivate();
                //playergoal_button->deactivate();
                //option=' ';
				replace_image(j, i, Get_File_Name(Displaying_Map, j, i));
                break;
            case '3':
                
                Displaying_Map.Game_Wall[j][i]=false;
                if( Displaying_Map.player_x==j&&Displaying_Map.player_y==i){
					fl_alert("You cannot place the box on player. Please move the player first!");
					return;
					/*
                    --Displaying_Map.player_num;
                    player_button->activate();
                    playergoal_button->activate();
					*/
                }
                if(Displaying_Map.Game_Goal[j][i]){
                    --Displaying_Map.Goals_num;
                    Displaying_Map.Game_Goal[j][i]=false;
                }
                if(Displaying_Map.Game_box[j][i])
                {--Displaying_Map.boxes_num;}
                ++Displaying_Map.boxes_num;
                Displaying_Map.Game_box[j][i]=true;
				replace_image(j, i, Get_File_Name(Displaying_Map, j, i));
                break;
            case goal:
                //replace_image(j, i, FILE_GOAL);
                Displaying_Map.Game_Wall[j][i]=false;
                if(Displaying_Map.Game_Goal[j][i]){
                    --Displaying_Map.Goals_num;
                }
                Displaying_Map.Game_Goal[j][i]=true;
                ++Displaying_Map.Goals_num;
                if(Displaying_Map.Game_box[j][i]){
                    --Displaying_Map.boxes_num;
                    Displaying_Map.Game_box[j][i]=false;
                }
				/*
                if( Displaying_Map.player_x==j&&Displaying_Map.player_y==i){
                    --Displaying_Map.player_num;
                    player_button->activate();
                    playergoal_button->activate();
                }
				*/
				replace_image(j, i, Get_File_Name(Displaying_Map, j, i));
                break;
            case player_goal:
                //replace_image(j, i, FILE_PLAYER);
                Displaying_Map.Game_Wall[j][i]=false;
                Displaying_Map.Game_Goal[j][i]=true;
                if(Displaying_Map.Game_box[j][i]){
                    --Displaying_Map.boxes_num;
                    Displaying_Map.Game_box[j][i]=false;
                }
                if(!Displaying_Map.Game_Goal[j][i])
                {++Displaying_Map.Goals_num;}
                if(Displaying_Map.player_x==j||Displaying_Map.player_y==i)
                {--Displaying_Map.player_num;}
                Displaying_Map.player_x=j;
                Displaying_Map.player_y=i;
                Displaying_Map.player_num = 1;
				replace_image(px, py, Get_File_Name(Displaying_Map, px, py));
				//player_button->deactivate();
				//playergoal_button->deactivate();
                //option=' ';
				replace_image(j, i, Get_File_Name(Displaying_Map, j, i));
                break;
            case '7':
                //replace_image(j, i, FILE_BLANK);
                Displaying_Map.Game_Wall[j][i]=true;
                if(Displaying_Map.Game_Goal[j][i]){
                    --Displaying_Map.Goals_num;
                    Displaying_Map.Game_Goal[j][i]=false;
                }
                if(Displaying_Map.Game_box[j][i]){
                    --Displaying_Map.boxes_num;
                    Displaying_Map.Game_box[j][i]=false;
                }
                if( Displaying_Map.player_x==j&&Displaying_Map.player_y==i){
                    --Displaying_Map.player_num;
                    player_button->activate();
                }
				replace_image(j, i, Get_File_Name(Displaying_Map, j, i));
                break;
            case nothing:
                //replace_image(j, i, FILE_FLOOR);
                Displaying_Map.Game_Wall[j][i]=false;
                if(Displaying_Map.Game_Goal[j][i]){
                    --Displaying_Map.Goals_num;
                    Displaying_Map.Game_Goal[j][i]=false;
                }
                if(Displaying_Map.Game_box[j][i]){
                    --Displaying_Map.boxes_num;
                    Displaying_Map.Game_box[j][i]=false;
                }
                if( Displaying_Map.player_x==j&&Displaying_Map.player_y==i){
                    player_button->activate();
                }
				replace_image(j, i, Get_File_Name(Displaying_Map, j, i));
                break;
            case goal_box:
                //replace_image(j, i, FILE_RED_BOX);
                if( Displaying_Map.player_x==j&&Displaying_Map.player_y==i){
                    --Displaying_Map.player_num;
                    player_button->activate();
                }
                Displaying_Map.Game_Wall[j][i]=false;
                if(!Displaying_Map.Game_box[j][i]){
                    ++Displaying_Map.boxes_num;
                    Displaying_Map.Game_box[j][i]=true;
                }
                if(!Displaying_Map.Game_Goal[j][i]){
                    ++Displaying_Map.Goals_num;
                    Displaying_Map.Game_Goal[j][i]=true;
                }
				replace_image(j, i, Get_File_Name(Displaying_Map, j, i));
                break;
            default:break;
        }
        std::cout << "x = " << x << ", y = " << y << "\n";
    }
}
