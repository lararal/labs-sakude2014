// lab02v2.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "DrawerAdapter.h"
#include "Circle.h"
#include "Polygon.h"
#include "Line.h"
#include <vector>


typedef enum {
	POLY_SCAN, POLY_FLOOD, CIRCLE, LINE
} my_shape;

void main()
{	
	DrawerAdapter adapter = DrawerAdapter();
	int p0_x, p0_y, p1_x, p1_y, menu_it = 0, color = adapter.MY_WHITE;
	float vxs = 0, vxh = 0.5f, vys = 0, vyh = 0.5f;
	adapter.InitGraphics();

	polygon_type polygon;
	polygon.n = 0;
	my_shape shape = POLY_SCAN;

	std::vector<Entity*> entities;

	adapter.menu_item = 0;
	CheckMenuItem(adapter.menu_color, 1, MF_CHECKED);
	CheckMenuItem(adapter.menu_draw, 21, MF_CHECKED);
	CheckMenuItem(adapter.menu_draw, 30, MF_CHECKED);
	while (adapter.key_input != ESC) {	// ESC exits the program
		adapter.CheckGraphicsMsg();
		if (adapter.key_input == ENTER)	 // Identify Enter
		{
			if (strlen(adapter.buffer) >= 2) {
				if (adapter.buffer[0] == 'x' && adapter.buffer[1] == '1'){
					vxs = atof(&(adapter.buffer[2]));
				}
				else if (adapter.buffer[0] == 'x' && adapter.buffer[1] == '2'){
					vxh = atof(&(adapter.buffer[2]));
				}
				else if (adapter.buffer[0] == 'y' && adapter.buffer[1] == '1'){
					vys = atof(&(adapter.buffer[2]));
				}
				else if (adapter.buffer[0] == 'y' && adapter.buffer[1] == '2'){
					vyh = atof(&(adapter.buffer[2]));
				}
				else if (adapter.buffer[0] == 'w' && adapter.buffer[1] == 'i' && adapter.buffer[2] == 'n'){
					adapter.SetViewport(vxs, vxh, vys, vyh);
					adapter.ClearGraphicsScreen();
					for (int i = 0; i <= entities.size() - 1; i++) {
						entities.at(i)->Draw();
						entities.at(i)->Fill();
					}
				}
			}
			adapter.ClearString(adapter.buffer);
			adapter.key_input = -1;
		}
		if (menu_it != adapter.menu_item)
			switch (adapter.menu_item){
			case 21:{
						CheckMenuItem(adapter.menu_draw, 21, MF_CHECKED);
						CheckMenuItem(adapter.menu_draw, 22, MF_UNCHECKED);
						CheckMenuItem(adapter.menu_draw, 23, MF_UNCHECKED);
						CheckMenuItem(adapter.menu_draw, 24, MF_UNCHECKED);
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
					   CheckMenuItem(adapter.menu_draw, 24, MF_UNCHECKED);
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
					   CheckMenuItem(adapter.menu_draw, 24, MF_UNCHECKED);

					   menu_it = adapter.menu_item;
					   shape = CIRCLE;
					   break;
			}
			case 24:
			{
				CheckMenuItem(adapter.menu_draw, 21, MF_UNCHECKED);
				CheckMenuItem(adapter.menu_draw, 22, MF_UNCHECKED);
				CheckMenuItem(adapter.menu_draw, 23, MF_UNCHECKED);
				CheckMenuItem(adapter.menu_draw, 24, MF_CHECKED);
				
				menu_it = adapter.menu_item;
				shape = LINE;
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
				}
				CheckMenuItem(adapter.menu_pattern, adapter.menu_item, MF_CHECKED);
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
			case LINE:
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
				case LINE:
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
			if (shape == POLY_FLOOD || shape == POLY_SCAN) {
				adapter.DrawXorLine(p0_x, p0_y, p1_x, p1_y);
				adapter.DDA(p0_x, p0_y, p1_x, p1_y);
				p0_x = p1_x = adapter.mouse_x;
				p0_y = p1_y = adapter.mouse_y;

				if (polygon.n > 0 &&
					(polygon.vertex[polygon.n - 1].x != p0_x
					|| polygon.vertex[polygon.n - 1].y != p0_y))
					adapter.InsertVertex(polygon, p0_x, p0_y);
			}
			else if (shape == CIRCLE) {
				adapter.CircleXorBresenham(p0_x, p0_y, adapter.distance(p0_x, p0_y, p1_x, p1_y));
				p1_x = adapter.mouse_x;
				p1_y = adapter.mouse_y;
				Circle circle_test = Circle(adapter, p0_x, p0_y, adapter.distance(p0_x, p0_y, p1_x, p1_y));
				circle_test.color = color;
				circle_test.Draw();
			}
			else if (shape == LINE) {
				adapter.DrawXorLine(p0_x, p0_y, p1_x, p1_y);
				Line* line = new Line(adapter, p0_x, p0_y, p1_x, p1_y);
				line->Draw();
				entities.push_back(line);
			}
			adapter.mouse_action = NO_ACTION;
		}
		else if (adapter.mouse_action == R_MOUSE_DOWN)
		{
			if (shape == POLY_FLOOD || shape == POLY_SCAN)
			{
				adapter.DDA(polygon.vertex[0].x, polygon.vertex[0].y, polygon.vertex[polygon.n - 1].x, polygon.vertex[polygon.n - 1].y);
				
				PolygonNamespace::Polygon poly_test = PolygonNamespace::Polygon(adapter);
				for (int i = 0; i < polygon.n; i++) {
					adapter.DrawPixel(polygon.vertex[i].x, polygon.vertex[i].y);
					Entity::point point;
					point.x = polygon.vertex[i].x;
					point.y = polygon.vertex[i].y;
					poly_test.AddVertex(point);
				}
				poly_test.color = color;

				if (shape == POLY_SCAN) poly_test.SetFillMethod(PolygonNamespace::Polygon::FILL_METHOD_SCAN);
				else if (shape == POLY_FLOOD) poly_test.SetFillMethod(PolygonNamespace::Polygon::FILL_METHOD_FLOOD);

				poly_test.Fill();

				polygon.n = 0;
			}
			else if (shape == CIRCLE) {
				Circle circle_test2 = Circle(adapter, p0_x, p0_y, adapter.distance(p0_x, p0_y, p1_x, p1_y));
				circle_test2.color = color;
				circle_test2.Fill();
			}
			adapter.mouse_action = NO_ACTION;
		}
	}
	adapter.CloseGraphics();
}
