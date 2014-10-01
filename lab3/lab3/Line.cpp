#include "stdafx.h"
#include "Line.h"


Line::Line(DrawerAdapter adapter, int xd1, int yd1, int xd2, int yd2) : Entity(adapter)
{
	float x1, y1, x2, y2;
	adapter.DeviceToNormalized(xd1, yd1, &x1, &y1);
	adapter.DeviceToNormalized(xd2, yd2, &x2, &y2);
	adapter.InverseViewingTransformation(&x1, &y1);
	adapter.InverseViewingTransformation(&x2, &y2);
	this->p1.x = x1;
	this->p1.y = y1;
	this->p2.x = x2;
	this->p2.y = y2;
}

void Line::Draw() {
	float x1, y1, x2, y2;
	
	x1 = p1.x;
	y1 = p1.y;
	x2 = p2.x;
	y2 = p2.y;
	
	if (adapter.Clip2D(&x1, &y1, &x2, &y2))
	{
		int xi1, yi1, xi2, yi2;
		adapter.ViewingTransformation(&x1, &y1);
		adapter.ViewingTransformation(&x2, &y2);
		adapter.NormalizedToDevice(x1, y1, &xi1, &yi1);
		adapter.NormalizedToDevice(x2, y2, &xi2, &yi2);
		adapter.DDA(xi1, yi1, xi2, yi2);
	}
}

void Line::Fill() {
	
}

Line::~Line()
{
}
