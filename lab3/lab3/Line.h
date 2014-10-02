#pragma once
#include "Entity.h"
class Line :
	public Entity
{
public:
	point p1, p2; //world coordinates
	Line(DrawerAdapter adapter, int, int, int, int);
	virtual void Draw();
	virtual void Fill();
	bool Pick(int, int, float);
	~Line();
};