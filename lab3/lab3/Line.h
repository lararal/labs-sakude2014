#pragma once
#include "Entity.h"
class Line :
	public Entity
{
public:
	point p1, p2; //world coordinates
	Line(int, int, int, int);
	Line(point, point);
	void Draw();
	~Line();
};