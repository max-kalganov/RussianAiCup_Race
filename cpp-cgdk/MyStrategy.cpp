#include "MyStrategy.h"

#define PI 3.14159265358979323846
#define _USE_MATH_DEFINES
#define fabs std::fabs
#include <cmath>
#include <cstdlib>

using namespace model;
using namespace std;

struct Tree
{
	//	int FatherR;  // ������
	int FatherC;  // �������
	int X_tile;   // ����� � ��������� X  � ������� , ��� � ������ ����������� ���� 
	int Y_tile;   // ����� � ��������� Y � ������� , ��� � ������ ����������� ����
	TileType tile_t;/////////////////////// ����� �������� �������� �� ���
};

int Convert(double num, const Game& game){
	int ans = num / game.getTrackTileSize();
	return ans;
}
bool povorot(int X, int Y, double* axis, const Game& game)
{
	//����� ��������, ��� � ����� ��������� ������� ����� ��� ����� �������. �� ���� ������ ����������� � ���� ��������� �����, �� �� �������� ������� ����� � ����������(������� � ��������), ������� ��� ������ ���������, ���� � ������ ���� �������
	int TileX = Convert(X, game);
	int TileY = Convert(Y, game);
	if (Convert(X + game.getTrackTileMargin() + 105, game) != TileX) return false;
	if (Convert(X - game.getTrackTileMargin() - 105, game) != TileX) return false;
	if (Convert(Y + game.getTrackTileMargin() + 105, game) != TileY) return false;
	if (Convert(Y - game.getTrackTileMargin() - 105, game) != TileY) return false;

	return true;

}
bool createVectorAndPovorot(int n, const Car& self, double* axis, const World& world, int *ansmas_X, int *ansmas_Y, int * putb, const Game& game)
{
	double sX, sY;
	sX = (self.getMass()*self.getSpeedX()* self.getSpeedX()) / (2 * game.getCarCrosswiseMovementFrictionFactor());//������ ���������� �� �, ������� ������ �� ������ ��������� �� ��������
	sY = (self.getMass()*self.getSpeedY() * self.getSpeedY()) / (2 * game.getCarCrosswiseMovementFrictionFactor());//� ����� �� Y
	if (self.getSpeedX() < 0)  sX *= -1; //���� �������� �������������� ����������� ������� ���(������ ���� ����� ���� �����), �� ���������� ���������� ���� ��������
	if (self.getSpeedY() < 0)  sY *= -1;
	axis[0] = 0;
	axis[1] = 0;
	if (povorot(self.getX() + sX, self.getY() + sY, axis, game) == true)//���� ������� ��������, �� � ������ ������ ��� ��������.
	{
		if ((world.getTilesXY()[Convert(self.getX() + sX, game)][Convert(self.getY() + sY, game)] == LEFT_TOP_CORNER) || (world.getTilesXY()[Convert(self.getX() + sX, game)][Convert(self.getY() + sY, game)] == RIGHT_TOP_CORNER) || (world.getTilesXY()[Convert(self.getX() + sX, game)][Convert(self.getY() + sY, game)] == LEFT_BOTTOM_CORNER) || (world.getTilesXY()[Convert(self.getX() + sX, game)][Convert(self.getY() + sY, game)] == RIGHT_BOTTOM_CORNER))
			axis[1] = 1;
		else axis[0] = 1;

		{
			int p = 0;
			//���� ������� ����� ���� ������ ���� ����� �� ������� ����(p), ��� ��������� ���� � ���-�� ���������(n) � ���� �������, ����� �� ���������, ���� ����� ����
			while (putb[p++] != 1)

				for (int i = p; i < n; ++i)
				{
					if (ansmas_X[i] = Convert(self.getX() + sX, game))
						if (ansmas_Y[i] = Convert(self.getY() + sY, game))
						{
							if (ansmas_X[i + 1] == Convert(self.getX() + sX, game)) axis[0] = 1; //����� ����� ������� ��� ������� convert().������ �� ���� ��� ������� � ��������� ����� - ������ ���������� ? ��� ����� �� �����, ��� ��� ��� ��������� ��������� ����, ��� � �������)
							if (ansmas_Y[i + 1] == Convert(self.getY() + sY, game)) axis[1] = 1;//������, ��������� ���� ������ ���� ���� ����� ��� ����.
							break;
						}
				}
		}

		return true;


	}
	//���� ������� �� �����(��� ����������) ���� ������� ������, �������� �� �������� �� ������ ���� �������. 
	else return false;
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
	//////// ����� ����� �������� ����� , ��� ����� ������������ ����� ������������ ��������� �����������



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
		main_mas[next] = new Tree[count_now * 3 + 1];    // �������� ����� �����������
		for (int i = 0; i < count_now; i++) {
			count_next += Write_New_Points(main_mas, count_next, count_now, next, now, i, world, &flag,x_finish,y_finish);  // �� ������ , ��� ��������� **mas !!!
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



void MyStrategy::move(const Car& self, const World& world, const Game& game, Move& move) 
{
	model::TileType tileType = world.getTilesXY()[0][0];
	const int n= 29;
	int best_way[3][n];//����� ����������� ������� ����� ����� �������� ������ ������
	int mas_size = 0;
	//if (world.getTick() < game.getInitialFreezeDurationTicks())
		Findbestway(self, world, &mas_size, best_way[0], best_way[1], game);
	
	for (int i = 0; i < n; ++i)
		if (best_way[0][i] == Convert(self.getX(), game))
			if (best_way[1][i] == Convert(self.getY(), game))
				best_way[2][i] = 1;

	double *axis = new double[2];

	if (createVectorAndPovorot(n, self, axis, world, best_way[0], best_way[1], best_way[3], game) == true)//������� ������ ���.�������� ������ ���������� ��� �����������(�� �� + ���� - ).��� � ������, �� �� ��������.����� �������.��� ������ ��� �������
	 {

		 move.setBrake(true);//����� ����������� , ����� ����� �������� ������

		double x, y, i, j;
		if (self.getSpeedX() < 0) axis[0] *= -1;
		if (self.getSpeedY() < 0) axis[1] *= -1;
		i = axis[0];
		j = axis[1];
		x = self.getSpeedX();
		y = self.getSpeedY();

		double phi = 0;//���� ����������, ��� �����������. ���� ����� ����, ��  � �� ������.
		double znam = sqrt(x*x + y*y)*sqrt(i*i + j*j);
		  if (znam != 0) phi = acos(fabs(x*i + y*j) / znam);
		    move.setWheelTurn(tan(phi));
		    move.setEnginePower(1.0);
		    //move.setThrowProjectile(true);
		    //move.setSpillOil(true);
	 }

    
      //  move.setUseNitro(true);
    
}




MyStrategy::MyStrategy() { }
