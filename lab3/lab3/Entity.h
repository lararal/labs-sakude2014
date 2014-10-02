#pragma once
#include "Matrix2D.h"
#include "DrawerAdapter.h"
class Entity
{
public:
	typedef struct {
		float x, y;
	} point;
	int color = 0;
	DrawerAdapter adapter;

	Entity(DrawerAdapter);
	~Entity();
	virtual void Draw() = 0;
	virtual void Fill() = 0;
	virtual bool Pick(int, int, float) = 0;
	void DoTransformation(Matrix2D);
};