#include "Game.h"
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */

using namespace std; 

vector<Cube> WallOfCubes(Range xr, Range yr, Range zr)
{
	vector<Cube> result;
	for(int x=xr.from; x<=xr.to ; x++)
	{
		for(int y=yr.from; y<=yr.to ; y++)
		{
			for(int z=zr.from; z<=zr.to ; z++)
			{
				Cube c(vec3(x,y,z), vec4(1,0,0,0));
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
	vector<Cube> c1 = WallOfCubes(Range(-s,s), Range(-s,s), Range(s,s));
	vector<Cube> c2 = WallOfCubes(Range(-s,s), Range(-s,s), Range(-s,-s));
	vector<Cube> c3 = WallOfCubes(Range(-s,s), Range(s,s), Range(-(s-1),(s-1)));
	vector<Cube> c4 = WallOfCubes(Range(-s,s), Range(-s,-s), Range(-(s-1),(s-1)));
	vector<Cube> c5 = WallOfCubes(Range(s,s), Range(-(s-1),(s-1)), Range(-(s-1),(s-1)));
	vector<Cube> c6 = WallOfCubes(Range(-s,-s), Range(-(s-1),(s-1)), Range(-(s-1),(s-1)));
	
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
	Snake s = SnakeGame::getInstance().getSnake();
	result.insert(result.begin(), s.cubes.begin(), s.cubes.end());
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
	
	int m = (SnakeGame::getInstance().getBoardSize() / 2) + 1;
	Side s = (Side)getRandInt(0, Side::sides_count - 1);
	int x = getRandInt(-m, m);
	int y = getRandInt(-m, m);
	int z = getRandInt(-m, m);
	switch(s)
	{
		case Side::pos_z: z = m; break;
		case Side::neg_z: z = -m; break;
		case Side::pos_x: x = m; break;
		case Side::neg_x: x = -m; break;
		case Side::pos_y: y = m; break;
		case Side::neg_y: y = -m; break;
	}
	return vec3(x, y, z);
}

// MAIN GAME LOOP
void SnakeGame::round()
{
	cout << "round " << ++(this->roundNumber)
	<< " snake movement is " << this->snake.direction << endl;

	this->snake.makeMove();
}

Cube SnakeGame::getFood()
{
	if(!foodExist)
	{
		this->food.position = randomPointOnCube();
		this->food.color = vec4(0,1,0,1);
		foodExist = true;
	}
	return this->food;
}

vec3 Snake::moveUp(int m, vec3 np, Side s)
{
	switch(s)
	{
		case Side::pos_x:
			if(np.y < m) { np.y++; }
			else { this->side = Side::pos_y; np.x--; }
			break;
		case Side::pos_y:
			if(np.x > -m) { np.x--; }
			else { this->side = Side::neg_x; np.y--; }
			break;
		case Side::neg_x:
			if(np.y > -m) { np.y--; }
			else { this->side = Side::neg_y; np.x++; }
			break;
		case Side::neg_y:
			if(np.x < m) { np.x++; }
			else { this->side = Side::pos_x; np.y++; }
			break;
	}
	return np;
}

vec3 Snake::moveDown(int m, vec3 np, Side s)
{
	switch(s)
	{
		case Side::pos_x:
			if(np.y > -m) { np.y--; }
			else { this->side = Side::neg_y; np.x--; }
			break;
		case Side::pos_y:
			if(np.x < m) { np.x++; }
			else { this->side = Side::pos_x; np.y--; }
			break;
		case Side::neg_x:
			if(np.y < m) { np.y++; }
			else { this->side = Side::pos_y; np.x++; }
			break;
		case Side::neg_y:
			if(np.x > -m) { np.x--; }
			else { this->side = Side::neg_x; np.y++; }
			break;
	}
	return np;
}


void Snake::makeMove()
{
	int m = (SnakeGame::getInstance().getBoardSize() / 2) + 1;
	Cube head = this->cubes.front();
	Direction d = this->direction;
	Side s = this->side;
	vec3 np = head.position;
	
	switch(d)
	{
		case Direction::up:
			np = moveUp(m, np, s);
			break;
		case Direction::down:
			np = moveDown(m, np, s);
			break;
		case Direction::left:
			
			break;
		case Direction::right:
			
			break;
	}

	Cube newHead(np, head.color);
	this->cubes.push_front(newHead);
	this->cubes.pop_back();
}