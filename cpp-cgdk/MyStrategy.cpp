#include "MyStrategy.h"

#define PI 3.14159265358979323846
#define _USE_MATH_DEFINES

#include <cmath>
#include <cstdlib>

using namespace model;
using namespace std;



struct Tree
{
	//	int FatherR;  // строка
	int FatherC;  // столбец
	int X_tile;   // здесь я записываю X  в массиве , где я нахожу кратчайшиий путь 
	int Y_tile;   // здесь я записываю Y в массиве , где я нахожу кратчайшиий путь
	TileType tile_t;/////////////////////// Нужно добавить проверку на тип
};

int Convert(double num, const Game& game){
	int ans = num / game.getTrackTileSize();
	return ans;
}

int Write_New_Points(Tree **main_mas, int count_next, int count_now, int next, int now, int i, const World& world, bool *flag , int x_finish,int y_finish) {
	
	bool Dir_up = true;
	bool Dir_down = true;
	bool Dir_left =true;
	bool Dir_right = true;
	int How_many_ans = 0;

	switch (main_mas[now][i].tile_t)
	{
	case VERTICAL: 
		if (now != 0){
			if (main_mas[now - 1][main_mas[now][i].FatherC].Y_tile > main_mas[now][i].Y_tile) Dir_down = false;
			else Dir_up = false;
			How_many_ans = 1;
		}
		else How_many_ans = 2;
		Dir_left = false;
		Dir_right = false;
		break;
	case HORIZONTAL:
		if (now != 0){
			if (main_mas[now - 1][main_mas[now][i].FatherC].X_tile > main_mas[now][i].X_tile) Dir_right = false;
			else Dir_left = false;
			How_many_ans = 1;
		}
		else How_many_ans = 2;
		Dir_up = false;
		Dir_down = false;
		break;
	case LEFT_TOP_CORNER:
		if (now != 0){
			if (main_mas[now - 1][main_mas[now][i].FatherC].X_tile > main_mas[now][i].X_tile) Dir_right = false;
			else Dir_down = false;
			How_many_ans = 1;
		}
		else How_many_ans = 2;
		Dir_up = false;
		Dir_left = false;
		break;
	case RIGHT_TOP_CORNER:
		if (now != 0){
			if (main_mas[now - 1][main_mas[now][i].FatherC].Y_tile > main_mas[now][i].Y_tile) Dir_down = false;
			else Dir_left = false;
			How_many_ans = 1;
		}
		else How_many_ans = 2;
		Dir_up = false;
		Dir_right = false;
		break;
	case LEFT_BOTTOM_CORNER:
		if (now != 0){
			if (main_mas[now - 1][main_mas[now][i].FatherC].X_tile > main_mas[now][i].X_tile) Dir_right = false;
			else Dir_up = false;
			How_many_ans = 1;
		}
		else How_many_ans = 2;
		Dir_down = false;
		Dir_left = false;
		break;
	case RIGHT_BOTTOM_CORNER:
		if (now != 0){
			if (main_mas[now - 1][main_mas[now][i].FatherC].Y_tile < main_mas[now][i].Y_tile) Dir_up = false;
			else Dir_left = false;
			How_many_ans = 1;
		} 
		else How_many_ans = 2;
		Dir_right = false;
		Dir_down = false;
		break;
	case LEFT_HEADED_T:
		if (now != 0){
			if (main_mas[now - 1][main_mas[now][i].FatherC].Y_tile > main_mas[now][i].Y_tile) Dir_down = false;
			else if (main_mas[now - 1][main_mas[now][i].FatherC].Y_tile < main_mas[now][i].Y_tile) Dir_up = false;
			else Dir_left = false;
			How_many_ans = 2;
		}
		else How_many_ans = 3;
		Dir_right = false;
		break;
	case RIGHT_HEADED_T:
		if (now != 0){
			if (main_mas[now - 1][main_mas[now][i].FatherC].Y_tile > main_mas[now][i].Y_tile) Dir_down = false;
			else if (main_mas[now - 1][main_mas[now][i].FatherC].Y_tile < main_mas[now][i].Y_tile) Dir_up = false;
			else Dir_right = false;
			How_many_ans = 2;
		}
		else How_many_ans = 3;
		Dir_left = false;
		break;
	case TOP_HEADED_T:
		if (now != 0){
			if (main_mas[now - 1][main_mas[now][i].FatherC].X_tile > main_mas[now][i].X_tile) Dir_right = false;
			else if (main_mas[now - 1][main_mas[now][i].FatherC].X_tile < main_mas[now][i].X_tile) Dir_left = false;
			else Dir_up = false;
			How_many_ans = 2;
		}
		else How_many_ans = 3;
		Dir_down = false;
		break;
	case BOTTOM_HEADED_T:
		if (now != 0){
			if (main_mas[now - 1][main_mas[now][i].FatherC].X_tile > main_mas[now][i].X_tile) Dir_right = false;
			else if (main_mas[now - 1][main_mas[now][i].FatherC].X_tile < main_mas[now][i].X_tile) Dir_left = false;
			else Dir_down = false;
			How_many_ans = 2;
		}
		else How_many_ans = 3;
		Dir_up = false;
		break;
	case CROSSROADS:
		if (now != 0){
			if (main_mas[now - 1][main_mas[now][i].FatherC].X_tile > main_mas[now][i].X_tile) Dir_right = false;
			else if (main_mas[now - 1][main_mas[now][i].FatherC].X_tile < main_mas[now][i].X_tile) Dir_left = false;
			else if (main_mas[now - 1][main_mas[now][i].FatherC].Y_tile > main_mas[now][i].Y_tile) Dir_down = false;
			else if (main_mas[now - 1][main_mas[now][i].FatherC].Y_tile < main_mas[now][i].Y_tile) Dir_up = false;
			How_many_ans = 3;
		}
		else How_many_ans = 4;
		break;
	}

	bool flag2 = false;
	for (int j = 0; j < How_many_ans; j++){
		main_mas[next][count_next].FatherC = i;
		if ((Dir_down == true)&&(flag2 == false)){
			main_mas[next][count_next].X_tile = main_mas[now][i].X_tile;
			main_mas[next][count_next].Y_tile = main_mas[now][i].Y_tile + 1;
			flag2 = true;
			Dir_down = false;
		}
		if ((Dir_up == true) && (flag2 == false)){
			main_mas[next][count_next].X_tile = main_mas[now][i].X_tile;
			main_mas[next][count_next].Y_tile = main_mas[now][i].Y_tile - 1;
			flag2 = true;
			Dir_up = false;
		}
		if ((Dir_left == true) && (flag2 == false)){
			main_mas[next][count_next].X_tile = main_mas[now][i].X_tile - 1;
			main_mas[next][count_next].Y_tile = main_mas[now][i].Y_tile;
			flag2 = true;
			Dir_left = false;
		}
		if ((Dir_right == true) && (flag2 == false)){
			main_mas[next][count_next].X_tile = main_mas[now][i].X_tile + 1;
			main_mas[next][count_next].Y_tile = main_mas[now][i].Y_tile;
			flag2 = true;
			Dir_right = false;
		}
		main_mas[next][count_next].tile_t = world.getTilesXY()[main_mas[next][count_next].X_tile][main_mas[next][count_next].Y_tile];
		flag2 = false;
		if ((main_mas[next][count_next].X_tile == x_finish) && (main_mas[next][count_next].Y_tile == y_finish)){
			*flag = true;
			How_many_ans = j + 1;
			break;
		}
		count_next++;
	}
	return How_many_ans;
	//////// Здесь нужно добавить кусок , где будут записываться ветки относительно доступных направлений



}



void Findbestway(const Car& self, const World& world, int *main_size, int *ansmas_X , int *ansmas_Y, const Game& game, int Current_Path) {
	int x_start, y_start;
	int x_finish, y_finish;
    x_start = world.getWaypoints()[Current_Path][0];
    y_start = world.getWaypoints()[Current_Path][1];

    if (Current_Path + 1 < world.getWaypoints().size()){
        x_finish = world.getWaypoints()[Current_Path + 1][0];
        y_finish = world.getWaypoints()[Current_Path + 1][1];
    }
    else
    {
        x_finish = world.getWaypoints()[0][0];
        y_finish = world.getWaypoints()[0][1];
    }

	Tree **main_mas = new Tree *[29];
	main_mas[0] = new Tree[1];
	main_mas[0][0].X_tile = x_start;
	main_mas[0][0].Y_tile = y_start;
	main_mas[0][0].tile_t = world.getTilesXY()[x_start][y_start];

	int now = 0, next = 1;
	int count_now = 1, count_next = 0, helper;
	bool flag = false;
	while (flag == false) {
		main_mas[next] = new Tree[count_now * 3 + 1];    // Возможно стоит переместить
		for (int i = 0; i < count_now; i++) {
			count_next += Write_New_Points(main_mas, count_next, count_now, next, now, i, world, &flag,x_finish,y_finish);  // не уверен , что сработает **mas !!!
			if (flag == true) break;
		}
		helper = count_next;
		count_next = 0;
		count_now = helper;
		now = next++;
	}


	int col = count_now - 1;
	ansmas_X[now] = main_mas[now][col].X_tile;
	ansmas_Y[now] = main_mas[now][col].Y_tile;
	int col2;
	for (int i = now - 1; i >= 0; i--) {
		col2 = main_mas[i + 1][col].FatherC;
		ansmas_X[i] = main_mas[i][col2].X_tile;
		ansmas_Y[i] = main_mas[i][col2].Y_tile;
		col = col2;
	}
    ansmas_X[now+1] = -1;
    ansmas_Y[now+1] = -1;
	for (int i = 0; i <= now; i++)
		delete[] main_mas[i];
	delete[] main_mas;

	
	*main_size = now + 1;
}



void MyStrategy::move(const Car& self, const World& world, const Game& game, Move& move) 
{
    static int best_way[25][2][29]; // best_way[кол-во путей] [в 0 - Х-ы, в 1 - У-и] [маршрут от какой-то точки до ключ. тайла] 
    static int Current_Tile = 0;
    static int Current_Path = 0;
    static bool FullMas = false;
    int mas_size = 0;
    if (world.getTick() == 13)
        int  a = 8;
    if (world.getTick() < world.getWaypoints().size()){
        if ((Current_Path + 1 < world.getWaypoints().size()) && (FullMas == false)){
            if ((Current_Path + 1) == world.getWaypoints().size()) FullMas = true;
            
            Findbestway(self, world, &mas_size, best_way[Current_Path][0], best_way[Current_Path][1], game, Current_Path);
            Current_Path++;
        }
        else {  
            Findbestway(self, world, &mas_size, best_way[Current_Path][0], best_way[Current_Path][1], game, Current_Path);
            best_way[Current_Path+1][0][0] = -1;
            best_way[Current_Path+1][1][0] = -1;
            Current_Path = 0;
        }
    }
    move.setEnginePower(1.0);
    //move.setThrowProjectile(true);
    //move.setSpillOil(true);



    if (world.getTick() > game.getInitialFreezeDurationTicks()) {
      //  move.setUseNitro(true);
        int y = 7;
    }
}

MyStrategy::MyStrategy() { }