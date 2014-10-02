#include "stdafx.h"
#include "Polygon.h"

using namespace PolygonNamespace;

Polygon::Polygon(DrawerAdapter adapter) : Entity(adapter)
{
}

void Polygon::AddVertex(point v){
	vertices.push_back(v);
}

void Polygon::SetFillMethod(int fm)
{
	this->fillMethod = fm;
}

void Polygon::Draw(){

	for (unsigned int i = 0; i < vertices.size() - 1; i++){
		adapter.DrawPixel(vertices.at(i).x, vertices.at(i).y);
		adapter.DDA(vertices.at(i).x, vertices.at(i).y, vertices.at(i + 1).x, vertices.at(i + 1).y);
	}
	adapter.DrawPixel(vertices.at(vertices.size() - 1).x, vertices.at(vertices.size() - 1).y);
	adapter.DDA(vertices.at(0).x, vertices.at(0).y, vertices.at(vertices.size() - 1).x, vertices.at(vertices.size() - 1).y);
}

void Polygon::Fill(){
	polygon_type polygon;
	edge_list_type list;
	polygon.n = 0;
	
	for (unsigned int i = 0; i < vertices.size(); i++){
		adapter.InsertVertex(polygon, vertices.at(i).x, vertices.at(i).y);
	}

	if (this->fillMethod == FILL_METHOD_SCAN) adapter.FillPolygon(polygon, list);
	else if (this->fillMethod == FILL_METHOD_FLOOD) adapter.FloodFillIterative(polygon, color);
}

bool Polygon::Pick(int x, int y, float d){
	return false;
}

Polygon::~Polygon()
{
}