#include "Angel.h"
#include <vector>
#include <list>

struct Cube
{
	vec3 position;
	vec4 color;

	Cube(){}
	~Cube(){}
	Cube(Cube const& c)
	{
		this->position = c.position;
		this->color = c.color;
	}
	Cube(vec3 _position, vec4 _color=vec4(0,0,1,1))
	{
		this->position = _position;
		this->color = _color;
	}
	inline bool operator == (const Cube &other) const
	{
	return 
		other.position.x == this->position.x &&
		other.position.y == this->position.y &&
		other.position.z == this->position.z;
	}
};

struct Range
{
	int from;
	int to;

	Range(int _from, int _to)
	{
		from = _from;
		to = _to;
	}
	~Range(){}
};

enum Direction 
{ 
	up, 
	down, 
	left, 
	right, 
	directions_count 
};

enum Side 
{
	pos_z, 
	neg_z, 
	pos_x, 
	neg_x, 
	pos_y, 
	neg_y, 
	sides_count 
};

class Snake
{
	public:
		std::list<Cube> cubes;
		Direction direction;
		Side side;
		void makeMove(bool extendSnake);
		bool isValid();
	private:
		vec3 moveUp(int m, vec3 np, Side s, vec3 d);
		vec3 moveLeft(int m, vec3 np, Side s, vec3 d);
		vec3 moveRight(int m, vec3 np, Side s, vec3 d);
};

class SnakeGame // SINGLETON
{
	Snake snake;
	Cube food;
	bool foodExist;
	bool extendSnake;
	int roundNumber;
	int boardSize;
	int score;
	bool failed;

	public:
        static SnakeGame& getInstance()
        {
            static SnakeGame instance; 
            return instance;
        }
		bool round();
		void move(Direction _direction)
		{
			snake.direction = _direction;
		}
		Cube getFood();
		Snake getSnake(){ return this->snake; }
		int getRound(){return this->roundNumber;}
		void initBoard(int _size)
		{
			boardSize = _size;
			this->snake.cubes.clear();
			this->foodExist = false;
			this->roundNumber = 0;
			this->score = 0;
			this->failed = false;
			this->snake.cubes.push_back(Cube(vec3(boardSize/2 + 1, 0, 0)));
			this->snake.cubes.push_back(Cube(vec3(boardSize/2 + 1, 0, -1)));
			this->snake.side = Side::pos_x;
			this->snake.direction = Direction::up;
		}
		int getBoardSize() { return this->boardSize; }
		void eatFood() { this->foodExist = false; }
    private:
        SnakeGame() {};
        SnakeGame(SnakeGame const&);
        void operator=(SnakeGame const&);
};

std::vector<Cube> Game();