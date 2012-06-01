#pragma once
#include "datatype.h"
#include <iostream>

struct snakeCell
{
	snakeCell *next;
	snakeCell *prev;
	crdXYZ move;
};

class snake
{
private:
	snakeCell *head;
	snakeCell *last;

	int snakeLength;

public:
	//snake(void);
	snake(int x, int y, int z);
	//snake(crdXYZ startPos);
	~snake(void);
	void add();
	void move(txyz direction);
	void move(int x, int y, int z);

	const snakeCell *getHead() {return head;}
	const snakeCell *getNext(const snakeCell *cell){return cell->next;};

	int getLength() {return snakeLength;}


	void outputSnake();
};

