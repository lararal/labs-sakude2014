#pragma once
#include "Entity.h"
class Circle :
	public Entity
{
public:
	point pc;
	float r;
	Circle(DrawerAdapter, int, int, int);
	Circle(DrawerAdapter, point, int);
	virtual void Draw();
	virtual void Fill();
	~Circle();
};

