// lab02v2.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "DrawerAdapter.h"
#include "Circle.h"

typedef enum {
	POLY_SCAN, POLY_FLOOD, CIRCLE
} my_shape;

void main()
{	
	DrawerAdapter adapter = DrawerAdapter();
	int p0_x, p0_y, p1_x, p1_y, menu_it = 0, color = adapter.MY_WHITE;
	adapter.InitGraphics();

	polygon_type polygon;
	polygon.n = 0;
	my_shape shape = POLY_SCAN;

	adapter.menu_item = 0;
	CheckMenuItem(adapter.menu_color, 1, MF_CHECKED);
	CheckMenuItem(adapter.menu_draw, 21, MF_CHECKED);
	CheckMenuItem(adapter.menu_draw, 30, MF_CHECKED);
	while (adapter.key_input != ESC) {	// ESC exits the program
		adapter.CheckGraphicsMsg();
		if (menu_it != adapter.menu_item)
			switch (adapter.menu_item){
			case 21:{
						CheckMenuItem(adapter.menu_draw, 21, MF_CHECKED);
						CheckMenuItem(adapter.menu_draw, 22, MF_UNCHECKED);
						CheckMenuItem(adapter.menu_draw, 23, MF_UNCHECKED);
						menu_it = adapter.menu_item;
						shape = POLY_SCAN;
						polygon.n = 0;
						break;
			}
			case 22:
			{
					   CheckMenuItem(adapter.menu_draw, 21, MF_UNCHECKED);
					   CheckMenuItem(adapter.menu_draw, 22, MF_CHECKED);
					   CheckMenuItem(adapter.menu_draw, 23, MF_UNCHECKED);
					   menu_it = adapter.menu_item;
					   shape = POLY_FLOOD;
					   polygon.n = 0;
					   break;
			}
			case 23:
			{
					   CheckMenuItem(adapter.menu_draw, 21, MF_UNCHECKED);
					   CheckMenuItem(adapter.menu_draw, 22, MF_UNCHECKED);
					   CheckMenuItem(adapter.menu_draw, 23, MF_CHECKED);

					   menu_it = adapter.menu_item;
					   shape = CIRCLE;
					   break;
			}
			case 30:
			case 31:
			case 32:
			case 33:
			case 34:
			case 35:
			case 36:
			case 37:
			case 38:
			case 39:
				for (int i = 30; i <= 39; i++){
					CheckMenuItem(adapter.menu_pattern, i, MF_UNCHECKED);
					CheckMenuItem(adapter.menu_pattern, adapter.menu_item, MF_CHECKED);
				}
				adapter.UpdatePattern(adapter.menu_item);
				menu_it = adapter.menu_item;
				break;
			default:
			{
					   for (int i = 1; i <= 16; i++)
						   CheckMenuItem(adapter.menu_color, i, MF_UNCHECKED);
					   CheckMenuItem(adapter.menu_color, adapter.menu_item, MF_CHECKED);
					   if (adapter.menu_item >= 1 && adapter.menu_item <= 16)
						   color = adapter.menu_item - 1;

					   menu_it = adapter.menu_item;

			}
		}
		adapter.SetGraphicsColor(color, adapter.numXpixels);

		if (adapter.mouse_action == L_MOUSE_DOWN)
		{
			switch (shape)
			{
			case POLY_FLOOD:
			case POLY_SCAN:
				// Pick first point up 
				if (polygon.n == 0)
				{
					p0_x = p1_x = adapter.mouse_x;
					p0_y = p1_y = adapter.mouse_y;
					adapter.InsertVertex(polygon, p0_x, p0_y);
				}
				break;
			case CIRCLE:
				p0_x = p1_x = adapter.mouse_x;
				p0_y = p1_y = adapter.mouse_y;
				break;
			}
		}
		if (adapter.mouse_action == L_MOUSE_MOVE_DOWN)
		{
			if (p1_x != adapter.mouse_x || p1_y != adapter.mouse_y)
			{
				switch (shape)
				{
				case POLY_FLOOD:
				case POLY_SCAN:
					// Draw new line
					adapter.DrawXorLine(p0_x, p0_y, p1_x, p1_y);
					p1_x = adapter.mouse_x;
					p1_y = adapter.mouse_y;
					adapter.DrawXorLine(p0_x, p0_y, p1_x, p1_y);
					break;
				case CIRCLE:
					adapter.CircleXorBresenham(p0_x, p0_y, adapter.distance(p0_x, p0_y, p1_x, p1_y));
					p1_x = adapter.mouse_x;
					p1_y = adapter.mouse_y;
					adapter.CircleXorBresenham(p0_x, p0_y, adapter.distance(p0_x, p0_y, p1_x, p1_y));
					break;
				}
			}
		}
		else  if (adapter.mouse_action == L_MOUSE_UP)
		{
			switch (shape)
			{
			case POLY_FLOOD:
			case POLY_SCAN:
				adapter.DrawXorLine(p0_x, p0_y, p1_x, p1_y);
				adapter.DDA(p0_x, p0_y, p1_x, p1_y);
				p0_x = p1_x = adapter.mouse_x;
				p0_y = p1_y = adapter.mouse_y;

				if (polygon.n > 0 &&
					(polygon.vertex[polygon.n - 1].x != p0_x
					|| polygon.vertex[polygon.n - 1].y != p0_y))
					adapter.InsertVertex(polygon, p0_x, p0_y);
				break;
			case CIRCLE:
				adapter.CircleXorBresenham(p0_x, p0_y, adapter.distance(p0_x, p0_y, p1_x, p1_y));
				p1_x = adapter.mouse_x;
				p1_y = adapter.mouse_y;
				//adapter.CircleBresenham(p0_x, p0_y, adapter.distance(p0_x, p0_y, p1_x, p1_y));
				Circle circle_test = Circle(p0_x, p0_y, adapter.distance(p0_x, p0_y, p1_x, p1_y));
				circle_test.Draw(adapter);
				break;
			}
			adapter.mouse_action = NO_ACTION;
		}
		else if (adapter.mouse_action == R_MOUSE_DOWN)
		{
			switch (shape)
			{
			case POLY_FLOOD:
			case POLY_SCAN:
				edge_list_type list;
				int num_Edges;
				adapter.DDA(polygon.vertex[0].x, polygon.vertex[0].y, polygon.vertex[polygon.n - 1].x, polygon.vertex[polygon.n - 1].y);
				for (int i = 0; i < polygon.n; i++) {
					adapter.DrawPixel(polygon.vertex[i].x, polygon.vertex[i].y);
				}
				if (shape == POLY_SCAN) adapter.FillPolygon(polygon, list);
				else if (shape == POLY_FLOOD) adapter.FloodFillIterative(polygon, color);
				polygon.n = 0;
				break;
			case CIRCLE:
				adapter.FillCircle(p0_x, p0_y, color);
				break;
			}
			adapter.mouse_action = NO_ACTION;
		}
	}
	adapter.CloseGraphics();
}
