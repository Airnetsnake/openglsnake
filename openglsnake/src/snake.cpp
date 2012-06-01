#include "snake.h"


//everything about our snake

snake::snake(int x, int y, int z):
	snakeLength(1),
		head(0),
		last(0)
{
	crdXYZ speed;
	
	speed.x = x;
	speed.y = y;
	speed.z = z;

	head = new snakeCell;
	head->prev = 0;
	head->next = 0;

	head->move = speed;
	last = head;
}

void snake::add()  //add length
{
	last -> next = new snakeCell;
	last->next->prev = last;
	last = last->next;
	last->move = last->prev->move;
	last->next = 0;

	snakeLength++;
}

void snake::move(int x, int y, int z) //how it moves
{
	snakeCell *cell = last;
	
	head->move.x = x;
	head->move.y = y;
	head->move.z = z;

	while(cell != head) 
	{
		cell -> move = cell->prev->move;
		cell = cell->prev;
	}

}


void snake::move(txyz direction)  //where to move
{
	move(direction[_IX], direction[_IY], direction[_IZ]);
}

void snake::outputSnake() 
{
	snakeCell *cell = head;
	int i = 0;

	do
	{
		std::cout<<i<<": "<<cell->move.x<<" "<<cell->move.y<<" "<<cell->move.z<<std::endl;		
		cell = cell->next;		
		i++;
	} while(cell != 0);

	std::cout<<std::endl;
}

snake::~snake(void)
{
	snakeCell *cell = head;
	snakeCell *temp = 0;

	do
	{
		cell = cell->next;
		temp = cell;
		delete temp;	
	} while(cell != 0);

}
