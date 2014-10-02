#include "stdafx.h"
#include "MyPolygon.h"


MyPolygon::MyPolygon(DrawerAdapter adapter) : Entity(adapter)
{
}

void MyPolygon::AddVertex(point vd){
	point v;
	adapter.DeviceToNormalized(vd.x, vd.y, &(v.x), &(v.y));
	adapter.InverseViewingTransformation(&(v.x), &(v.y));
	vertices.push_back(v);
}

void MyPolygon::AddVertex(int xd, int yd){
	point v;
	adapter.DeviceToNormalized(xd, yd, &(v.x), &(v.y));
	adapter.InverseViewingTransformation(&(v.x), &(v.y));
	vertices.push_back(v);
}

void MyPolygon::SetFillMethod(int fm)
{
	this->fillMethod = fm;
}

int MyPolygon::length(){
	return vertices.size();
}

void MyPolygon::clean(){
	while (vertices.size() != 0)
	{
		vertices.pop_back();
	}
}

void MyPolygon::Draw(){

	float x1, y1, x2, y2;
	int xi1, yi1, xi2, yi2;
	int xi0, yi0, xin, yin;
	for (unsigned int i = 0; i < vertices.size() - 1; i++){

		x1 = vertices.at(i).x;
		y1 = vertices.at(i).y;
		x2 = vertices.at(i + 1).x;
		y2 = vertices.at(i + 1).y;

		adapter.ViewingTransformation(&x1, &y1);
		adapter.ViewingTransformation(&x2, &y2);
		adapter.NormalizedToDevice(x1, y1, &xi1, &yi1);
		adapter.NormalizedToDevice(x2, y2, &xi2, &yi2);
		adapter.DDA(xi1, yi1, xi2, yi2);

		adapter.DDA(xi1, yi1, xi2, yi2);
		adapter.DrawPixel(xi1, yi1);
		//daqui pra baixo
		//adapter.DrawPixel(vertices.at(i).x, vertices.at(i).y);
		//adapter.DDA(vertices.at(i).x, vertices.at(i).y, vertices.at(i + 1).x, vertices.at(i + 1).y);
		if (i == 0){
			xi0 = xi1;
			yi0 = yi1;
		}
		if (i == (vertices.size() - 2)){
			xin = xi2;
			yin = yi2;
		}
	}


	adapter.DrawPixel(xin, yin);
	//adapter.DrawPixel(vertices.at(vertices.size() - 1).x, vertices.at(vertices.size() - 1).y);
	adapter.DDA(xi0, yi0, xin, yin);
	//adapter.DDA(vertices.at(0).x, vertices.at(0).y, vertices.at(vertices.size() - 1).x, vertices.at(vertices.size() - 1).y);
}

void MyPolygon::Fill(){

	polygon_type polygon;
	edge_list_type list;
	polygon.n = 0;
	int xi, yi;
	float x, y;
	for (unsigned int i = 0; i < vertices.size(); i++){
		x = vertices.at(i).x;
		y = vertices.at(i).y;
		adapter.ViewingTransformation(&x, &y);
		adapter.NormalizedToDevice(x, y, &xi, &yi);
		adapter.InsertVertex(polygon, xi, yi);
	}

	if (this->fillMethod == FILL_METHOD_SCAN) adapter.FillPolygon(polygon, list);
	else if (this->fillMethod == FILL_METHOD_FLOOD) adapter.FloodFillIterative(polygon, color);
}

bool MyPolygon::Pick(int x, int y, float d)
{
	return false;
}


MyPolygon::~MyPolygon()
{
}