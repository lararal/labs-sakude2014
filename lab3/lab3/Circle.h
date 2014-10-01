#pragma once
#include "Entity.h"
class Circle :
	public Entity
{
public:
	point pc;
	int r;
	Circle(DrawerAdapter, int, int, int);
	Circle(DrawerAdapter, point, int);
	virtual void Draw();
	void Fill();
	~Circle();
};

