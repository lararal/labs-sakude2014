#include "stdafx.h"
#include "Polygon.h"

using namespace PolygonNamespace;

Polygon::Polygon(DrawerAdapter adapter) : Entity(adapter)
{
}

void Polygon::AddVertex(point vd){
	point v;
	adapter.DeviceToNormalized(vd.x, vd.y, &(v.x), &(v.y));
	adapter.InverseViewingTransformation(&(v.x), &(v.y));
	vertices.push_back(v);
}

void Polygon::SetFillMethod(int fm)
{
	this->fillMethod = fm;
}

void Polygon::Draw(){

	float x1, y1, x2, y2;
	int xi1, yi1, xi2, yi2;
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

		//adapter.DrawPixel(vertices.at(i).x, vertices.at(i).y);
		//adapter.DDA(vertices.at(i).x, vertices.at(i).y, vertices.at(i + 1).x, vertices.at(i + 1).y);


	}
	//adapter.DrawPixel(vertices.at(vertices.size() - 1).x, vertices.at(vertices.size() - 1).y);
	//adapter.DDA(vertices.at(0).x, vertices.at(0).y, vertices.at(vertices.size() - 1).x, vertices.at(vertices.size() - 1).y);
}

void Polygon::Fill(){
	/*polygon_type polygon;
	edge_list_type list;
	polygon.n = 0;
	
	for (unsigned int i = 0; i < vertices.size(); i++){
		adapter.InsertVertex(polygon, vertices.at(i).x, vertices.at(i).y);
	}

	if (this->fillMethod == FILL_METHOD_SCAN) adapter.FillPolygon(polygon, list);
	else if (this->fillMethod == FILL_METHOD_FLOOD) adapter.FloodFillIterative(polygon, color);*/
}

Polygon::~Polygon()
{
}