#pragma once
#include "Matrix2D.h"
#include "DrawerAdapter.h"
class Entity
{
public:
	typedef struct {
		int x, y;
	} point;
	int color = 0;

	Entity();
	~Entity();
	virtual void Draw(DrawerAdapter) = 0;
	void DoTransformation(Matrix2D);
};