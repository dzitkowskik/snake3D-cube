#include "Angel.h"
#include <vector>
#include <list>

struct Cube
{
	 vec3 position;
	 vec4 color;
};

struct range
{
	int from;
	int to;

	range(int _from, int _to)
	{
		from = _from;
		to = _to;
	}
	~range(){}
};

enum direction
{
	left,
	right,
	up,
	down,
	count
};

struct Snake
{
	std::list<Cube> cubes;
	direction direction;
};

class SnakeGame // SINGLETON
{
	Snake snake;
	Cube food;
	bool foodExist;
	int roundNumber;
	int boardSize;

	public:
        static SnakeGame& getInstance()
        {
            static SnakeGame instance; 
            return instance;
        }
		void round();
		void move(direction _direction)
		{
			snake.direction = _direction;
		}
		Cube getFood();
		Snake getSnake(){ return this->snake; }
		void initBoard(int _size)
		{
			boardSize = _size;
			Cube c1 = { vec3(boardSize/2 + 1, 0, 0), vec4(0,0,1,0) };
			Cube c2 = { vec3(boardSize/2 + 1, 0, -1), vec4(0,0,1,0) };
			this->snake.cubes.push_back(c1);
			this->snake.cubes.push_back(c2);
		}
		int getBoardSize() { return this->boardSize; }
		void eatFood() { this->foodExist = false; }
    private:
        SnakeGame() {};
        SnakeGame(SnakeGame const&);
        void operator=(SnakeGame const&);
};

std::vector<Cube> Game();