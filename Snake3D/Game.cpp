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

vector<Cube> Game()
{
	vector<Cube> result;
	vector<Cube> board = GetBoard();
	vector<Cube> snake = GetSnake();
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
bool SnakeGame::round()
{
	if(snake.isValid() && !failed)
	{
		++(this->roundNumber);
	}
	else
	{
		failed = true;
		cout << "you lost! - your score was: " << this->score << endl;
		cout << "to continue press c" << endl;
		this->snake.cubes.front().color = vec4(1,1,0,1);
		return failed;
	}

	this->snake.makeMove(this->extendSnake);
	this->extendSnake = false;

	// Check if snake hits the food
	Cube head = this->snake.cubes.front();
	Cube food = this->food;

	if(head == food)
	{
		this->extendSnake = true;
		this->foodExist = false;
		this->score++;
	}
	return failed;
}

Cube SnakeGame::getFood()
{
	if(!this->foodExist)
	{
		this->food.position = randomPointOnCube();
		this->food.color = vec4(0,1,0,1);
		this->foodExist = true;
	}
	return this->food;
}

vec3 Snake::moveUp(int m, vec3 np, Side s, vec3 d)
{
	int round = SnakeGame::getInstance().getRound();
	
	if (round<2) {np.z++;}
	else
	{
		switch(s)
		{
			case Side::pos_x:
				if ((np.z > -m)&&(np.z < m)&&(np.y > -m)&&(np.y < m)) //if the snake is inside the face pos_x
				{
					if (d.z == 1) {np.z++;}
					else if (d.z == -1) {np.z--;}
					else if (d.y == 1) {np.y++;}
					else if (d.y == -1) {np.y--;}
				}
				else 
				{
					if ((np.z == -m)&&(d.z == -1)) {this->side = Side::neg_z; np.x--; }
					else if ((np.z == m)&&(d.z == 1)) {this->side = Side::pos_z; np.x--; }
					else if ((np.y == -m)&&(d.y == -1)) {this->side = Side::neg_y; np.x--; }
					else if ((np.y == m)&&(d.y == 1)) {this->side = Side::pos_y; np.x--; }
					
					else if ((np.z == m)&&(d.y == 1)) { np.y++; }
					else if ((np.z == m)&&(d.y == -1)) { np.y--; }
					else if ((np.z == -m)&&(d.y == 1)) { np.y++; }
					else if ((np.z == -m)&&(d.y == -1)) { np.y--; }
					else if ((np.y == m)&&(d.z == 1)) { np.z++; }
					else if ((np.y == m)&&(d.z == -1)) { np.z--; }
					else if ((np.y == -m)&&(d.z == 1)) { np.z++; }
					else if ((np.y == -m)&&(d.z == -1)) { np.z--; }	
				}
				break;
			case Side::pos_y:
				if ((np.z > -m)&&(np.z < m)&&(np.x > -m)&&(np.x < m)) //if the snake is inside the face pos_x
				{
					if (d.z == 1) {np.z++;}
					else if (d.z == -1) {np.z--;}
					else if (d.x == 1) {np.x++;}
					else if (d.x == -1) {np.x--;}
				}
				else 
				{
					if ((np.z == -m)&&(d.z == -1)) {this->side = Side::neg_z; np.y--; }
					else if ((np.z == m)&&(d.z == 1)) {this->side = Side::pos_z; np.y--; }
					else if ((np.x == -m)&&(d.x == -1)) {this->side = Side::neg_x; np.y--; }
					else if ((np.x == m)&&(d.x == 1)) {this->side = Side::pos_x; np.y--; }
					
					else if ((np.z == m)&&(d.x == 1)) { np.x++; }
					else if ((np.z == m)&&(d.x == -1)) { np.x--; }
					else if ((np.z == -m)&&(d.x == 1)) { np.x++; }
					else if ((np.z == -m)&&(d.x == -1)) { np.x--; }
					else if ((np.x == m)&&(d.z == 1)) { np.z++; }
					else if ((np.x == m)&&(d.z == -1)) { np.z--; }
					else if ((np.x == -m)&&(d.z == 1)) { np.z++; }
					else if ((np.x == -m)&&(d.z == -1)) { np.z--; }	
				}
				break;
			case Side::neg_x:
				if ((np.z > -m)&&(np.z < m)&&(np.y > -m)&&(np.y < m)) //if the snake is inside the face pos_x
				{
					if (d.z == 1) {np.z++;}
					else if (d.z == -1) {np.z--;}
					else if (d.y == 1) {np.y++;}
					else if (d.y == -1) {np.y--;}
				}
				else 
				{
					if ((np.z == -m)&&(d.z == -1)) {this->side = Side::neg_z; np.x++; }
					else if ((np.z == m)&&(d.z == 1)) {this->side = Side::pos_z; np.x++; }
					else if ((np.y == -m)&&(d.y == -1)) {this->side = Side::neg_y; np.x++; }
					else if ((np.y == m)&&(d.y == 1)) {this->side = Side::pos_y; np.x++; }
					
					else if ((np.z == m)&&(d.y == 1)) { np.y++; }
					else if ((np.z == m)&&(d.y == -1)) { np.y--; }
					else if ((np.z == -m)&&(d.y == 1)) { np.y++; }
					else if ((np.z == -m)&&(d.y == -1)) { np.y--; }
					else if ((np.y == m)&&(d.z == 1)) { np.z++; }
					else if ((np.y == m)&&(d.z == -1)) { np.z--; }
					else if ((np.y == -m)&&(d.z == 1)) { np.z++; }
					else if ((np.y == -m)&&(d.z == -1)) { np.z--; }		
				}
				break;
			case Side::neg_y:
				if ((np.z > -m)&&(np.z < m)&&(np.x > -m)&&(np.x < m)) //if the snake is inside the face pos_x
				{
					if (d.z == 1) {np.z++;}
					else if (d.z == -1) {np.z--;}
					else if (d.x == 1) {np.x++;}
					else if (d.x == -1) {np.x--;}
				}
				else 
				{
					if ((np.z == -m)&&(d.z == -1)) {this->side = Side::neg_z; np.y++; }
					else if ((np.z == m)&&(d.z == 1)) {this->side = Side::pos_z; np.y++; }
					else if ((np.x == -m)&&(d.x == -1)) {this->side = Side::neg_x; np.y++; }
					else if ((np.x == m)&&(d.x == 1)) {this->side = Side::pos_x; np.y++; }
					
					else if ((np.z == m)&&(d.x == 1)) { np.x++; }
					else if ((np.z == m)&&(d.x == -1)) { np.x--; }
					else if ((np.z == -m)&&(d.x == 1)) { np.x++; }
					else if ((np.z == -m)&&(d.x == -1)) { np.x--; }
					else if ((np.x == m)&&(d.z == 1)) { np.z++; }
					else if ((np.x == m)&&(d.z == -1)) { np.z--; }
					else if ((np.x == -m)&&(d.z == 1)) { np.z++; }
					else if ((np.x == -m)&&(d.z == -1)) { np.z--; }	
				}
				break;
				case Side::pos_z:
				if ((np.x > -m)&&(np.x < m)&&(np.y > -m)&&(np.y < m)) //if the snake is inside the face pos_x
				{
					if (d.x == 1) {np.x++;}
					else if (d.x == -1) {np.x--;}
					else if (d.y == 1) {np.y++;}
					else if (d.y == -1) {np.y--;}
				}
				else 
				{
					if ((np.x == -m)&&(d.x == -1)) {this->side = Side::neg_x; np.z--; }
					else if ((np.x == m)&&(d.x == 1)) {this->side = Side::pos_x; np.z--; }
					else if ((np.y == -m)&&(d.y == -1)) {this->side = Side::neg_y; np.z--; }
					else if ((np.y == m)&&(d.y == 1)) {this->side = Side::pos_y; np.z--; }
					
					else if ((np.x == m)&&(d.y == 1)) { np.y++; }
					else if ((np.x == m)&&(d.y == -1)) { np.y--; }
					else if ((np.x == -m)&&(d.y == 1)) { np.y++; }
					else if ((np.x == -m)&&(d.y == -1)) { np.y--; }
					else if ((np.y == m)&&(d.x == 1)) { np.x++; }
					else if ((np.y == m)&&(d.x == -1)) { np.x--; }
					else if ((np.y == -m)&&(d.x == 1)) { np.x++; }
					else if ((np.y == -m)&&(d.x == -1)) { np.x--; }	
				}
				break;
				case Side::neg_z:
				if ((np.x > -m)&&(np.x < m)&&(np.y > -m)&&(np.y < m)) //if the snake is inside the face pos_x
				{
					if (d.x == 1) {np.x++;}
					else if (d.x == -1) {np.x--;}
					else if (d.y == 1) {np.y++;}
					else if (d.y == -1) {np.y--;}
				}
				else 
				{
					if ((np.x == -m)&&(d.x == -1)) {this->side = Side::neg_x; np.z++; }
					else if ((np.x == m)&&(d.x == 1)) {this->side = Side::pos_x; np.z++; }
					else if ((np.y == -m)&&(d.y == -1)) {this->side = Side::neg_y; np.z++; }
					else if ((np.y == m)&&(d.y == 1)) {this->side = Side::pos_y; np.z++; }
					
					else if ((np.x == m)&&(d.y == 1)) { np.y++; }
					else if ((np.x == m)&&(d.y == -1)) { np.y--; }
					else if ((np.x == -m)&&(d.y == 1)) { np.y++; }
					else if ((np.x == -m)&&(d.y == -1)) { np.y--; }
					else if ((np.y == m)&&(d.x == 1)) { np.x++; }
					else if ((np.y == m)&&(d.x == -1)) { np.x--; }
					else if ((np.y == -m)&&(d.x == 1)) { np.x++; }
					else if ((np.y == -m)&&(d.x == -1)) { np.x--; }	
				}
				break;
		}
	}
	return np;
}

vec3 Snake::moveLeft(int m, vec3 np, Side s, vec3 d)
{ 
	switch(s)
	{
		case Side::pos_x:
			if ((np.z > -m)&&(np.z < m)&&(np.y > -m)&&(np.y < m)) //if the snake is inside the face pos_x
			{
				if (d.z == 1) {np.y--;}
				if (d.z == -1) {np.y++;}
				if (d.y == 1) {np.z++;}
				if (d.y == -1) {np.z--;}
			}
			else // if the snake is in the border
			{
				if ((np.z == -m)&&(d.y == -1)) {this->side = Side::neg_z; np.x--;}
				else if ((np.z == -m)&&(d.y == 1)) {np.z++;}
				else if ((np.z == m)&&(d.y == -1)) {np.z--;}
				else if ((np.z == m)&&(d.y == 1)) {this->side = Side::pos_z; np.x--;}
				else if ((np.z == -m)&&(d.z == -1)) {np.y++;} 
				else if ((np.z == m)&&(d.z == 1)) {np.y--;}
				
				else if ((np.y == -m)&&(d.z == -1)) {np.y++;}
				else if ((np.y == -m)&&(d.z == 1)) {this->side = Side::neg_y; np.x--;}
				else if ((np.y == m)&&(d.z == 1)) {np.y--;}
				else if ((np.y == m)&&(d.z == -1)) {this->side = Side::pos_y; np.x--;}
				else if ((np.y == -m)&&(d.y == -1)) {np.z--;}
				else if ((np.y == m)&&(d.y == 1)) { np.z++;}
			}
			break;
		case Side::pos_y:
			if ((np.z > -m)&&(np.z < m)&&(np.x > -m)&&(np.x < m)) //if the snake is inside the face pos_x
			{
				if (d.z == 1) {np.x++;}
				if (d.z == -1) {np.x--;}
				if (d.x == -1) {np.z++;}
				if (d.x == 1) {np.z--;}
			}
			else // if the snake is in the border
			{
				if ((np.z == -m)&&(d.x == 1)) {this->side = Side::neg_z; np.y--;}
				else if ((np.z == -m)&&(d.x == -1)) {np.z++;}
				else if ((np.z == m)&&(d.x == 1)) {np.z--;}
				else if ((np.z == m)&&(d.x == -1)) {this->side = Side::pos_z; np.y--;}
				else if ((np.z == -m)&&(d.z == -1)) {np.x--;} 
				else if ((np.z == m)&&(d.z == 1)) {np.x++;}
				
				else if ((np.x == -m)&&(d.z == 1)) {np.x++;}
				else if ((np.x == -m)&&(d.z == -1)) {this->side = Side::neg_x; np.y--;}
				else if ((np.x == m)&&(d.z == -1)) {np.x--;}
				else if ((np.x == m)&&(d.z == 1)) {this->side = Side::pos_x; np.y--;}
				else if ((np.x == -m)&&(d.x == -1)) {np.z++;}
				else if ((np.x == m)&&(d.x == 1)) { np.z--;}
			}
			break;
		case Side::neg_x:
			if ((np.z > -m)&&(np.z < m)&&(np.y > -m)&&(np.y < m)) //if the snake is inside the face pos_x
			{
				if (d.z == 1) {np.y++;}
				if (d.z == -1) {np.y--;}
				if (d.y == 1) {np.z--;}
				if (d.y == -1) {np.z++;}
			}
			else // if the snake is in the border
			{
				if ((np.z == -m)&&(d.y == 1)) {this->side = Side::neg_z; np.x++;}
				else if ((np.z == -m)&&(d.y == -1)) {np.z++;}
				else if ((np.z == m)&&(d.y == 1)) {np.z--;}
				else if ((np.z == m)&&(d.y == -1)) {this->side = Side::pos_z; np.x++;}
				else if ((np.z == -m)&&(d.z == -1)) { np.y--;} 
				else if ((np.z == m)&&(d.z == 1)) {; np.y++;}
				
				else if ((np.y == -m)&&(d.z == 1)) {np.y++;}
				else if ((np.y == -m)&&(d.z == -1)) {this->side = Side::neg_y; np.x++;}
				else if ((np.y == m)&&(d.z == -1)) {np.y--;}
				else if ((np.y == m)&&(d.z == 1)) {this->side = Side::pos_y; np.x++;}
				else if ((np.y == -m)&&(d.y == -1)) {np.z++;}
				else if ((np.y == m)&&(d.y == 1)) {np.z--;}
			}
			break;
		case Side::neg_y:
			if ((np.z > -m)&&(np.z < m)&&(np.x > -m)&&(np.x < m)) //if the snake is inside the face pos_x
			{
				if (d.z == 1) {np.x--;}
				if (d.z == -1) {np.x++;}
				if (d.x == -1) {np.z--;}
				if (d.x == 1) {np.z++;}
			}
			else // if the snake is in the border
			{
				if ((np.z == -m)&&(d.x == -1)) {this->side = Side::neg_z; np.y++;}
				else if ((np.z == -m)&&(d.x == 1)) {np.z++;}
				else if ((np.z == m)&&(d.x == -1)) {np.z--;}
				else if ((np.z == m)&&(d.x == 1)) {this->side = Side::pos_z; np.y++;}
				else if ((np.z == -m)&&(d.z == -1)) {np.x++;} 
				else if ((np.z == m)&&(d.z == 1)) {np.x--;}
				
				else if ((np.x == -m)&&(d.z == -1)) {np.x++;}
				else if ((np.x == -m)&&(d.z == 1)) {this->side = Side::neg_x; np.y++;}
				else if ((np.x == m)&&(d.z == 1)) {np.x--;}
				else if ((np.x == m)&&(d.z == -1)) {this->side = Side::pos_x; np.y++;}
				else if ((np.x == -m)&&(d.x == -1)) {np.z--;}
				else if ((np.x == m)&&(d.x == 1)) { np.z++;}
			}
			break;
			case Side::pos_z:
			if ((np.x > -m)&&(np.x < m)&&(np.y > -m)&&(np.y < m)) //if the snake is inside the face pos_x
			{
				if (d.x == 1) {np.y++;}
				if (d.x == -1) {np.y--;}
				if (d.y == 1) {np.x--;}
				if (d.y == -1) {np.x++;}
			}
			else // if the snake is in the border
			{
				if ((np.x == -m)&&(d.y == 1)) {this->side = Side::neg_x; np.z--;}
				else if ((np.x == -m)&&(d.y == -1)) {np.x++;}
				else if ((np.x == m)&&(d.y == 1)) {np.x--;}
				else if ((np.x == m)&&(d.y == -1)) {this->side = Side::pos_x; np.z--;}
				else if ((np.x == -m)&&(d.x == -1)) {np.y--;} 
				else if ((np.x == m)&&(d.x == 1)) {np.y++;}
				
				else if ((np.y == -m)&&(d.x == 1)) {np.y++;}
				else if ((np.y == -m)&&(d.x == -1)) {this->side = Side::neg_y; np.z--;}
				else if ((np.y == m)&&(d.x == -1)) {np.y--;}
				else if ((np.y == m)&&(d.x == 1)) {this->side = Side::pos_y; np.z--;}
				else if ((np.y == -m)&&(d.y == -1)) {np.x++;}
				else if ((np.y == m)&&(d.y == 1)) { np.x--;}
			}
			break;
			case Side::neg_z:
			if ((np.x > -m)&&(np.x < m)&&(np.y > -m)&&(np.y < m)) //if the snake is inside the face pos_x
			{
				if (d.x == 1) {np.y--;}
				if (d.x == -1) {np.y++;}
				if (d.y == 1) {np.x++;}
				if (d.y == -1) {np.x--;}
			}
			else // if the snake is in the border
			{
				if ((np.x == -m)&&(d.y == -1)) {this->side = Side::neg_x; np.z++;}
				else if ((np.x == -m)&&(d.y == 1)) {np.x++;}
				else if ((np.x == m)&&(d.y == -1)) {np.x--;}
				else if ((np.x == m)&&(d.y == 1)) {this->side = Side::pos_x; np.z++;}
				else if ((np.x == -m)&&(d.x == -1)) {np.y++;} 
				else if ((np.x == m)&&(d.x == 1)) {np.y--;}
				
				else if ((np.y == -m)&&(d.x == -1)) {np.y++;}
				else if ((np.y == -m)&&(d.x == 1)) {this->side = Side::neg_y; np.z--;}
				else if ((np.y == m)&&(d.x == 1)) {np.y--;}
				else if ((np.y == m)&&(d.x == -1)) {this->side = Side::pos_y; np.z--;}
				else if ((np.y == -m)&&(d.y == -1)) {np.x--;}
				else if ((np.y == m)&&(d.y == 1)) { np.x++;}
			}
			break;
	}
	SnakeGame::getInstance().move(Direction::up);
	return np;
}

vec3 Snake::moveRight(int m, vec3 np, Side s, vec3 d)
{ 
	switch(s)
	{
		case Side::pos_x:
			if ((np.z > -m)&&(np.z < m)&&(np.y > -m)&&(np.y < m)) //if the snake is inside the face pos_x
			{
				if (d.z == 1) {np.y++;}
				if (d.z == -1) {np.y--;}
				if (d.y == 1) {np.z--;}
				if (d.y == -1) {np.z++;}
			}
			else // if the snake is in the border
			{
				if ((np.z == -m)&&(d.y == 1)) {this->side = Side::neg_z; np.x--;}
				else if ((np.z == -m)&&(d.y == -1)) {np.z++;}
				else if ((np.z == m)&&(d.y == 1)) {np.z--;}
				else if ((np.z == m)&&(d.y == -1)) {this->side = Side::pos_z; np.x--;}
				else if ((np.z == -m)&&(d.z == -1)) {np.y--;} 
				else if ((np.z == m)&&(d.z == 1)) {np.y++;}
				
				else if ((np.y == -m)&&(d.z == 1)) {np.y++;}
				else if ((np.y == -m)&&(d.z == -1)) {this->side = Side::neg_y; np.x--;}
				else if ((np.y == m)&&(d.z == -1)) {np.y--;}
				else if ((np.y == m)&&(d.z == 1)) {this->side = Side::pos_y; np.x--;}
				else if ((np.y == -m)&&(d.y == -1)) {np.z++;}
				else if ((np.y == m)&&(d.y == 1)) { np.z--;}
			}
			break;
		case Side::pos_y:
			if ((np.z > -m)&&(np.z < m)&&(np.x > -m)&&(np.x < m)) //if the snake is inside the face pos_x
			{
				if (d.z == 1) {np.x--;}
				if (d.z == -1) {np.x++;}
				if (d.x == -1) {np.z--;}
				if (d.x == 1) {np.z++;}
			}
			else // if the snake is in the border
			{
				if ((np.z == -m)&&(d.x == -1)) {this->side = Side::neg_z; np.y--;}
				else if ((np.z == -m)&&(d.x == 1)) {np.z++;}
				else if ((np.z == m)&&(d.x == -1)) {np.z--;}
				else if ((np.z == m)&&(d.x == 1)) {this->side = Side::pos_z; np.y--;}
				else if ((np.z == -m)&&(d.z == -1)) {np.x++;} 
				else if ((np.z == m)&&(d.z == 1)) {np.x--;}
				
				else if ((np.x == -m)&&(d.z == -1)) {np.x++;}
				else if ((np.x == -m)&&(d.z == 1)) {this->side = Side::neg_x; np.y--;}
				else if ((np.x == m)&&(d.z == 1)) {np.x--;}
				else if ((np.x == m)&&(d.z == -1)) {this->side = Side::pos_x; np.y--;}
				else if ((np.x == -m)&&(d.x == -1)) {np.z--;}
				else if ((np.x == m)&&(d.x == 1)) { np.z++;}
			}
			break;
		case Side::neg_x:
			if ((np.z > -m)&&(np.z < m)&&(np.y > -m)&&(np.y < m)) //if the snake is inside the face pos_x
			{
				if (d.z == 1) {np.y--;}
				if (d.z == -1) {np.y++;}
				if (d.y == 1) {np.z++;}
				if (d.y == -1) {np.z--;}
			}
			else // if the snake is in the border
			{
				if ((np.z == -m)&&(d.y == -1)) {this->side = Side::neg_z; np.x++;}
				else if ((np.z == -m)&&(d.y == 1)) {np.z++;}
				else if ((np.z == m)&&(d.y == -1)) {np.z--;}
				else if ((np.z == m)&&(d.y == 1)) {this->side = Side::pos_z; np.x++;}
				else if ((np.z == -m)&&(d.z == -1)) { np.y++;} 
				else if ((np.z == m)&&(d.z == 1)) {; np.y--;}
				
				else if ((np.y == -m)&&(d.z == -1)) {np.y++;}
				else if ((np.y == -m)&&(d.z == 1)) {this->side = Side::neg_y; np.x++;}
				else if ((np.y == m)&&(d.z == 1)) {np.y--;}
				else if ((np.y == m)&&(d.z == -1)) {this->side = Side::pos_y; np.x++;}
				else if ((np.y == -m)&&(d.y == -1)) {np.z--;}
				else if ((np.y == m)&&(d.y == 1)) {np.z++;}
			}
			break;
		case Side::neg_y:
			if ((np.z > -m)&&(np.z < m)&&(np.x > -m)&&(np.x < m)) //if the snake is inside the face pos_x
			{
				if (d.z == 1) {np.x++;}
				if (d.z == -1) {np.x--;}
				if (d.x == -1) {np.z++;}
				if (d.x == 1) {np.z--;}
			}
			else // if the snake is in the border
			{
				if ((np.z == -m)&&(d.x == 1)) {this->side = Side::neg_z; np.y++;}
				else if ((np.z == -m)&&(d.x == -1)) {np.z++;}
				else if ((np.z == m)&&(d.x == 1)) {np.z--;}
				else if ((np.z == m)&&(d.x == -1)) {this->side = Side::pos_z; np.y++;}
				else if ((np.z == -m)&&(d.z == -1)) {np.x--;} 
				else if ((np.z == m)&&(d.z == 1)) {np.x++;}
				
				else if ((np.x == -m)&&(d.z == 1)) {np.x++;}
				else if ((np.x == -m)&&(d.z == -1)) {this->side = Side::neg_x; np.y++;}
				else if ((np.x == m)&&(d.z == -1)) {np.x--;}
				else if ((np.x == m)&&(d.z == 1)) {this->side = Side::pos_x; np.y++;}
				else if ((np.x == -m)&&(d.x == -1)) {np.z++;}
				else if ((np.x == m)&&(d.x == 1)) { np.z--;}
			}
			break;
			case Side::pos_z:
			if ((np.x > -m)&&(np.x < m)&&(np.y > -m)&&(np.y < m)) //if the snake is inside the face pos_x
			{
				if (d.x == 1) {np.y--;}
				if (d.x == -1) {np.y++;}
				if (d.y == 1) {np.x++;}
				if (d.y == -1) {np.x--;}
			}
			else // if the snake is in the border
			{
				if ((np.x == -m)&&(d.y == -1)) {this->side = Side::neg_x; np.z--;}
				else if ((np.x == -m)&&(d.y == 1)) {np.x++;}
				else if ((np.x == m)&&(d.y == -1)) {np.x--;}
				else if ((np.x == m)&&(d.y == 1)) {this->side = Side::pos_x; np.z--;}
				else if ((np.x == -m)&&(d.x == -1)) {np.y++;} 
				else if ((np.x == m)&&(d.x == 1)) {np.y--;}
				
				else if ((np.y == -m)&&(d.x == -1)) {np.y++;}
				else if ((np.y == -m)&&(d.x == 1)) {this->side = Side::neg_y; np.z--;}
				else if ((np.y == m)&&(d.x == 1)) {np.y--;}
				else if ((np.y == m)&&(d.x == -1)) {this->side = Side::pos_y; np.z--;}
				else if ((np.y == -m)&&(d.y == -1)) {np.x--;}
				else if ((np.y == m)&&(d.y == 1)) { np.x++;}
			}
			break;
			case Side::neg_z:
			if ((np.x > -m)&&(np.x < m)&&(np.y > -m)&&(np.y < m)) //if the snake is inside the face pos_x
			{
				if (d.x == 1) {np.y++;}
				if (d.x == -1) {np.y--;}
				if (d.y == 1) {np.x--;}
				if (d.y == -1) {np.x++;}
			}
			else // if the snake is in the border
			{
				if ((np.x == -m)&&(d.y == 1)) {this->side = Side::neg_x; np.z++;}
				else if ((np.x == -m)&&(d.y == -1)) {np.x++;}
				else if ((np.x == m)&&(d.y == 1)) {np.x--;}
				else if ((np.x == m)&&(d.y == -1)) {this->side = Side::pos_x; np.z++;}
				else if ((np.x == -m)&&(d.x == -1)) {np.y--;} 
				else if ((np.x == m)&&(d.x == 1)) {np.y++;}
				
				else if ((np.y == -m)&&(d.x == 1)) {np.y++;}
				else if ((np.y == -m)&&(d.x == -1)) {this->side = Side::neg_y; np.z--;}
				else if ((np.y == m)&&(d.x == -1)) {np.y--;}
				else if ((np.y == m)&&(d.x == 1)) {this->side = Side::pos_y; np.z--;}
				else if ((np.y == -m)&&(d.y == -1)) {np.x++;}
				else if ((np.y == m)&&(d.y == 1)) { np.x--;}
			}
			break;
	}
	SnakeGame::getInstance().move(Direction::up);
	return np;
}

void Snake::makeMove(bool extendSnake)
{
	int m = (SnakeGame::getInstance().getBoardSize() / 2) + 1;
	
	Snake snake_temp = SnakeGame::getInstance().getSnake();
	Cube head = snake_temp.cubes.front();
	snake_temp.cubes.pop_front();
	Cube head2 =  snake_temp.cubes.front();
	vec3 dir = head.position - head2.position; 

	Direction d = this->direction;
	Side s = this->side;
	vec3 np = head.position;
	
	switch(d)
	{
		case Direction::up:
			np = moveUp(m, np, s, dir);
			break;
		case Direction::left:
			np = moveLeft(m, np, s, dir);
			break;
		case Direction::right:
			np = moveRight(m, np, s, dir);
			break;
	}

	Cube newHead(np, head.color);
	this->cubes.push_front(newHead);
	if(!extendSnake)
		this->cubes.pop_back();
}

bool Snake::isValid()
{
	Cube head = this->cubes.front();
	for(auto it = ++(this->cubes.begin()); it != this->cubes.end(); it++)
	{
		if((*it) == head) return false;
	}
	return true;
}