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

int Write_New_Points(Tree **main_mas, int count_next, int count_now, int next, int now, int i, const World& world, bool *flag) {
	
	bool Dir_up = true;
	bool Dir_down = true;
	bool Dir_left =true;
	bool Dir_right = true;


	switch (main_mas[now][i].tile_t)
	{
	case VERTICAL:
	case HORIZONTAL:
	case LEFT_TOP_CORNER:
	case RIGHT_TOP_CORNER:
	case LEFT_BOTTOM_CORNER:
	case RIGHT_BOTTOM_CORNER:
	case LEFT_HEADED_T:
	case RIGHT_HEADED_T:
	case TOP_HEADED_T:
	case BOTTOM_HEADED_T:
	case CROSSROADS:

	default:
		break;
	}
}



void Findbestway(const Car& self, const World& world, int *main_size, int *ansmas_X, int *ansmas_Y, const Game& game) {
	int x_start, y_start;
	int x_finish, y_finish;
	x_finish = self.getNextWaypointX();
	y_finish = self.getNextWaypointY();
	x_start = Convert(self.getX(),game);
	y_start = Convert(self.getY(),game);


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
		for (int i = 0; i <= count_now; i++) {
			count_next += Write_New_Points(main_mas, count_next, count_now, next, now, i, world, &flag);  // не уверен , что сработает **mas !!!
			if (flag == true) break;
		}
		helper = count_next;
		count_next = 0;
		count_now = helper;
		now = next++;
	}


	int col = count_now;
	ansmas_X[now] = main_mas[now][col].X_tile;
	ansmas_Y[now] = main_mas[now][col].Y_tile;

	for (int i = now - 1; i >= 0; i++) {
		col = main_mas[i + 1][col].FatherC;
		ansmas_X[i] = main_mas[i][col].X_tile;
		ansmas_Y[i] = main_mas[i][col].Y_tile;
	}

	for (int i = 0; i <= now; i++)
		delete[] main_mas[i];
	delete[] main_mas;

	
	*main_size = now + 1;
}



void MyStrategy::move(const Car& self, const World& world, const Game& game, Move& move) 
{
	model::TileType tileType = world.getTilesXY()[0][0];

	int best_way[2][29];
	int mas_size = 0;
	Findbestway(self, world, &mas_size, best_way[0], best_way[1], game);

    move.setEnginePower(1.0);
    //move.setThrowProjectile(true);
    //move.setSpillOil(true);


    if (world.getTick() > game.getInitialFreezeDurationTicks()) {
      //  move.setUseNitro(true);
    }
}

MyStrategy::MyStrategy() { }
