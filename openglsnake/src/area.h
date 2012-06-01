#pragma once
#include "datatype.h"

class area
{

private:
	crdXYZ size;
	int ***map;

public:
	area(void);
	area(int x, int y, int z);

	const crdXYZ & getSize() {return size;}

	~area(void);
};

