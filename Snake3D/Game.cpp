#include "Game.h"

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

	// Create walls
	vector<Cube> c1 = WallOfCubes(range(-2,2), range(-2,2), range(2,2));
	vector<Cube> c2 = WallOfCubes(range(-2,2), range(-2,2), range(-2,-2));
	vector<Cube> c3 = WallOfCubes(range(-2,2), range(2,2), range(-1,1));
	vector<Cube> c4 = WallOfCubes(range(-2,2), range(-2,-2), range(-1,1));
	vector<Cube> c5 = WallOfCubes(range(2,2), range(-1,1), range(-1,1));
	vector<Cube> c6 = WallOfCubes(range(-2,-2), range(-1,1), range(-1,1));
	
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

vector<Cube> Game()
{
	vector<Cube> result;
	vector<Cube> board = GetBoard();
	vector<Cube> snake = GetSnake();

	result.insert( result.end(), board.begin(), board.end());
	result.insert( result.end(), snake.begin(), snake.end());
	return result;
}