#include "stdafx.h"
#include "Circle.h"

Circle::Circle(DrawerAdapter adapter, int xci, int yci, int ri) : Entity(adapter)
{
	float x1, y1, x2, y2; 
	int ri2 = ri;
	adapter.DeviceToNormalized(xci, yci, &x1, &y1);
	adapter.DeviceToNormalized(xci, yci + ri, &x2, &y2);
	adapter.InverseViewingTransformation(&x1, &y1);
	adapter.InverseViewingTransformation(&x2, &y2);
	this->pc.x = x1;
	this->pc.y = y1;
	this->r = adapter.distance(x1, y1, x2, y2);
}

//Circle::Circle(DrawerAdapter adapter, point pc, int r) : Entity(adapter)
//{
//	this->pc = pc;
//	this->r = r;
//}


void Circle::Draw() {
	int xi, yi, xi1, yi1, xi2, yi2;
	float xc, yc, x1, y1, x2, y2;
	xc = pc.x; yc = pc.y;
	x1 = pc.x; y1 = pc.y + r;
	x2 = pc.x + r; y2 = pc.y;

	adapter.ViewingTransformation(&xc, &yc);
	adapter.ViewingTransformation(&x1, &y1);
	adapter.ViewingTransformation(&x2, &y2);
	adapter.NormalizedToDevice(xc, yc, &xi, &yi);
	adapter.NormalizedToDevice(x1, y1, &xi1, &yi1);
	adapter.NormalizedToDevice(x2, y2, &xi2, &yi2);
	adapter.DrawEllipse(xi, yi, adapter.distance(xi, yi, xi2, yi2), adapter.distance(xi, yi, xi1, yi1), 1);
	//adapter.CircleBresenham(pc.x, pc.y, r);
}

void Circle::Fill() {
	//adapter.FillCircle(pc.x, pc.y, color);
}

Circle::~Circle()
{
}