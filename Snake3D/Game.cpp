#include "Game.h"
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */

using namespace std; 

vector<Cube> WallOfCubes(range xr, range yr, range zr)
{
	vector<Cube> result;
	for(int x=xr.from; x<=xr.to ; x++)
	{
		for(int y=yr.from; y<=yr.to ; y++)
		{
			for(int z=zr.from; z<=zr.to ; z++)
			{
				Cube c = { vec3(x,y,z), vec4(1,0,0,0) };
				result.push_back(c);
			}
		}
	}
	return result;
}

vector<Cube> GetBoard()
{
	vector<Cube> result;
	int s = SnakeGame::getInstance().getBoardSize() / 2;

	// Create walls
	vector<Cube> c1 = WallOfCubes(range(-s,s), range(-s,s), range(s,s));
	vector<Cube> c2 = WallOfCubes(range(-s,s), range(-s,s), range(-s,-s));
	vector<Cube> c3 = WallOfCubes(range(-s,s), range(s,s), range(-(s-1),(s-1)));
	vector<Cube> c4 = WallOfCubes(range(-s,s), range(-s,-s), range(-(s-1),(s-1)));
	vector<Cube> c5 = WallOfCubes(range(s,s), range(-(s-1),(s-1)), range(-(s-1),(s-1)));
	vector<Cube> c6 = WallOfCubes(range(-s,-s), range(-(s-1),(s-1)), range(-(s-1),(s-1)));
	
	// Concatenate walls
	result.insert( result.end(), c1.begin(), c1.end());
	result.insert( result.end(), c2.begin(), c2.end());
	result.insert( result.end(), c3.begin(), c3.end());
	result.insert( result.end(), c4.begin(), c4.end());
	result.insert( result.end(), c5.begin(), c5.end());
	result.insert( result.end(), c6.begin(), c6.end());
	
	return result;
}

vector<Cube> GetSnake()
{
	vector<Cube> result;
	return result;
}

vector<Cube> GetFood()
{
	vector<Cube> result;
	result.push_back(SnakeGame::getInstance().getFood());
	return result;
}

vector<Cube> Game()
{
	vector<Cube> result;
	vector<Cube> board = GetBoard();
	vector<Cube> snake = GetSnake();
	vector<Cube> food = GetFood();
	result.insert(result.end(), food.begin(), food.end());
	result.insert(result.end(), board.begin(), board.end());
	result.insert(result.end(), snake.begin(), snake.end());
	return result;
}

int getRandInt(int from, int to)
{
	return (rand() % (to+1)) + from;
}

vec3 randomPointOnCube()
{
	srand(time(NULL));
	enum side { pos_z, neg_z, pos_x, neg_x, pos_y, neg_y, count };
	int m = (SnakeGame::getInstance().getBoardSize() / 2) + 1;
	side s = (side)getRandInt(0, side::count - 1);
	int x = getRandInt(-m, m);
	int y = getRandInt(-m, m);
	int z = getRandInt(-m, m);
	switch(s)
	{
		case side::pos_z:
			z = m;
			break;
		case side::neg_z:
			z = -m;
			break;
		case side::pos_x:
			x = m;
			break;
		case side::neg_x:
			x = -m;
			break;
		case side::pos_y:
			y = m;
			break;
		case side::neg_y:
			y = -m;
			break;
	}
	return vec3(x, y, z);
}

// MAIN GAME LOOP
void SnakeGame::round()
{
	cout << "round " << ++(this->roundNumber)
	<< " snake movement is " << this->snake.direction << endl;
}

Cube SnakeGame::getFood()
{
	if(!foodExist)
	{
		// TODO: Make random food
		this->food.position = randomPointOnCube();
		this->food.color = vec4(0,1,0,1);
		foodExist = true;
	}
	return this->food;
}

