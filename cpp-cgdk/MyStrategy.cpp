#include "MyStrategy.h"

#define Sp_For_Prop 24.911710015002704
#define S_For_Prop 841.15578617973542;
#define PI 3.14159265358979323846
#define _USE_MATH_DEFINES
#include <cmath>
#include <cstdlib>

using namespace model;
using namespace std;

static int best_way[25][2][29]; // best_way[кол-во путей] [в 0 - Х-ы, в 1 - У-и] [маршрут от какой-то точки до ключ. тайла]

struct Tree
{
	//	int FatherR;  // строка
	int FatherC;  // столбец
	int X_tile;   // здесь я записываю X  в массиве , где я нахожу кратчайшиий путь 
	int Y_tile;   // здесь я записываю Y в массиве , где я нахожу кратчайшиий путь
	TileType tile_t;/////////////////////// Нужно добавить проверку на тип
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

bool povorot(int X, int Y, const Game& game, const World& world, int *Current_Tile, int *Current_Path)
{
	//Можно подумать, что я теряю небольшой участок тайла при таком решении. Но если машина остановится в этом маленьком куске, то ей придется огибать затем и окружность(которая у поворота), поэтому нет смысла тормозить, если я въедув этот кусочек
	int TileX = Convert(X, game);
	int TileY = Convert(Y, game);
	if (Convert(X + game.getTrackTileMargin() + 105, game) != TileX) return false;
	if (Convert(X - game.getTrackTileMargin() - 105, game) != TileX) return false;
	if (Convert(Y + game.getTrackTileMargin() + 105, game) != TileY) return false;
	if (Convert(Y - game.getTrackTileMargin() - 105, game) != TileY) return false;
	if ((TileX == best_way[*Current_Path][0][*Current_Tile]) && (TileY == best_way[*Current_Path][1][*Current_Tile])) return false;
	int max_num = world.getTilesXY().size();
	if ((TileX < 0) || (TileY < 0) || (TileX >= max_num ) || (TileY >= max_num))return false;
    if ((world.getTilesXY()[TileX][TileY] == HORIZONTAL) ||
		(world.getTilesXY()[TileX][TileY] == VERTICAL)) return false;
	return true;

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
	default:
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

double ConvertAngle(double phi)
{
	if (phi >= PI / 4) return 1;
	if (phi <= PI / -4) return -1;
	return (phi * 4 / PI);

}

bool Check_Cond_For_Turning(const Car& self, const World& world, int* Current_Tile, int* Current_Path, const Game& game){
    
    double S_X = self.getSpeedX()*self.getSpeedX() * S_For_Prop; S_X = S_X / (Sp_For_Prop * Sp_For_Prop);
    double S_Y = self.getSpeedY()*self.getSpeedY() * S_For_Prop; S_Y = S_Y / (Sp_For_Prop * Sp_For_Prop);
    if (self.getSpeedX() < 0)  S_X *= -1; //Если скорость противоположна направлению вектора оси(машина едет вверх либо влево), то пройденное расстояние надо отнимать
    if (self.getSpeedY() < 0)  S_Y *= -1;

    if (povorot(self.getX() + S_X, self.getY() + S_Y, game, world, Current_Tile,Current_Path)){
        return true;
    }
    return false;
}

int Create_Axis(int *axis, int* Current_Tile, int* Current_Path, const Car& self, const Game& game){
    double S_X = self.getSpeedX()*self.getSpeedX() * S_For_Prop; S_X = S_X / (Sp_For_Prop * Sp_For_Prop);
    double S_Y = self.getSpeedY()*self.getSpeedY() * S_For_Prop; S_Y = S_Y / (Sp_For_Prop * Sp_For_Prop);
    if (self.getSpeedX() < 0)  S_X *= -1; //Если скорость противоположна направлению вектора оси(машина едет вверх либо влево), то пройденное расстояние надо отнимать
    if (self.getSpeedY() < 0)  S_Y *= -1;
    int TileX = Convert(self.getX() + S_X, game);
    int TileY = Convert(self.getY() + S_Y, game);
    
    int Next_Tile = *Current_Tile + 1;
    int Next_Path = *Current_Path;
    if (best_way[*Current_Path][0][*Current_Tile + 1] == -1){
        Next_Tile = 1;
        Next_Path = *Current_Path + 1;
    }

    int Next_Tile2 = *Current_Tile + 2;
    int Next_Path2 = *Current_Path;
    if (best_way[*Current_Path][0][*Current_Tile + 2] == -1){
        if (Next_Tile == 1){
            if (best_way[*Current_Path + 1][0][*Current_Tile + 2] == -1){
                Next_Path = *Current_Path + 2;
                Next_Tile2 = 1;
            }
            else{
                Next_Tile2 = 2;
                Next_Path = *Current_Path + 1;
            }
        }
        else {
            Next_Tile2 = 1;
            Next_Path = *Current_Path + 1;
        }
    }


    if ((TileX == best_way[Next_Path][0][Next_Tile]) && (TileY == best_way[Next_Path][1][Next_Tile])){
        if (best_way[Next_Path][0][Next_Tile] == best_way[Next_Path2][0][Next_Tile2]){
            axis[0] = 0;
            if (best_way[Next_Path][1][Next_Tile] > best_way[Next_Path2][1][Next_Tile2])
                axis[1] = -1;
            else axis[1] = 1;
        }
        else{
            axis[1] = 0;
            if (best_way[Next_Path][0][Next_Tile] > best_way[Next_Path2][0][Next_Tile2])
                axis[0] = -1;
            else axis[0] = 1;
        }
    }
    else if ((TileX == best_way[Next_Path2][0][Next_Tile2]) && (TileY == best_way[Next_Path2][1][Next_Tile2])){
        int Next_Tile3 = Next_Tile2 + 1;
        int Next_Path3 = Next_Path2;
        if (best_way[Next_Path2][0][Next_Tile2 + 1] == -1){
            Next_Tile3 = 1;
            Next_Path3 = Next_Path2 + 1;
        }
        if (best_way[Next_Path2][0][Next_Tile2] == best_way[Next_Path3][0][Next_Tile3]){
            axis[0] = 0;
            if (best_way[Next_Path2][1][Next_Tile2] > best_way[Next_Path3][1][Next_Tile3])
                axis[1] = -1;
            else axis[1] = 1;
        }
        else{
            axis[1] = 0;
            if (best_way[Next_Path2][0][Next_Tile2] > best_way[Next_Path3][0][Next_Tile3])
                axis[0] = -1;
            else axis[0] = 1;
        }
    }
    else return 0;
    return 1;

}

void Create_Axis_in_my_pos(int *axis, int* Current_Tile, int* Current_Path){
    int Next_Tile = *Current_Tile + 1;
    int Next_Path = *Current_Path;
    if (best_way[*Current_Path][0][*Current_Tile + 1] == -1){
        Next_Tile = 1;
        Next_Path = *Current_Path + 1;
    }
    if (best_way[*Current_Path][0][*Current_Tile] == best_way[Next_Path][0][Next_Tile]){
        axis[0] = 0;
        if (best_way[*Current_Path][1][*Current_Tile] > best_way[Next_Path][1][Next_Tile])
            axis[1] = -1;
        else axis[1] = 1;
    }
    else{
        axis[1] = 0;
        if (best_way[*Current_Path][0][*Current_Tile] > best_way[Next_Path][0][Next_Tile])
            axis[0] = -1;
        else axis[0] = 1;
    }
        
}

void MyStrategy::move(const Car& self, const World& world, const Game& game, Move& move) 
{ 
    static int Current_Tile = 0;
    static int Current_Path = 0;
    static bool FullMas = false;
    int mas_size = 0;
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
	static double PastCoorX = self.getX();
	static double PastCoorY = self.getY();
  ///   Всё до этого момента заполняет массив идеального пути . Этой части на заполнение массива нужно world.getWaypoints().size() - тиков   

	if ((Convert(self.getX(), game) != Convert(PastCoorX, game)) || (Convert(self.getY(), game) != Convert(PastCoorY, game)))
	{
		PastCoorX = self.getX();
		PastCoorY = self.getY();
		Current_Tile++;
		if (best_way[Current_Path][0][Current_Tile] == -1){
			Current_Tile = 1;
			Current_Path = Current_Path + 1;
		}

	}
    if (world.getTick() > game.getInitialFreezeDurationTicks())
	{
		Current_Tile;
        static bool amTurning = false;
        int *axis = new int[2];

        // Узнаём нужно ли поворачивать
        amTurning = Check_Cond_For_Turning(self, world, &Current_Tile, &Current_Path,game);   //  в функции povorot нужно добавить проверку , нужно ли мне поворачивать в 

                                                                                                // каком-то тайле. И добавить проверку на тот случай, если идут несколько 
        if (world.getTick() == 181)
                int d = 9;

        // поворотных тайлов подряд (использование тормоза)       
        // Создаём вектор направления 
		if (world.getTick() == 830)
			int a = 5;
		if (amTurning) {
            int ans;
            ans = Create_Axis(axis, &Current_Tile, &Current_Path,self,game);   // возвращает 0 ,если точка, где остановится машина не является ни одной из двух последующих 
			move.setBrake(true);
		}
        else{
            Create_Axis_in_my_pos(axis, &Current_Tile, &Current_Path);
			move.setBrake(false);
			move.setEnginePower(1.0);
        }

        // Находим направление скорости
        double vect_Speed[2];
        vect_Speed[0] = self.getSpeedX();
        vect_Speed[1] = self.getSpeedY();
		 double x = vect_Speed[0];
		 double y = vect_Speed[1];
		 double znak;
		 double i = axis[0];
		 double j = axis[1];
		double phi = 0.;//надо определить, как отклониться. Есть такая идея, но  я не уверен.
		double znam = sqrt(x*x + y*y)*sqrt(i*i + j*j);
		if ((znam >0.00001) || (znam<-0.00001)) phi = acos(f_abs(x*i + y*j) / znam);
		if (axis[0] == 1)
			if (self.getSpeedY() < 0) znak = 1.0;
			else znak = -1.0;
		if (axis[0] == -1)
			if (self.getSpeedY() < 0) znak = -1.0;
			else znak = 1.0;
		if (axis[1] == 1)
			if (self.getSpeedX() < 0) znak = -1.0;
			else znak = 1.0;
		if (axis[1] == -1)
			if (self.getSpeedX() < 0) znak = 1.0;
			else znak = -1.0;
		move.setWheelTurn(ConvertAngle(2 * phi*znak));
		if (f_abs(phi) < 0.2) 
			move.setWheelTurn(ConvertAngle(self.getAngleTo(self.getX() + axis[0], self.getY() + axis[1])));
        // Направляем колёса так, чтобы вектор скорости и вектор направления совпадали

        //!! Нужно , чтобы машина ехала к дальней стороне от поворота, иначе она не  войдёт в поворот
		//!! в процессе поворота нужно постоянно проверять , находимся ли мы в тайлах, указанных в best_way. Иначе строить Local_Path
        //!! возможно стоит создать два массива для настоящей точки и для следующей. 
        
        delete[] axis;
	}

    move.setEnginePower(1.0);
}




MyStrategy::MyStrategy() { }


/*
if (world.getTick() == 400)
int d = 9;





createVectorAndPovorot(n, self, axis, world, best_way[0], best_way[1], best_way[2], game);//создаем вектор оси.Осталось только определить его направление(ли бо + либо - ).Это я сделал, но не сохранил.потом добавлю.Это считай что сделано
double x, y, i, j;
int znak;
i = axis[0];
j = axis[1];
x = self.getSpeedX();
y = self.getSpeedY();
double phi = 0.;//надо определить, как отклониться. Есть такая идея, но  я не уверен.
double znam = sqrt(x*x + y*y)*sqrt(i*i + j*j);
if ((znam >0.00001)||(znam<-0.00001)) phi = acos((x*i + y*j) / znam);
if (axis[0] != 0)
if (self.getSpeedY() < 0) znak = 1.0;
else znak = -1.0;
if (axis[1] != 0)
if (self.getSpeedX() < 0) znak = 1.0;
else znak = -1.0;
move.setWheelTurn(ConvertAngle(2*phi*znak));
*/

/*bool createVectorAndPovorot(int n, const Car& self, double* axis, const World& world, int *ansmas_X, int *ansmas_Y, int * putb, const Game& game)
{
	double sX, sY;
	sX = (self.getMass()*self.getSpeedX() * self.getSpeedX()) / (2 * 1000 * game.getCarCrosswiseMovementFrictionFactor()) - 200;//смотрю расстояние по Х, которое проеду до полной остановки на тормозах
	sY = (self.getMass()*self.getSpeedY() * self.getSpeedY()) / (2 * 1000 * game.getCarCrosswiseMovementFrictionFactor()) - 200;//и также по Y
	if (self.getSpeedX() < 0)  sX *= -1; //Если скорость противоположна направлению вектора оси(машина едет вверх либо влево), то пройденное расстояние надо отнимать
	if (self.getSpeedY() < 0)  sY *= -1;
	axis[0] = 0;
	axis[1] = 0;
	if (povorot(self.getX() + sX, self.getY() + sY, axis, game,world) == true)//если поворот возможен, то я создаю вектор для поворота.
	{					
			for (int i = 1; i < n; ++i)///!!!!!!!!!!!!!!!!!!!!!!!!!!!!!1
			{
				if (ansmas_X[i] == Convert(self.getX() + sX, game))
					if (ansmas_Y[i] == Convert(self.getY() + sY, game))
					{
						if (ansmas_X[i + 1] == Convert(self.getX() + sX, game))
							if (self.getSpeedX() > 0) axis[1] = 1;
	     						else axis[1] = -1;//Смотрю, следующий тайл нашего пути идет вверх или вниз.
						if (ansmas_Y[i + 1] == Convert(self.getY() + sY, game))
						    	if (self.getSpeedY() > 0) axis[0] = 1;
							     else axis[0] = -1;
								break;
					}
		

           	}
		return true;


	}
	//Если поворот не нужен(или невозможен) надо сделать вектор, которого он держался до вызова этой функции. 

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
*/
