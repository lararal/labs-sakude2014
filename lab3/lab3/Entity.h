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
	virtual void Draw();
	virtual void Fill();
	void DoTransformation(Matrix2D);
};