void PlotCircle(int xc, int yc, int x, int y)
{
    Plot(xc+x,yc+y);  // first quarter (+x,+y)
    Plot(xc-x,yc+y);  // second quarter (-x,+y)
	Plot(xc-x,yc-y);  // third quarter (-x,-y)
	Plot(xc+x,yc-y);  // fourth quarter (+x,-y)

}

void EllipseBresenham(int xc,int yc,int a, int b)

{  int x, y, d1, d2,aa=a*a, bb=b*b,t;
     x=a; y=0;
	 d1=2*aa-2*a*b+bb/2;
	 d2=aa/2-4*a*bb+2*aa;
     while (d2<0)   // Draw a quarter of arc of ellipse in counterclockwise
	 {
		PlotCircle(xc,yc,x,y); // Plot 4 symetrical circle points
        if (d1<0)		 
		{  y++;  // Vertical move
		   t=4*aa*y;
		   d1+=t+2*aa;
		   d2+=t;	
		}
		else { // Diagonal Move
			x--;
		    y++;
			t=-4*bb*x+4*aa*y;
			d1+=t+2*aa;
			d2+=t+2*bb;
		}
	 }
	while (x>=0)
	{ PlotCircle(xc,yc,x,y); // Plot 4 symetrical circle points
	  if (d2<0)
	  {  x--;  // Diagonal Move
	     y++;
		 d2+=-4*bb*x+4*aa*y+2*bb; 
	  }
	  else {  // Horizontal Move
		x--;
		d2+=-4*bb*x+2*bb;
	  }
	}
}
