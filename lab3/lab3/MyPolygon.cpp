#include "stdafx.h"
#include "MyPolygon.h"
#include "PolyFill.h"
#include "Line.h"

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

	ClipPolygon();

	float x1, y1, x2, y2;
	int xi1, yi1, xi2, yi2;
	int xi0, yi0, xin, yin;
	adapter.SetGraphicsColor(color, adapter.GetMaxX());
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

	if (this->fillMethod == FILL_METHOD_EMPTY) return;

	polygon_type polygon;
	edge_list_type list;
	polygon.n = 0;
	int xi, yi;
	float x, y;
	adapter.SetGraphicsColor(color, adapter.GetMaxX());
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
	for (unsigned int i = 0; i < vertices.size() - 1; i++){
		Line line = Line(adapter, vertices.at(i), vertices.at(i + 1));
		if (line.Pick(x, y, d)) return true;
	}
	Line closeLine = Line(adapter, vertices.at(0), vertices.at(vertices.size() - 1));
	if (closeLine.Pick(x, y, d)) return true;

	return (this->fillMethod != FILL_METHOD_EMPTY) && PointInPoly(x, y);
}

bool MyPolygon::PointInPoly(float x, float y)
{
	float x1, y1;
	adapter.DeviceToNormalized(x, y, &x1, &y1);
	adapter.InverseViewingTransformation(&x1, &y1);
	int i, j, c = 0;
	for (i = 0, j = vertices.size() - 1; i < vertices.size(); j = i++) {
		if (((vertices[i].y > y1) != (vertices[j].y > y1)) &&
			(x1 < (vertices[j].x - vertices[i].x) * (y1 - vertices[i].y) / (vertices[j].y - vertices[i].y) + vertices[i].x))
			c = !c;
	}
	return c;
}

void MyPolygon::ClipPolygon(){
	DrawerAdapter::win_edge_type edge;
	float_polygon_type poly_out;
	for (int edg = 0; edg < 4; edg++){
		poly_out.n = 0; // Reset poly_out	
		edge = (DrawerAdapter::win_edge_type)edg;
		//poly.vertex[poly.n] = poly.vertex[0];
		vertices.push_back(vertices.at(0));
		for (int i = 0; i < vertices.size() -1; i++){
			float_point_type p1, p2;
			p1.x = vertices.at(i).x;
			p1.y = vertices.at(i).y;
			p2.x = vertices.at(i + 1).x;
			p2.y = vertices.at(i + 1).y;
			adapter.ClipEdge(p1, p2, edge, poly_out);
		}
		clean();
		for (int i = 0; i < poly_out.n; i++){
			Entity::point p;
			p.x = poly_out.vertex[i].x;
			p.y = poly_out.vertex[i].y;
			vertices.push_back(p);
		}
		//poly = poly_out;// Copy poly_out to poly
	}
}

MyPolygon::~MyPolygon()
{
}