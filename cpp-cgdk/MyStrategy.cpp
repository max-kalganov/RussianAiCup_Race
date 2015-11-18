#include "MyStrategy.h"

#define PI 3.14159265358979323846
#define _USE_MATH_DEFINES

#include <cmath>
#include <cstdlib>

using namespace model;
using namespace std;

struct Tile
{
	int *tileX= new int [100];
	int *tileY = new int[100];
	int count = 0;

}bestWay; //объявляю структуру,  в которой будет путь и кол-во тайло, которые нужно пройти.

//исключение возможности хождения по кругам. ДОБАВИТЬ УСЛОВИЕ,ЕСЛИ ВЕРНУЛСЯ В СВОЮ ЖЕ ТОЧКУ(self.getX,self.getY)
bool circle(Tile Tile, int X, int Y)
{
	for (int i = 0; i < Tile.count; ++i)
		if ((Tile.tileX[i] == X) && (Tile.tileY[i] == Y)) return true;
	return false;

}
 //определение наименьшего расстояния до ключевого тайла
Tile TileMinCount(Tile Tile1, Tile Tile2, Tile Tile3, Tile Tile4)
{
	if (Tile1.count < Tile2.count) Tile2 = Tile1;
	else Tile1 = Tile2;

		if (Tile1.count < Tile3.count) Tile3 = Tile1;
		else Tile1 = Tile3;

		if (Tile1.count < Tile4.count) return Tile1;
		else return Tile4;
}
// рекурсивная функция нахождения кратчайшего пути

Tile shortWay(const Car& self, const World& world, Tile Tile, int X, int Y)
{
	if ((world.getTilesXY()[X][Y] == EMPTY) ||  (circle(Tile, X, Y))) // Проверка на хождение по кругу и тупиковый тайл(в который нельзя въехать)
	{
		Tile.count = 100;
		return Tile;
	}

	if ((X ==self.getNextWaypointX())&& (Y== self.getNextWaypointY())) // Условие выхода из рекурсии
	{
		Tile.tileX[Tile.count] = X;
		Tile.tileY[Tile.count] = Y;
		return Tile;
	}
	   else
	        {
			Tile.count++;
			//Нужно написать для каждого вида тайла по отдельности.Вроде можно case, но выглядит как-то усрашающе:)
			return TileMinCount(shortWay(self, world, Tile, X + 1, Y), shortWay(self, world, Tile, X, Y + 1), shortWay(self, world, Tile, X, Y - 1), shortWay(self, world, Tile, X - 1, Y));

			}
}

void MyStrategy::move(const Car& self, const World& world, const Game& game, Move& move) 
{
	model::TileType tileType = world.getTilesXY()[0][0];
	
	int i = 0, j = 0;
	int count = 0;
	 bestWay.tileX[bestWay.count]=self.getX;  //заношу координаты машины в даный момент
	 bestWay.tileY[bestWay.count] = self.getY;//
	 bestWay.count++;

	 bestWay = shortWay(self, world, bestWay, self.getX, self.getY); //получаю структуру, где дан массив пути и кол-во тайлов, которые нужно пройти.



	//	while (world.getTilesXY()[i][j] != world.getTilesXY()[self.getNextWaypointX()][self.getNextWaypointY()])
	


	


	   

	//if (world.getTilesXY()[0][0] == LEFT_BOTTOM_CORNER)
    move.setEnginePower(1.0);
    move.setThrowProjectile(true);
    move.setSpillOil(true);


    if (world.getTick() > game.getInitialFreezeDurationTicks()) {
        move.setUseNitro(true);
    }
}

MyStrategy::MyStrategy() { }
