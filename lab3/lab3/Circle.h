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
	void Draw();
	void Fill();
	bool Pick(int, int, float);
	~Circle();
};

