void PlotCircle(int xc, int yc, int x, int y)
{
    	Plot(xc+x,yc+y);  
    	Plot(xc+y,yc+x);  
	Plot(xc+y,yc-x);  
	Plot(xc+x,yc-y);  
	TO DO
}


void CircleBresenham(int xc,int yc,int r)

{  int x, y, d, deltaE,deltaSE;
     x=0; y=r;
	 d=1-r; deltaE=3; deltaSE=-2*r+5;
	
	 PlotCircle(xc,yc,x,y); // Plot 8 symetrical circle points

     while (y>x)   // Draw a quarter of circle in clockwise
	 {
	        if (d<0)		
		{	d+=deltaE;
			deltaE+=2;
			deltaSE+=2;
		}
		else {
			d+=deltaSE;
			deltaE+=2;
			deltaSE+=4;
			y--;
			}
		 x++;
	PlotCircle(xc,yc,x,y); // Plot 8 symetrical circle points
	 }


}

