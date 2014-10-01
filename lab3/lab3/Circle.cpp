#include "stdafx.h"
#include "Circle.h"

Circle::Circle(DrawerAdapter adapter, int xc, int yc, int r) : Entity(adapter)
{
	this->pc.x = xc;
	this->pc.y = yc;
	this->r = r;
}

Circle::Circle(DrawerAdapter adapter, point pc, int r) : Entity(adapter)
{
	this->pc = pc;
	this->r = r;
}

void Circle::Draw() {
	adapter.CircleBresenham(pc.x, pc.y, r);
}

void Circle::Fill() {
	adapter.FillCircle(pc.x, pc.y, color);
}

Circle::~Circle()
{
}