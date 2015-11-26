#include "MyStrategy.h"

#define PI 3.14159265358979323846
#define _USE_MATH_DEFINES
#define fabs f_abs
#include <cmath>
#include <cstdlib>

using namespace model;
using namespace std;

struct Tree
{
	//	int FatherR;  // строка
	int FatherC;  // столбец
	int X_tile;   // здесь € записываю X  в массиве , где € нахожу кратчайшиий путь 
	int Y_tile;   // здесь € записываю Y в массиве , где € нахожу кратчайшиий путь
	TileType tile_t;/////////////////////// Ќужно добавить проверку на тип
};
double f_abs(double a)
{
	if (a < 0) return -a;
	else return a;

}
int Convert(double num, const Game& game){
	int ans = num / game.getTrackTileSize();
	return ans;
}
bool povorot(int X, int Y, double* axis, const Game& game, const World& world)
{
	//ћожно подумать, что € тер€ю небольшой участок тайла при таком решении. Ќо если машина остановитс€ в этом маленьком куске, то ей придетс€ огибать затем и окружность(котора€ у поворота), поэтому нет смысла тормозить, если € въедув этот кусочек
	int TileX = Convert(X, game);
	int TileY = Convert(Y, game);
	if (Convert(X + game.getTrackTileMargin() + 105, game) != TileX) return false;
	if (Convert(X - game.getTrackTileMargin() - 105, game) != TileX) return false;
	if (Convert(Y + game.getTrackTileMargin() + 105, game) != TileY) return false;
	if (Convert(Y - game.getTrackTileMargin() - 105, game) != TileY) return false;
	if ((world.getTilesXY()[TileX][TileY] == HORIZONTAL) ||
		(world.getTilesXY()[TileX][TileY] == VERTICAL)) return false;

	return true;

}
bool createVectorAndPovorot(Move& move,int n, const Car& self, double* axis, const World& world, int *ansmas_X, int *ansmas_Y, int * putb, const Game& game)
{
	double sX, sY;
	sX = (self.getMass()*self.getSpeedX() * self.getSpeedX()) / (2 * 1000 * game.getCarCrosswiseMovementFrictionFactor());//смотрю рассто€ние по ’, которое проеду до полной остановки на тормозах
	sY = (self.getMass()*self.getSpeedY() * self.getSpeedY()) / (2 * 1000 * game.getCarCrosswiseMovementFrictionFactor());//и также по Y
	if (self.getSpeedX() < 0)  sX *= -1; //≈сли скорость противоположна направлению вектора оси(машина едет вверх либо влево), то пройденное рассто€ние надо отнимать
	if (self.getSpeedY() < 0)  sY *= -1;
	axis[0] = 0;
	axis[1] = 0;
	if (povorot(self.getX() + sX, self.getY() + sY, axis, game,world) == true)//если поворот возможен, то € создаю вектор дл€ поворота.
	{ 
			for (int i = 1; i < n; ++i)///!!!!!!!!!!!!!!!!!!!!!!!!!!!!!1
			{
				if (ansmas_X[i] == Convert(self.getX() + sX, game))
					if (ansmas_Y[i] == Convert(self.getY() + sY, game))
					{
						if (ansmas_X[i + 1] == Convert(self.getX() + sX, game))
							if (self.getSpeedX() > 0) axis[1] = 1;
	     						else axis[1] = -1;//—мотрю, следующий тайл нашего пути идет вверх или вниз.
						if (ansmas_Y[i + 1] == Convert(self.getY() + sY, game))
						    	if (self.getSpeedY() > 0) axis[0] = 1;
							     else axis[0] = -1;
								break;
					}
		

           	}
		return true;


	}
	//≈сли поворот не нужен(или невозможен) надо сделать вектор, которого он держалс€ до вызова этой функции. 

	else
	{
		
		if (world.getTilesXY()[Convert(self.getX(), game)][Convert(self.getY(), game)] == VERTICAL)
			if (self.getSpeedY() <= 0) axis[1] = -1;
			  else axis[1] = 1;
		if (world.getTilesXY()[Convert(self.getX(), game)][Convert(self.getY(), game)] == HORIZONTAL)
				if (self.getSpeedX() <= 0) axis[0] = -1;
				else axis[0] = 1;
		return false;
	}
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
	//////// «десь нужно добавить кусок , где будут записыватьс€ ветки относительно доступных направлений



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
		main_mas[next] = new Tree[count_now * 3 + 1];    // ¬озможно стоит переместить
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

	for (int i = 0; i <= now; i++)
		delete[] main_mas[i];
	delete[] main_mas;

	
	*main_size = now + 1;
}

double ConvertAngle(double phi)
{
	if (phi >= PI / 4) return 1;
	if (phi <= (-1)*PI / 4) return -1;
	return (phi * 4 / PI);

}
void MyStrategy::move(const Car& self, const World& world, const Game& game, Move& move) 
{
	model::TileType tileType = world.getTilesXY()[0][0];
	const int n= 29;

	if (world.getTick() > 300)
	{

		int best_way[3][n] = { 0 };//ѕосле прохождени€ полного круга нужно обнулить третью строку
		int mas_size = 0;
		//if (world.getTick() < game.getInitialFreezeDurationTicks())
		Findbestway(self, world, &mas_size, best_way[0], best_way[1], game);

		for (int i = 0; i < n; ++i)
			if (best_way[0][i] == Convert(self.getX(), game))
				if (best_way[1][i] == Convert(self.getY(), game))
				{
					best_way[2][i] = 1;
					break;
				}
		double *axis = new double[2];


		createVectorAndPovorot(move,n, self, axis, world, best_way[0], best_way[1], best_way[2], game);//создаем вектор оси.ќсталось только определить его направление(ли бо + либо - ).Ёто € сделал, но не сохранил.потом добавлю.Ёто считай что сделано 
		double x, y, i, j;
		int znak;
		i = axis[0];
		j = axis[1];
		x = self.getSpeedX();
		y = self.getSpeedY();
		double phi = 0.;//надо определить, как отклонитьс€. ≈сть така€ иде€, но  € не уверен.
		//double znam = sqrt(x*x + y*y)*sqrt(i*i + j*j);
		if (world.getTick() == 401)
		{

			int a =5;


		}
	//	if ((znam >0.00001)||(znam<-0.00001)) 
		//phi = acos((x*i + y*j) / znam);
		if (axis[0] != 0)
		{
			if (self.getSpeedY() < 0) znak = 1.0;
			else znak = -1.0;
			phi = self.getSpeedY() / self.getSpeedX();
			phi = fabs(phi);
		}
		if (axis[1] != 0)
		{
			if (self.getSpeedX() < 0) znak = 1.0;
			else znak = -1.0;
			phi = self.getSpeedX() / self.getSpeedY();
			phi = fabs(phi);
		}
		move.setWheelTurn(2*phi*znak);

			delete[]axis;
	
	
		    //move.setThrowProjectile(true);
		    //move.setSpillOil(true);
	 }

	
      //  move.setUseNitro(true);
	move.setEnginePower(1.0);
	
}




MyStrategy::MyStrategy() { }
