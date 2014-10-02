#pragma once
#include "Entity.h"
#include <vector>

class MyPolygon : public Entity
{
private:
	int fillMethod = 0;
public:
	const static int FILL_METHOD_SCAN = 0;
	const static int FILL_METHOD_FLOOD = 1;
	const static int FILL_METHOD_EMPTY = 2;

	std::vector<point> vertices;
	MyPolygon(DrawerAdapter adapter);

	void AddVertex(point);
	void AddVertex(int, int);
	void SetFillMethod(int);
	int length(void);
	void clean(void);
	virtual void Draw();
	virtual void Fill();
	bool Pick(int, int, float);
	void ClipPolygon();
	bool PointInPoly(float x, float y);
	~MyPolygon();
};
