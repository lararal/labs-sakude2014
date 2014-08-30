
void main()
{  
	int p0_x, p0_y, p1_x,p1_y,cor,color=MY_MAGENTA;
  InitGraphics(); 
  while (key_input!=ESC) {	// ESC exits the program
	 CheckGraphicsMsg();
 	  if (mouse_action==L_MOUSE_DOWN) {  	// Pick first point 
 		 p0_x=p1_x=mouse_x; p0_y=p1_y=mouse_y;
	//	 mouse_action=NO_ACTION;
		}  
    	 	if (mouse_action==L_MOUSE_MOVE_DOWN){//Mouse moving left button
		 if (p1_x!=mouse_x || p1_y!=mouse_y)  	// test if x or y changed
		 {		// Erase previous line. 
	  	 SetGraphicsColor((int)MY_BLACK,1);
		DrawLine(p0_x,p0_y,p1_x,p1_y); // It can be improved using XOR line
		p1_x=mouse_x;p1_y=mouse_y;  
		 SetGraphicsColor((int)MY_LIGHTGREEN,1);
		DrawLine(p0_x,p0_y,p1_x,p1_y); // Draw new line
		//mouse_action=NO_ACTION;
		 }
			}
	    if (mouse_action==L_MOUSE_UP)
		 {	SetGraphicsColor((int)color,2);
			DrawLine(p0_x,p0_y,p1_x,p1_y); // Draw  final line
			mouse_action=NO_ACTION;
 		}
  	  }
  CloseGraphics();
}


