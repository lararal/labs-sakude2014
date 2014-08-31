#include "stdafx.h"
#include <windows.h>
#include "grafbase.h"
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <vector>
#include "G.h"

void main()
{
	int p0_x = 0;
	int p0_y = 0;
	int p1_x = 0;
	int p1_y = 0;
	int cor, padrao, color = MY_MAGENTA;
	char command[4];
	
	G::InitGraphics();
	while (true) {
		G::CheckGraphicsMsg();
		
		if (G::mouse_action == L_MOUSE_DOWN) {  	// Pick first point 
			p0_x = p1_x = G::mouse_x; p0_y = p1_y = G::mouse_y;
			//	 mouse_action=NO_ACTION;
		}
		if (G::mouse_action == L_MOUSE_MOVE_DOWN){//Mouse moving left button
			if (p1_x != G::mouse_x || p1_y != G::mouse_y)  	// test if x or y changed
			{	// Erase previous line. 

				G::SetGraphicsColor((int)MY_LIGHTGREEN, 1);
				G::Draw(p0_x, p0_y, p1_x, p1_y);
				p1_x = G::mouse_x; p1_y = G::mouse_y;
				G::Draw(p0_x, p0_y, p1_x, p1_y);
			}
		}
		if (G::mouse_action == L_MOUSE_UP)
		{
			G::Draw(p0_x, p0_y, p1_x, p1_y); //Erase the last xor line 
			G::SetGraphicsColor((int)color, 2);
			G::Draw(p0_x, p0_y, p1_x, p1_y); // Draw  final line
			G::mouse_action = NO_ACTION;
		}
	}
	G::CloseGraphics();
}


