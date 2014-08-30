
#include "stdafx.h"



#include <windows.h>
#include "grafbase.h"
#include <stdio.h>
#include <string.h>
#include <math.h>


#define PI 3.1415926535897932384626433832795
#define ENTER 13
#define BACKSPACE 8
#define ESC 27
#define NO_ACTION -1
#define L_MOUSE_MOVE_DOWN 0
#define L_MOUSE_DOWN 1
#define R_MOUSE_DOWN 2
#define L_DOUBLE_CLICK 3
#define L_MOUSE_UP 4

#define CONSOLE_SIZE_X 640  // initial input line size in pixel
#define START_TEXT_X 5   // input line box initial position X
#define END_TEXT_X (CONSOLE_SIZE_X-2) // size of input line box in X
static SHORT numXpixels = 500;//640;			// size of window in X
static SHORT numYpixels = 500;//480             // size of window in Y
int start_text_y=numYpixels-20; 
int end_text_y = 21+start_text_y;					// size of input line in Y

int key_input=NO_ACTION,mouse_action=NO_ACTION;
 int mouse_x, mouse_y;
static BOOL graphics = TRUE;                /* Boolean, enable graphics?  */


//static short draw_color = MY_WHITE;        /* Current drawing color.     */
char buffer[200]="";					// string buffer for keyboard input


static COLORREF win_draw_color =RGB(255,255,255);  // current draw color
static HBRUSH blackBrush;
HDC  hdc;                                   /* Presentation Space Handle */
HWND WinHandle = NULL;                /* Client area window handle */
/* Foreground colors default to standard EGA palette.                */
/* No map is necessary unless a non-standard palette is implemented. */
static  HPEN hpen;
// 16 Predefined colors
typedef enum { MY_BLACK,MY_BLUE,MY_GREEN,MY_CYAN,MY_RED,MY_MAGENTA,
MY_BROWN,MY_LIGHTGRAY,MY_DARKGRAY,MY_LIGHTBLUE,MY_LIGHTGREEN,
MY_LIGHTCYAN,MY_LIGHTRED,MY_LIGHTMAGENTA,MY_YELLOW,MY_WHITE} my_color;
/* Define RGB color settings for MY enumerated colors */
static COLORREF color_trans_map[] =
{
	RGB(0,0,0),//MY_BLACK 
    RGB(0,0,255),//MY_BLUE,
    RGB(0,127,0),//MY_GREEN,
    RGB(0,233,233),//MY_CYAN,
    RGB(255,0,0),//MY_RED,
    RGB(255,0,255),//MY_MAGENTA,
    RGB(153,51,0),//MY_BROWN,
    RGB(175,175,175),//MY_LIGHTGRAY,
    RGB(70,70,70),//MY_DARKGRAY,
    RGB(51,51,255),//MY_LIGHTBLUE,
    RGB(0,255,0),//MY_LIGHTGREEN,
    RGB(51,255,255),//MY_LIGHTCYAN,
    RGB(255,25,25),//MY_LIGHTRED,
    RGB(255,65,255),//MY_LIGHTMAGENTA,
    RGB(255,255,0),//MY_YELLOW,
    RGB(255,255,255),//MY_WHITE,
 };
/****************************************************************************
*  Set the X dimension of the current window in pixels.                 *
****************************************************************************/
void SetMaxX(int maxX)
{
    numXpixels=maxX;
}
/****************************************************************************
*  Set the X dimension of the current window in pixels.                 *
****************************************************************************/
void SetMaxY(int maxY)
{
    numYpixels = maxY;
	start_text_y=numYpixels-20; 
	end_text_y = 21+start_text_y;
}

/****************************************************************************
*  Draws a pixel at the specified point on the screen.                      *
*  Caution!! GpiSetPel has been found to crash programs at some locations!  *
****************************************************************************/
void DrawPixel(int x, int y)
{
    
    SetPixel(hdc, x,y,win_draw_color);
}





/****************************************************************************
*                                                                           *
*  Name       :  InitGraphics()                                             *
*                                                                           *
*  Description:   Initializes the process for Window services               *
*  Concepts   : - obtains anchor block handle							    *
*  - creates the main frame window which creates the                        *
*  main client window                                                       *
*                                                                           *                                                                           *
*                                                                           *
****************************************************************************/


 

wchar_t wind_class[]=L"Window Application";
wchar_t wind_name[]= L"Lab 01 CCI-36     ";
void InitGraphics()
{
 
 HINSTANCE hInst=NULL;
 HWND hWnd; 
 WNDCLASS    wc;
  LPCWSTR window_class=(LPCWSTR) wind_class;
 //
 LPCWSTR window_name=(LPCWSTR) wind_name;
		// Fill up window structure 
 
        wc.lpszClassName =  window_class;  // registration name
        wc.hInstance = hInst;				// application instance
        wc.lpfnWndProc =  (WNDPROC) WinProc; // event handling function
        wc.hCursor = LoadCursor(NULL, IDC_ARROW); // cursor type
        wc.hIcon = NULL;
        wc.lpszMenuName = NULL;						// menu, if any
        wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH); // background color
        wc.style = CS_HREDRAW|CS_VREDRAW;		// window style
        wc.cbClsExtra = 0;
        wc.cbWndExtra = 0;
   
    /* Registeer window class      */
//GlobalAddAtom(window_class);

	  if(!RegisterClass(&wc))
	  {
        printf(" Error in RegisterClass...\n");
		exit(1);
	  }

		// Create window
      hWnd = CreateWindow(
            window_class,           // Desktop window class name             
            window_name,       // window name                 
             WS_OVERLAPPEDWINDOW | WS_VISIBLE,  // Window class style                  
            0, 0,                //window  top, left corner(origin)
            500, 500 ,                   // window X,Y size                                    
            (HWND)NULL,                   // Parent window         /
            (HMENU)NULL,				// handle to menu 
			(HINSTANCE) hInst,			// handle to application instance 
			(LPVOID)NULL);  //  pointer to window-creation data  

    if ((hWnd == NULL))
    {
   
        printf("error in CreateWindow ...\n ");
		exit(1);

    }

		// Sets the visibility state of window 
		ShowWindow(hWnd,SW_SHOW);

		// store window handle device context 
		WinHandle = hWnd;
        hdc = GetDC(WinHandle);
		// set hpen, blackbrush for clearing window, color for text and text background
		hpen = CreatePen(PS_SOLID, 1, win_draw_color);
		SelectObject(hdc, hpen);
		blackBrush = (HBRUSH) GetStockObject(BLACK_BRUSH);
       	SetBkColor(hdc,RGB(0,0,0));
    	SetTextColor(hdc,RGB(255,255,255));
    
}


/****************************************************************************
*  Reset display to default mode.                                           *
****************************************************************************/
void CloseGraphics(void)
{
                              
 // Delete pen and destroy window
    DeleteObject(hpen);
    ReleaseDC(WinHandle,hdc);
    DestroyWindow(WinHandle);          /* Tidy up...                 */
 }




/****************************************************************************
*  Returns the X dimension of the current window in pixels.                 *
****************************************************************************/
int GetMaxX(void)
{
    return numXpixels;
}

/****************************************************************************
*  Returns the Y dimension of the current window in pixels.                 *
****************************************************************************/
int GetMaxY(void)
{
    return numYpixels;
}


/****************************************************************************
*  Set current graphics drawing color.                                      *
****************************************************************************/
void SetGraphicsColor(int new_color, int width)
{  
	HPEN hpenOld;
    if (graphics)
    {  
				// test to avoid unnecessay pen changing
		if (win_draw_color!=color_trans_map[new_color])
		{
		// get COLORREF from defined palette
	    win_draw_color = color_trans_map[new_color];

		// create a pen with that color 
		hpen = CreatePen(PS_SOLID, width, win_draw_color);
		hpenOld = (HPEN)SelectObject(hdc, hpen);
		//   delete old pen
		DeleteObject(hpenOld);
		}
	  }

}




/****************************************************************************
*  Returns the color value of the pixel at the specified point on the       *
*  screen.                                                                  *
****************************************************************************/
int GetPixel(int x, int y)
{
    return (int)GetPixel(hdc, x,y);
}



void CheckGraphicsMsg(void)
{  MSG msg;
        /*Peek Message from message queue */
    if (PeekMessage(&msg, WinHandle, 0L, 0L, PM_REMOVE))
//  Translate keyboard hit to virtual key code and send to message queue
	{   TranslateMessage(&msg);
        DispatchMessage(&msg);
	}

}
void ClearString(char *str)
{ str[0]=0x00;
}
void EraseMessage()
{
   RECT rec={START_TEXT_X,start_text_y,
				   END_TEXT_X,end_text_y}; 
   HBRUSH backgrdBrush= ( HBRUSH )GetStockObject(BLACK_BRUSH);
				// Clear input input box
       FillRect(hdc,&rec,backgrdBrush);
}
void  PrintMessage(char *buffer)
{
		// Write input text in the graphics window
		// Input line is in the upper portion of the graphics window
	  if (strlen(buffer)>0)
		TextOutA(hdc,START_TEXT_X,start_text_y,(LPCSTR)buffer,strlen(buffer));

}


/****************************************************************************
*  Mouse Handler for Win 95                                                   *
****************************************************************************/
static LRESULT CALLBACK WinProc(HWND hWnd,UINT messg,WPARAM wParam,LPARAM lParam)
{
 
  PAINTSTRUCT ps;
 
  char str[3]=" ";
  switch (messg)
  {
	
  case WM_PAINT:
	  BeginPaint(hWnd,&ps);
	  PrintMessage(buffer);
			// Draw everything
//	  RefreshScreen();

	  ValidateRect(hWnd,NULL);

	  EndPaint(hWnd,&ps);
	  break;
  case WM_CHAR:
        {
			// Win32 keyboard message: lParam=key code wParam= virtual key (ASCII) code 
           
            if (!(LOWORD(wParam) & KF_UP) &&
                !(LOWORD(wParam) & KF_ALTDOWN )    )

            {
				//  take keyboard input
                key_input = (char)LOWORD(wParam);
            
            
            
			if (key_input==ENTER) // Enter
			{
			   EraseMessage();
			   //buffer[strlen(buffer)]=0x00;
			}
			else if (key_input==BACKSPACE) // BackSpace
			{   if (strlen(buffer)>0)
				{ int len=strlen(buffer)-1;
					// Clear last character in buffer
                 buffer[len]=' ';
					// Clear characters in input box
                 strcat_s(buffer,"   ");
				 PrintMessage(buffer);
				 buffer[len]=0x00; // put end string
				}
			}
			else if (key_input>31 && key_input<130)
			{ int leng=strlen(buffer);
			EraseMessage();
			   str[0]=key_input;
			   strcat_s(buffer,str); // add char
			   // display, update input box
			   PrintMessage(buffer);		
			}
			else if (key_input!=ESC) //ESC
					key_input = -1;
             break;
			}
        }
	case WM_SIZE:
				// resize 
		SetMaxX(LOWORD(lParam));  // width of client area 
		SetMaxY(HIWORD(lParam));            
        PostMessage(WinHandle, WM_PAINT, wParam, lParam);

	break;
	case WM_MOUSEMOVE:
        
        {     key_input = wParam;   
		     if (key_input==MK_LBUTTON)

			  {
			  EraseMessage();
              mouse_x = LOWORD(lParam);
              mouse_y = HIWORD(lParam);
			  key_input = wParam;  
              printf_s(buffer," x = %d y = %d",mouse_x,mouse_y);
              PrintMessage(buffer);
              mouse_action =  L_MOUSE_MOVE_DOWN;  
		       ClearString(buffer);
			  }
            break;
        }
   case WM_LBUTTONDOWN:
        
        {
           
           	  EraseMessage();
              mouse_x = LOWORD(lParam);
              mouse_y = HIWORD(lParam);
			  key_input = wParam;  
              printf_s(buffer," x = %d y = %d",mouse_x,mouse_y);
              PrintMessage(buffer);
              mouse_action =  L_MOUSE_DOWN;  
		       ClearString(buffer);
            break;
        }
case WM_LBUTTONUP:
        
        {
           
           	  EraseMessage();
              mouse_x = LOWORD(lParam);
              mouse_y = HIWORD(lParam);
			  key_input = wParam;  
              sprintf_s(buffer," x = %d y = %d",mouse_x,mouse_y);
              PrintMessage(buffer);
              mouse_action =  L_MOUSE_UP;  
		       ClearString(buffer);
            break;
        }
        
   case WM_RBUTTONDOWN:
        {    	   
			   EraseMessage();
               key_input = wParam;        
               mouse_x = LOWORD(lParam);
               mouse_y = HIWORD(lParam);
            
               sprintf_s(buffer," x = %d y = %d",mouse_x,mouse_y);
          	   PrintMessage(buffer);
              
               mouse_action =  R_MOUSE_DOWN; 
			    ClearString(buffer);
			
            break;
        }
   case WM_LBUTTONDBLCLK: 
	          EraseMessage();
              mouse_x = LOWORD(lParam);
              mouse_y = HIWORD(lParam);
			  key_input = wParam;  
              sprintf_s(buffer," x = %d y = %d",mouse_x,mouse_y);
              PrintMessage(buffer);
			  mouse_action =  L_DOUBLE_CLICK; 
		       ClearString(buffer);
           key_input = wParam;  
		   
            break; 
  case WM_DESTROY:
	  PostQuitMessage(0);
	  break;
  default: 
	  return(DefWindowProc(hWnd,messg,wParam,lParam));
	  break;
  }
  
  return 0;

}


void main()
{  int p0_x, p0_y, p1_x,p1_y,color=MY_MAGENTA;
  InitGraphics(); 
  while (key_input!=ESC) {	// ESC exits the program
	 CheckGraphicsMsg();
 	  // Insert your interactive main code here

  	  }
  CloseGraphics();
}


