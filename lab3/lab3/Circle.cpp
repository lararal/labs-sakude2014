#include "stdafx.h"
#include "Circle.h"

Circle::Circle(int xc, int yc, int r)
{
	this->pc.x = xc;
	this->pc.y = yc;
	this->r = r;
}

Circle::Circle(point pc, int r)
{
	this->pc = pc;
	this->r = r;
}

void Circle::Draw(DrawerAdapter adapter){
	adapter.CircleBresenham(pc.x, pc.y, r);
}

Circle::~Circle()
{
}