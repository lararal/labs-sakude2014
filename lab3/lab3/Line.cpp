#include "stdafx.h"
#include "Line.h"


Line::Line(int x1, int y1, int x2, int y2)
{
	this->p1.x = x1;
	this->p1.y = y1;
	this->p2.x = x2;
	this->p2.y = y2;
}

Line::Line(point p1, point p2)
{
	this->p1 = p1;
	this->p2 = p2;
}

void Line::Draw(){
	//DDA(p1.x, p1.y, p2.x, p2.y);
}

Line::~Line()
{
}
