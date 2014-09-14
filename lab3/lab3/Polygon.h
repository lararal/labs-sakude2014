#pragma once
#include "Entity.h"
#include <vector>

class Polygon : public Entity
{
public:
	std::vector<point> vertices;

	Polygon();
	void AddVertex(point);
	void Draw();
	~Polygon();
};
