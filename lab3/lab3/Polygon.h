#pragma once
#include "Entity.h"
#include <vector>

namespace PolygonNamespace
{
	class Polygon : public Entity
	{
	private:
		int fillMethod = 0;
	public:
		const static int FILL_METHOD_SCAN = 0;
		const static int FILL_METHOD_FLOOD = 1;

		std::vector<point> vertices;
		Polygon(DrawerAdapter adapter);

		void AddVertex(point);
		void SetFillMethod(int);
		virtual void Draw();
		virtual void Fill();
		bool Pick(int, int, float);
		~Polygon();
	};
}