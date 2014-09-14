#include "stdafx.h"
#include "Polygon.h"


Polygon::Polygon()
{
}

void Polygon::AddVertex(point v){
	vertices.push_back(v);
}

void Polygon::Draw(){

	for (int i = 0; i < vertices.size() - 1; i++){
		//DDA(vertices.at(i).x, vertices.at(i).y, vertices.at(i + 1).x, vertices.at(i + 1).y);
	}
	//DDA(vertices.at(0).x, vertices.at(0).y, vertices.at(vertices.size() - 1).x, vertices.at(vertices.size() - 1).y);
}

Polygon::~Polygon()
{
}