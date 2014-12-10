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

struct snake
{
	std::list<Cube> cubes;
	direction direction;
};

class SnakeGame // SINGLETON
{
	snake snake;
	int roundNumber;

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
    private:
        SnakeGame() {};
        SnakeGame(SnakeGame const&);
        void operator=(SnakeGame const&);
};

std::vector<Cube> Game();