#pragma once
#include "Entity.h"
class Circle :
	public Entity
{
public:
	point pc;
	int r;
	Circle(int, int, int);
	Circle(point, int);
	void Draw(DrawerAdapter);
	~Circle();
};

