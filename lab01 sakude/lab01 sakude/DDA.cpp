#define Max(x,y) ((x>y)?x:y)
#define Arred(x) ((int)(x+0.5))   // only for x>=0

void DrawLine(int x1, int y1, int x2, int y2)
{  // i,length são inteiros   x,y,dx,dy são reais

     length=Max(abs(x2-x1),abs(y2-y1));
	   
	 if (length>0)
	 {
      dx=((float)(x2-x1))/length;
      dy=((float)(y2-y1))/length;
      x=(float)x1; y=(float)y1;
     for (i=0; i<=length; i++)
     {
       DrawPixel(Arred((int)x),Arred((int)y));
	   x=x+ dx;    // dx = 1 ou -1 ou m
	   y=y+ dy;   // yx = 1 ou -1 ou 1/m
   }
   }
}