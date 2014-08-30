
void DrawXorPixel(int x, int y)
{ 
unsigned int mask=0x00FFFF;
  
    COLORREF cor=GetPixel(hdc,x,y);
 
    cor^=mask; // bit-bit Xor operation mask with color
    SetPixel(hdc,x,y,cor);
}
void DrawXorLine(int x1, int y1, int x2, int y2)
{  int i,length; float  x,y,dx,dy;

     length=Max(abs(x2-x1),abs(y2-y1));
	   
	 if (length>0)
	 {
      dx=((float)(x2-x1))/length;
      dy=((float)(y2-y1))/length;
      x=(float)x1; y=(float)y1;
     for (i=0; i<=length; i++)
     {
       DrawXorPixel(Arred(x),Arred(y));
	   x=x+ dx;    // dx = 1 ou -1 ou m
	   y=y+ dy;   // yx = 1 ou -1 ou 1/m
   }
   }
}