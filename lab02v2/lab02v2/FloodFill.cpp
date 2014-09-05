bool Empty(int x,int y)
{  
   return(GetPixel(x,y)==0);

}

void FloodFillRec(int x,int y)
{
	if (Empty(x,y))
	{
		
			DrawPixel(x,y);
		
		if (Empty(x+1,y))
			FloodFillRec(x+1,y);
		if (Empty(x-1,y))
 			FloodFillRec(x-1,y);
		if (Empty(x,y+1))
			FloodFillRec(x,y+1);
		if (Empty(x,y-1))
			FloodFillRec(x,y-1);

	}
}

void FloodFillRecursive(polygon_type poly)
{  
    int x_seed=0,y_seed=0;
	for (int i=0; i<poly.n; i++)
	{
		x_seed+=poly.vertex[i].x;
		y_seed+=poly.vertex[i].y;
	}
	x_seed/=poly.n;
	y_seed/=poly.n;
    FloodFillRec(x_seed,y_seed);
}