// lab02v2.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <windows.h>
#include "PolyFill.h"
#include "grafbase.h"
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <vector>
#include <queue>


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

#define Max(x,y) ((x>y)?x:y)
#define Arred(x) ((int)(x+0.5))   // only for x>=0

static SHORT numXpixels = 500;//640;			// size of window in X
static SHORT numYpixels = 500;//480             // size of window in Y
int start_text_y = numYpixels - 20;
int end_text_y = 21 + start_text_y;					// size of input line in Y

int key_input = NO_ACTION, mouse_action = NO_ACTION;
int mouse_x, mouse_y;
static BOOL graphics = TRUE;                /* Boolean, enable graphics?  */


//static short draw_color = MY_WHITE;        /* Current drawing color.     */
char buffer[200] = "";					// string buffer for keyboard input

std::vector<std::vector<int>> pattern = {
	{ 1, 1, 1, 1, 1, 1, 1, 1 },
	{ 1, 1, 1, 1, 1, 1, 1, 1 },
	{ 1, 1, 1, 1, 1, 1, 1, 1 },
	{ 1, 1, 1, 1, 1, 1, 1, 1 },
	{ 1, 1, 1, 1, 1, 1, 1, 1 },
	{ 1, 1, 1, 1, 1, 1, 1, 1 },
	{ 1, 1, 1, 1, 1, 1, 1, 1 },
	{ 1, 1, 1, 1, 1, 1, 1, 1 },
};


static COLORREF win_draw_color = RGB(255, 255, 255);  // current draw color
static HBRUSH blackBrush;
HDC  hdc;                                   /* Presentation Space Handle */
HWND WinHandle = NULL;                /* Client area window handle */
/* Foreground colors default to standard EGA palette.                */
/* No map is necessary unless a non-standard palette is implemented. */
static  HPEN hpen;
// 16 Predefined colors

int menu_item;


typedef enum {
	FULL, DOTTED, DASHED, DOTTED_DASHED
} my_pattern;

typedef enum {
	POLY_SCAN, POLY_FLOOD, CIRCLE
} my_shape;

typedef enum {
	MY_BLACK, MY_BLUE, MY_GREEN, MY_CYAN, MY_RED, MY_MAGENTA,
	MY_BROWN, MY_LIGHTGRAY, MY_DARKGRAY, MY_LIGHTBLUE, MY_LIGHTGREEN,
	MY_LIGHTCYAN, MY_LIGHTRED, MY_LIGHTMAGENTA, MY_YELLOW, MY_WHITE
} my_color;
/* Define RGB color settings for MY enumerated colors */
static COLORREF color_trans_map[] =
{
	RGB(0, 0, 0),//MY_BLACK 
	RGB(0, 0, 255),//MY_BLUE,
	RGB(0, 127, 0),//MY_GREEN,
	RGB(0, 233, 233),//MY_CYAN,
	RGB(255, 0, 0),//MY_RED,
	RGB(255, 0, 255),//MY_MAGENTA,
	RGB(153, 51, 0),//MY_BROWN,
	RGB(175, 175, 175),//MY_LIGHTGRAY,
	RGB(70, 70, 70),//MY_DARKGRAY,
	RGB(51, 51, 255),//MY_LIGHTBLUE,
	RGB(0, 255, 0),//MY_LIGHTGREEN,
	RGB(51, 255, 255),//MY_LIGHTCYAN,
	RGB(255, 25, 25),//MY_LIGHTRED,
	RGB(255, 65, 255),//MY_LIGHTMAGENTA,
	RGB(255, 255, 0),//MY_YELLOW,
	RGB(255, 255, 255),//MY_WHITE,
};


void UpdatePattern(int p){

	std::vector<std::vector<int>> pattern0 = {
		{ 1, 1, 1, 1, 1, 1, 1, 1 },
		{ 1, 1, 1, 1, 1, 1, 1, 1 },
		{ 1, 1, 1, 1, 1, 1, 1, 1 },
		{ 1, 1, 1, 1, 1, 1, 1, 1 },
		{ 1, 1, 1, 1, 1, 1, 1, 1 },
		{ 1, 1, 1, 1, 1, 1, 1, 1 },
		{ 1, 1, 1, 1, 1, 1, 1, 1 },
		{ 1, 1, 1, 1, 1, 1, 1, 1 },
	};
	std::vector<std::vector<int>> pattern1 = {
		{ 1, 1, 1, 1, 1, 1, 1, 1 },
		{ 0, 0, 0, 0, 1, 0, 0, 0 },
		{ 0, 0, 0, 0, 1, 0, 0, 0 },
		{ 0, 0, 0, 0, 1, 0, 0, 0 },
		{ 1, 1, 1, 1, 1, 1, 1, 1 },
		{ 1, 0, 0, 0, 0, 0, 0, 0 },
		{ 1, 0, 0, 0, 0, 0, 0, 0 },
		{ 1, 0, 0, 0, 0, 0, 0, 0 }
	};
	std::vector<std::vector<int>> pattern2 = {
		{ 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 1, 1, 1, 1, 1, 1, 1, 1 },
		{ 1, 1, 1, 1, 1, 1, 1, 1 },
		{ 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 1, 1, 1, 1, 1, 1, 1, 1 },
		{ 1, 1, 1, 1, 1, 1, 1, 1 }
	};
	std::vector<std::vector<int>> pattern3 = {
		{ 1, 1, 0, 0, 1, 1, 0, 0 },
		{ 1, 1, 0, 0, 1, 1, 0, 0 },
		{ 1, 1, 0, 0, 1, 1, 0, 0 },
		{ 1, 1, 0, 0, 1, 1, 0, 0 },
		{ 1, 1, 0, 0, 1, 1, 0, 0 },
		{ 1, 1, 0, 0, 1, 1, 0, 0 },
		{ 1, 1, 0, 0, 1, 1, 0, 0 },
		{ 1, 1, 0, 0, 1, 1, 0, 0 },
	};
	std::vector<std::vector<int>> pattern4 = {
		{ 1, 0, 1, 0, 1, 0, 1, 0 },
		{ 1, 0, 1, 0, 1, 0, 1, 0 },
		{ 1, 0, 1, 0, 1, 0, 1, 0 },
		{ 1, 0, 1, 0, 1, 0, 1, 0 },
		{ 1, 0, 1, 0, 1, 0, 1, 0 },
		{ 1, 0, 1, 0, 1, 0, 1, 0 },
		{ 1, 0, 1, 0, 1, 0, 1, 0 },
		{ 1, 0, 1, 0, 1, 0, 1, 0 },
	};
	std::vector<std::vector<int>> pattern5 = {
		{ 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 1, 1, 1, 1, 1, 1, 1, 1 },
		{ 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 1, 1, 1, 1, 1, 1, 1, 1 },
		{ 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 1, 1, 1, 1, 1, 1, 1, 1 },
		{ 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 1, 1, 1, 1, 1, 1, 1, 1 },
	};
	std::vector<std::vector<int>> pattern6 = {
		{ 0, 0, 0, 1, 0, 0, 0, 0 },
		{ 0, 0, 1, 1, 1, 0, 0, 0 },
		{ 0, 1, 1, 1, 1, 1, 0, 0 },
		{ 1, 1, 1, 1, 1, 1, 1, 0 },
		{ 0, 1, 1, 1, 1, 1, 0, 0 },
		{ 0, 0, 1, 1, 1, 0, 0, 0 },
		{ 0, 0, 0, 1, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0 },
	};
	std::vector<std::vector<int>> pattern7 = {
		{ 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 1, 1, 1, 1, 1, 1, 1, 0 },
		{ 1, 0, 0, 0, 0, 0, 1, 0 },
		{ 1, 0, 1, 0, 1, 0, 1, 0 },
		{ 1, 0, 0, 0, 0, 0, 1, 0 },
		{ 1, 0, 1, 1, 1, 0, 1, 0 },
		{ 1, 0, 0, 0, 0, 0, 1, 0 },
		{ 1, 1, 1, 1, 1, 1, 1, 0 },
	};
	std::vector<std::vector<int>> pattern8 = {
		{ 0, 1, 0, 1, 0, 1, 0, 1 },
		{ 1, 0, 1, 0, 1, 0, 1, 0 },
		{ 0, 1, 0, 1, 0, 1, 0, 1 },
		{ 1, 0, 1, 0, 1, 0, 1, 0 },
		{ 0, 1, 0, 1, 0, 1, 0, 1 },
		{ 1, 0, 1, 0, 1, 0, 1, 0 },
		{ 0, 1, 0, 1, 0, 1, 0, 1 },
		{ 1, 0, 1, 0, 1, 0, 1, 0 },
	};
	std::vector<std::vector<int>> pattern9 = {
		{ 0, 0, 1, 1, 0, 0, 1, 1 },
		{ 0, 0, 1, 1, 0, 0, 1, 1 },
		{ 1, 1, 0, 0, 1, 1, 0, 0 },
		{ 1, 1, 0, 0, 1, 1, 0, 0 },
		{ 0, 0, 1, 1, 0, 0, 1, 1 },
		{ 0, 0, 1, 1, 0, 0, 1, 1 },
		{ 1, 1, 0, 0, 1, 1, 0, 0 },
		{ 1, 1, 0, 0, 1, 1, 0, 0 },
	};

	switch (p){
		case 30:
			pattern = pattern0;
			break;
		case 31:
			pattern = pattern1;
			break;
		case 32:
			pattern = pattern2;
			break;
		case 33:
			pattern = pattern3;
			break;
		case 34:
			pattern = pattern4;
			break;
		case 35:
			pattern = pattern5;
			break;
		case 36:
			pattern = pattern6;
			break;
		case 37:
			pattern = pattern7;
			break;
		case 38:
			pattern = pattern8;
			break;
		case 39:
			pattern = pattern9;
			break;
	}
}

/****************************************************************************
*  Set the X dimension of the current window in pixels.                 *
****************************************************************************/
void SetMaxX(int maxX)
{
	numXpixels = maxX;
}
/****************************************************************************
*  Set the X dimension of the current window in pixels.                 *
****************************************************************************/
void SetMaxY(int maxY)
{
	numYpixels = maxY;
	start_text_y = numYpixels - 20;
	end_text_y = 21 + start_text_y;
}

/****************************************************************************
*  Draws a pixel at the specified point on the screen.                      *
*  Caution!! GpiSetPel has been found to crash programs at some locations!  *
****************************************************************************/
void DrawPixel(int x, int y)
{
	SetPixel(hdc, x, y, win_draw_color);
}

HMENU menu, menu_draw, menu_color, menu_pattern;

void MenuBar()
{
	menu = CreateMenu();
	menu_draw = CreatePopupMenu();
	menu_color = CreatePopupMenu();
	menu_pattern = CreatePopupMenu();

	AppendMenu(
		menu,      // handle to menu to be changed
		MF_POPUP,      // menu-item flags
		(UINT)menu_draw,  // menu-item identifier or handle to drop-down menu or submenu
		(LPCTSTR)L"&Draw" // menu-item content
		);


	InsertMenu(menu_draw, 0, MF_STRING, 21, (LPCTSTR)L"&Polygon - ScanLine");

	AppendMenu(menu_draw, MF_STRING, 22, (LPCTSTR)L"&Polygon - FloodFill");

	AppendMenu(menu_draw, MF_STRING, 23, (LPCTSTR)L"&Circle");


	AppendMenu(menu, MF_POPUP, (UINT)menu_color, (LPCTSTR)L"&Color");

	InsertMenu(menu_color, 0, MF_STRING, 1, (LPCTSTR)L"Black");
	AppendMenu(menu_color, MF_STRING, 2, (LPCTSTR)L"Blue");
	AppendMenu(menu_color, MF_STRING, 3, (LPCTSTR)L"Green");
	AppendMenu(menu_color, MF_STRING, 4, (LPCTSTR)L"Cyan");
	AppendMenu(menu_color, MF_STRING, 5, (LPCTSTR)L"Red");

	AppendMenu(menu_color, MF_STRING, 6, (LPCTSTR)L"Magenta");
	AppendMenu(menu_color, MF_STRING, 7, (LPCTSTR)L"Brown");
	AppendMenu(menu_color, MF_STRING, 8, (LPCTSTR)L"LightGray");
	AppendMenu(menu_color, MF_STRING, 9, (LPCTSTR)L"DarkGray");

	AppendMenu(menu_color, MF_STRING, 10, (LPCTSTR)L"LightBlue");
	AppendMenu(menu_color, MF_STRING, 11, (LPCTSTR)L"LightGreen");
	AppendMenu(menu_color, MF_STRING, 12, (LPCTSTR)L"LightCyan");
	AppendMenu(menu_color, MF_STRING, 13, (LPCTSTR)L"LightRed");
	AppendMenu(menu_color, MF_STRING, 14, (LPCTSTR)L"LightMagenta");
	AppendMenu(menu_color, MF_STRING, 15, (LPCTSTR)L"Yellow");
	AppendMenu(menu_color, MF_STRING, 16, (LPCTSTR)L"White");

	AppendMenu(menu, MF_POPUP, (UINT)menu_pattern, (LPCTSTR)L"&Pattern");
	InsertMenu(menu_pattern, 0, MF_STRING, 30, (LPCTSTR)L"Pattern 0");
	AppendMenu(menu_pattern, MF_STRING, 31, (LPCTSTR)L"Pattern 1");
	AppendMenu(menu_pattern, MF_STRING, 32, (LPCTSTR)L"Pattern 2");
	AppendMenu(menu_pattern, MF_STRING, 33, (LPCTSTR)L"Pattern 3");
	AppendMenu(menu_pattern, MF_STRING, 34, (LPCTSTR)L"Pattern 4");
	AppendMenu(menu_pattern, MF_STRING, 35, (LPCTSTR)L"Pattern 5");
	AppendMenu(menu_pattern, MF_STRING, 36, (LPCTSTR)L"Pattern 6");
	AppendMenu(menu_pattern, MF_STRING, 37, (LPCTSTR)L"Pattern 7");
	AppendMenu(menu_pattern, MF_STRING, 38, (LPCTSTR)L"Pattern 8");
	AppendMenu(menu_pattern, MF_STRING, 39, (LPCTSTR)L"Pattern 9");
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

wchar_t wind_class[] = L"Window Application";
wchar_t wind_name[] = L"Lab 01 CCI-36     ";
void InitGraphics()
{

	HINSTANCE hInst = NULL;
	HWND hWnd;
	WNDCLASS    wc;
	LPCWSTR window_class = (LPCWSTR)wind_class;
	//
	LPCWSTR window_name = (LPCWSTR)wind_name;
	// Fill up window structure 

	wc.lpszClassName = window_class;  // registration name
	wc.hInstance = hInst;				// application instance
	wc.lpfnWndProc = (WNDPROC)WinProc; // event handling function
	wc.hCursor = LoadCursor(NULL, IDC_ARROW); // cursor type
	wc.hIcon = NULL;
	wc.lpszMenuName = NULL;						// menu, if any
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH); // background color
	wc.style = CS_HREDRAW | CS_VREDRAW;		// window style
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;

	MenuBar();

	/* Registeer window class      */
	//GlobalAddAtom(window_class);

	if (!RegisterClass(&wc))
	{
		printf(" Error in RegisterClass...\n");
		exit(1);
	}

	// Create window
	hWnd = CreateWindow(
		window_class,           // Desktop window class name             
		L"Lab 2 CCI-36",       // window name                 
		WS_OVERLAPPEDWINDOW | WS_VISIBLE,  // Window class style                  
		0, 0,                //window  top, left corner(origin)
		500, 500,                   // window X,Y size                                    
		(HWND)NULL,                   // Parent window         /
		(HMENU)menu,				// handle to menu 
		(HINSTANCE)hInst,			// handle to application instance 
		(LPVOID)NULL);  //  pointer to window-creation data  

	if ((hWnd == NULL))
	{

		printf("error in CreateWindow ...\n ");
		exit(1);

	}

	// Sets the visibility state of window 
	ShowWindow(hWnd, SW_SHOW);

	// store window handle device context 
	WinHandle = hWnd;
	hdc = GetDC(WinHandle);
	// set hpen, blackbrush for clearing window, color for text and text background
	hpen = CreatePen(PS_SOLID, 1, win_draw_color);
	SelectObject(hdc, hpen);
	blackBrush = (HBRUSH)GetStockObject(BLACK_BRUSH);
	SetBkColor(hdc, RGB(0, 0, 0));
	SetTextColor(hdc, RGB(255, 255, 255));

}

/****************************************************************************
*  Reset display to default mode.                                           *
****************************************************************************/
void CloseGraphics(void)
{

	// Delete pen and destroy window
	DeleteObject(hpen);
	ReleaseDC(WinHandle, hdc);
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
		if (win_draw_color != color_trans_map[new_color])
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
	return (int)GetPixel(hdc, x, y);
}


void CheckGraphicsMsg(void)
{
	MSG msg;
	/*Peek Message from message queue */
	if (PeekMessage(&msg, WinHandle, 0L, 0L, PM_REMOVE))
		//  Translate keyboard hit to virtual key code and send to message queue
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

}
void ClearString(char *str)
{
	str[0] = 0x00;
}
void EraseMessage()
{
	RECT rec = { START_TEXT_X, start_text_y,
		END_TEXT_X, end_text_y };
	HBRUSH backgrdBrush = (HBRUSH)GetStockObject(BLACK_BRUSH);
	// Clear input input box
	FillRect(hdc, &rec, backgrdBrush);
}
void  PrintMessage(char *buffer)
{
	// Write input text in the graphics window
	// Input line is in the upper portion of the graphics window
	if (strlen(buffer)>0)
		TextOutA(hdc, START_TEXT_X, start_text_y, (LPCSTR)buffer, strlen(buffer));

}
void DDA(int x1, int y1, int x2, int y2)
{   // Atenção: x, y, dx, dy real  e i, length inteiro
	int length;
	float x, y, dx, dy;
	length = Max(abs(x2 - x1), abs(y2 - y1));
	if (length>0){
		dx = ((float)(x2 - x1)) / length;
		dy = ((float)(y2 - y1)) / length;
		x = (float)x1;
		y = (float)y1;
		for (int i = 0; i <= length; i++)
		{
			//int size = pattern.size();
			DrawPixel(Arred((int)x), Arred((int)y));
			x = x + dx;    // dx = 1.0 ou -1.0 ou 1/m
			y = y + dy;   // yx = 1.0 ou -1.0 ou m
		}
	}
}
void DrawXorPixel(int x, int y)
{
	unsigned int mask = win_draw_color;
	COLORREF cor = GetPixel(hdc, x, y);
	cor ^= mask; // bit-bit Xor operation mask with color
	SetPixel(hdc, x, y, cor);
}
void DrawXorLine(int x1, int y1, int x2, int y2)
{
	int i, length;
	float  x, y, dx, dy;

	length = Max(abs(x2 - x1), abs(y2 - y1));

	if (length>0)
	{
		dx = ((float)(x2 - x1)) / length;
		dy = ((float)(y2 - y1)) / length;
		x = (float)x1; y = (float)y1;
		for (i = 0; i <= length; i++)
		{
			DrawXorPixel(Arred(x), Arred(y));
			x = x + dx;    // dx = 1 ou -1 ou m
			y = y + dy;   // yx = 1 ou -1 ou 1/m
		}
	}
}
void Bresenham(int x1, int y1, int x2, int y2) {
	int x, y, dx, dy, sx, sy, err, e2;
	int i = 0;
	dx = abs(x2 - x1);
	dy = abs(y2 - y1);
	err = dx - dy;
	if (x1 < x2) sx = 1; else sx = -1;
	if (y1 < y2) sy = 1; else sy = -1;
	while (x1 != x2 && y1 != y2) {
		DrawXorPixel(x1, y1);
		e2 = 2 * err;
		if (e2 > -dy) {
			err = err - dy;
			x1 = x1 + sx;
		}
		if (e2 < dx) {
			err = err + dx;
			y1 = y1 + sy;
		}
		i++;
	}
	DrawXorPixel(x1, y1);
}
void PlotXorCircle(int xc, int yc, int x, int y)
{
	DrawXorPixel(xc + x, yc + y);
	DrawXorPixel(xc + y, yc + x);
	DrawXorPixel(xc + y, yc - x);
	DrawXorPixel(xc + x, yc - y);
	DrawXorPixel(xc - x, yc - y);
	DrawXorPixel(xc - y, yc - x);
	DrawXorPixel(xc - y, yc + x);
	DrawXorPixel(xc - x, yc + y);
}
void PlotCircle(int xc, int yc, int x, int y)
{
	DrawPixel(xc + x, yc + y);
	DrawPixel(xc + y, yc + x);
	DrawPixel(xc + y, yc - x);
	DrawPixel(xc + x, yc - y);
	DrawPixel(xc - x, yc - y);
	DrawPixel(xc - y, yc - x);
	DrawPixel(xc - y, yc + x);
	DrawPixel(xc - x, yc + y);
}
void CircleXorBresenham(int xc, int yc, int r) {
	int x, y, d, deltaE, deltaSE;
	int i = 0;
	x = 0; y = r;
	d = 1 - r; deltaE = 3; deltaSE = -2 * r + 5;

	PlotXorCircle(xc, yc, x, y); // Plot 8 symetrical circle points

	while (y > x)   // Draw a quarter of circle in clockwise
	{
		if (d < 0)
		{
			d += deltaE;
			deltaE += 2;
			deltaSE += 2;
		}
		else {
			d += deltaSE;
			deltaE += 2;
			deltaSE += 4;
			y--;
		}
		x++;
		PlotXorCircle(xc, yc, x, y); // Plot 8 symetrical circle points
		i++;
	}
	PlotXorCircle(xc, yc, x, y);
}
void CircleBresenham(int xc, int yc, int r) {
	int x, y, d, deltaE, deltaSE;
	int i = 0;
	x = 0; y = r;
	d = 1 - r; deltaE = 3; deltaSE = -2 * r + 5;

	PlotCircle(xc, yc, x, y); // Plot 8 symetrical circle points

	while (y > x)   // Draw a quarter of circle in clockwise
	{
		if (d < 0)
		{
			d += deltaE;
			deltaE += 2;
			deltaSE += 2;
		}
		else {
			d += deltaSE;
			deltaE += 2;
			deltaSE += 4;
			y--;
		}
		x++;
		PlotCircle(xc, yc, x, y); // Plot 8 symetrical circle points
		i++;
	}
	PlotCircle(xc, yc, x, y);
}

int distance(int x1, int y1, int x2, int y2){
	return sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2));
}

/****************************************************************************
*  Mouse Handler for Win 95                                                   *
****************************************************************************/
static LRESULT CALLBACK WinProc(HWND hWnd, UINT messg, WPARAM wParam, LPARAM lParam)
{

	PAINTSTRUCT ps;

	char str[3] = " ";
	switch (messg)
	{
	case WM_COMMAND:
		menu_item = LOWORD(wParam);

		break;
	case WM_PAINT:
		BeginPaint(hWnd, &ps);
		PrintMessage(buffer);
		// Draw everything
		//	  RefreshScreen();

		ValidateRect(hWnd, NULL);

		EndPaint(hWnd, &ps);
		break;
	case WM_CHAR:
	{
					// Win32 keyboard message: lParam=key code wParam= virtual key (ASCII) code 

					if (!(LOWORD(wParam) & KF_UP) &&
						!(LOWORD(wParam) & KF_ALTDOWN))

					{
						//  take keyboard input
						key_input = (char)LOWORD(wParam);



						if (key_input == ENTER) // Enter
						{
							EraseMessage();
							//buffer[strlen(buffer)]=0x00;
						}
						else if (key_input == BACKSPACE) // BackSpace
						{
							if (strlen(buffer)>0)
							{
								int len = strlen(buffer) - 1;
								// Clear last character in buffer
								buffer[len] = ' ';
								// Clear characters in input box
								strcat_s(buffer, "   ");
								PrintMessage(buffer);
								buffer[len] = 0x00; // put end string
							}
						}
						else if (key_input>31 && key_input<130)
						{
							int leng = strlen(buffer);
							EraseMessage();
							str[0] = key_input;
							strcat_s(buffer, str); // add char
							// display, update input box
							PrintMessage(buffer);
						}
						else if (key_input != ESC) //ESC
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
	if (key_input == MK_LBUTTON)

	{
		EraseMessage();
		mouse_x = LOWORD(lParam);
		mouse_y = HIWORD(lParam);
		key_input = wParam;
		printf_s(buffer, " x = %d y = %d", mouse_x, mouse_y);
		PrintMessage(buffer);
		mouse_action = L_MOUSE_MOVE_DOWN;
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
						   printf_s(buffer, " x = %d y = %d", mouse_x, mouse_y);
						   PrintMessage(buffer);
						   mouse_action = L_MOUSE_DOWN;
						   ClearString(buffer);
						   break;
	}
	case WM_LBUTTONUP:

	{

						 EraseMessage();
						 mouse_x = LOWORD(lParam);
						 mouse_y = HIWORD(lParam);
						 key_input = wParam;
						 sprintf_s(buffer, " x = %d y = %d", mouse_x, mouse_y);
						 PrintMessage(buffer);
						 mouse_action = L_MOUSE_UP;
						 ClearString(buffer);
						 break;
	}

	case WM_RBUTTONDOWN:
	{
						   EraseMessage();
						   key_input = wParam;
						   mouse_x = LOWORD(lParam);
						   mouse_y = HIWORD(lParam);

						   sprintf_s(buffer, " x = %d y = %d", mouse_x, mouse_y);
						   PrintMessage(buffer);

						   mouse_action = R_MOUSE_DOWN;
						   ClearString(buffer);

						   break;
	}
	case WM_LBUTTONDBLCLK:
		EraseMessage();
		mouse_x = LOWORD(lParam);
		mouse_y = HIWORD(lParam);
		key_input = wParam;
		sprintf_s(buffer, " x = %d y = %d", mouse_x, mouse_y);
		PrintMessage(buffer);
		mouse_action = L_DOUBLE_CLICK;
		ClearString(buffer);
		key_input = wParam;

		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return(DefWindowProc(hWnd, messg, wParam, lParam));
		break;
	}

	return 0;

}
void PolyInsert(edge_list_type &list, int x1, int y1, int x2, int y2)
{     // insert line segment in edge struct, if not horizontal
	if (y1 != y2)
	{
		int YM = Max(y1, y2), J1 = list.n + 1;

		while (J1 != 1 && list.edge[J1 - 1].Ymax<YM)
		{
			list.edge[J1] = list.edge[J1 - 1];
			J1--;
		}

		list.edge[J1].Ymax = YM;
		list.edge[J1].dx = -1 * (float)(x2 - x1) / (y2 - y1);
		if (y1>y2)
		{
			list.edge[J1].Ymin = y2;
			list.edge[J1].Xinter = (float)x1;
		}
		else {
			list.edge[J1].Ymin = y1;
			list.edge[J1].Xinter = (float)x2;
		}

		list.n++;
	}

}
void InsertVertex(polygon_type &poly, int x, int y)
{	// insert x,y as the last element
	if (poly.n<MAX_POLY)
	{
		poly.vertex[poly.n].x = x;
		poly.vertex[poly.n].y = y;
		poly.n++;
	}
}
void GetPoint(polygon_type polygon, int k, int &x, int &y)
{
	x = polygon.vertex[k].x;
	y = polygon.vertex[k].y;

}
void LoadPolygon(polygon_type &polygon, edge_list_type &list, int &num_Edges)
{
	int x1, x2, y1, y2, k = 0;

	list.n = 0;
	GetPoint(polygon, k, x1, y1);
	num_Edges = 0;

	for (; k < polygon.n; k++)
	{
		GetPoint(polygon, (k+1)%polygon.n, x2, y2);
		if (y1 == y2) x1 = x2;
		else
		{
			PolyInsert(list, x1, y1, x2, y2);
			num_Edges += 1;
			x1 = x2;
			y1 = y2;
		}
	}

}

void Include(edge_list_type &list, int &end_Edge, int final_Edge, int scan)
{	// include all edges that intersept y_scan
	while (end_Edge < final_Edge && list.edge[end_Edge + 1].Ymax >= scan)
	{
		end_Edge++;
	}
}

void FillIn(int x1, int x2, int y)
{
	int px, py;

	py = y % 8;

	if (x1 != x2)
	{
		for (int x = x1; x <= x2; x++)
		{
			px = x % 8;

			if (pattern[y%8][x%8]) DrawPixel(x, y);

		}
	}
}

void UpdateXValues(edge_list_type &list, int last_Edge, int &start_Edge, int scan)
{
	int K1;

	for (K1 = start_Edge; K1 <= last_Edge; K1++)
	{
		if (list.edge[K1].Ymin < scan)
		{
			list.edge[K1].Xinter += list.edge[K1].dx;
		}
		else
		{  // remove edge
			start_Edge++;
			if (start_Edge <= K1)
			for (int i = K1; i >= start_Edge; i--)
				list.edge[i] = list.edge[i - 1];
		}
	}
}
void FillScan(edge_list_type &list, int end_Edge, int start_Edge, int scan)
{
	int NX, J, K;


	NX = (end_Edge - start_Edge + 1) / 2;

	J = start_Edge;


	for (K = 1; K <= NX; K++)
	{
		FillIn((int)list.edge[J].Xinter,
			(int)list.edge[J + 1].Xinter, scan);
		J += 2;
	}

}
void XSort(edge_list_type &list, int start_Edge, int last_Edge)
{
	int L, k; bool sorted = false;
	edge_type temp;

	// Use bubble sort

	for (k = start_Edge; k < last_Edge; k++)
	{

		L = k + 1;

		while (L > start_Edge &&
			list.edge[L].Xinter < list.edge[L - 1].Xinter)
		{
			temp = list.edge[L];
			list.edge[L] = list.edge[L - 1];
			list.edge[L - 1] = temp;
			L--;

		}

	}

}
void FillPolygon(polygon_type &polygon, edge_list_type &list)
{
	int Edges, start_Edge, end_Edge, scan;
	LoadPolygon(polygon, list, Edges);
	if (Edges == 2) return;
	scan = list.edge[1].Ymax;
	start_Edge = 1;
	end_Edge = 1;
	Include(list, end_Edge, Edges, scan);
	while (end_Edge != start_Edge - 1)
	{
		XSort(list, start_Edge, end_Edge);
		FillScan(list, end_Edge, start_Edge, scan);
		scan--;
		UpdateXValues(list, end_Edge, start_Edge, scan);
		Include(list, end_Edge, Edges, scan);
	}

}

bool Empty(int x, int y)
{
	return(GetPixel(x, y) == 0);
}

bool Empty(int x, int y, int color)
{
	int colorPixel = GetPixel(x, y);
	return(colorPixel != color_trans_map[color]);
}

void FloodFillRec(int x, int y)
{
	if (Empty(x, y))
	{

		DrawPixel(x, y);

		if (Empty(x + 1, y))
			FloodFillRec(x + 1, y);
		if (Empty(x - 1, y))
			FloodFillRec(x - 1, y);
		if (Empty(x, y + 1))
			FloodFillRec(x, y + 1);
		if (Empty(x, y - 1))
			FloodFillRec(x, y - 1);
			DrawPixel(x, y);

	}
}

void FloodFillIterative(polygon_type poly, int color){
	std::queue<point_type> q;
	point_type seed; 
	seed.x = 0, seed.y = 0;
	for (int i = 0; i<poly.n; i++)
	{
		seed.x += poly.vertex[i].x;
		seed.y += poly.vertex[i].y;
	}
	seed.x /= poly.n;
	seed.y /= poly.n;
	
	q.push(seed);
	point_type current, west, east, north, south;
	while (!q.empty()){
		current = q.front();
		q.pop();
		if (Empty(current.x, current.y, color)){
			DrawPixel(current.x, current.y);
			west.x = current.x - 1;
			west.y = current.y;
			east.x = current.x + 1;
			east.y = current.y;
			north.x = current.x;
			north.y = current.y - 1;
			south.x = current.x;
			south.y = current.y + 1;
			q.push(west);
			q.push(east);
			q.push(north);
			q.push(south);
		}
	}
}

void FillCircle(int r_x, int r_y, int color){
	std::queue<point_type> q;
	point_type seed;
	seed.x = r_x;
	seed.y = r_y;

	q.push(seed);
	point_type current, west, east, north, south;
	while (!q.empty()){
		current = q.front();
		q.pop();
		if (Empty(current.x, current.y, color)){
			DrawPixel(current.x, current.y);
			west.x = current.x - 1;
			west.y = current.y;
			east.x = current.x + 1;
			east.y = current.y;
			north.x = current.x;
			north.y = current.y - 1;
			south.x = current.x;
			south.y = current.y + 1;
			q.push(west);
			q.push(east);
			q.push(north);
			q.push(south);
		}
	}
}

void FloodFillRecursive(polygon_type poly)
{
	
	for (int i = 0; i < poly.n; i++){
		DrawPixel(poly.vertex[i].x, poly.vertex[i].y);
	}

	int x_seed = 0, y_seed = 0;
	for (int i = 0; i<poly.n; i++)
	{
		x_seed += poly.vertex[i].x;
		y_seed += poly.vertex[i].y;
	}
	x_seed /= poly.n;
	y_seed /= poly.n;

	point_type seed;
	seed.x = x_seed;
	seed.y = y_seed;
	FloodFillRec(x_seed, y_seed);
}


void main()
{
	int p0_x, p0_y, p1_x, p1_y, menu_it = 0, color = MY_WHITE;
	InitGraphics();

	polygon_type polygon;
	polygon.n = 0;
	my_shape shape = POLY_SCAN;

	menu_item = 0;
	CheckMenuItem(menu_color, 1, MF_CHECKED);
	CheckMenuItem(menu_draw, 21, MF_CHECKED);
	CheckMenuItem(menu_draw, 30, MF_CHECKED);
	while (key_input != ESC) {	// ESC exits the program
		CheckGraphicsMsg();
		if (menu_it != menu_item)
			switch (menu_item){
			case 21:{
				CheckMenuItem(menu_draw, 21, MF_CHECKED);
				CheckMenuItem(menu_draw, 22, MF_UNCHECKED);
				CheckMenuItem(menu_draw, 23, MF_UNCHECKED);
				menu_it = menu_item;
				shape = POLY_SCAN;
				polygon.n = 0;
				break;
			}
			case 22:
			{
				CheckMenuItem(menu_draw, 21, MF_UNCHECKED);
				CheckMenuItem(menu_draw, 22, MF_CHECKED);
				CheckMenuItem(menu_draw, 23, MF_UNCHECKED);
				menu_it = menu_item;
				shape = POLY_FLOOD;
				polygon.n = 0;
				break;
			}
			case 23:
			{
				CheckMenuItem(menu_draw, 21, MF_UNCHECKED);
				CheckMenuItem(menu_draw, 22, MF_UNCHECKED);
				CheckMenuItem(menu_draw, 23, MF_CHECKED);

				menu_it = menu_item;
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
					CheckMenuItem(menu_pattern, i, MF_UNCHECKED);
					CheckMenuItem(menu_pattern, menu_item, MF_CHECKED);
				}
				UpdatePattern(menu_item);
				menu_it = menu_item;
				break;
			default:
			{ 
				for (int i = 1; i <= 16; i++)
					CheckMenuItem(menu_color, i, MF_UNCHECKED);
					CheckMenuItem(menu_color, menu_item, MF_CHECKED);
				if (menu_item >= 1 && menu_item <= 16)
					color = menu_item - 1;

				menu_it = menu_item;

			}
		}
		SetGraphicsColor(color, numXpixels);

		if (mouse_action == L_MOUSE_DOWN)
		{  
			switch (shape) 
			{
			case POLY_FLOOD:
			case POLY_SCAN:
				// Pick first point up 
				if (polygon.n == 0)
				{
					p0_x = p1_x = mouse_x;
					p0_y = p1_y = mouse_y;
					InsertVertex(polygon, p0_x, p0_y);
				}
				break;
			case CIRCLE:
				p0_x = p1_x = mouse_x;
				p0_y = p1_y = mouse_y;
				break;
			}			
		}
		if (mouse_action == L_MOUSE_MOVE_DOWN)
		{  
			if (p1_x != mouse_x || p1_y != mouse_y)
			{  
				switch (shape)
				{
				case POLY_FLOOD:
				case POLY_SCAN:
					  // Draw new line
					DrawXorLine(p0_x, p0_y, p1_x, p1_y);
					p1_x = mouse_x;
					p1_y = mouse_y;
					DrawXorLine(p0_x, p0_y, p1_x, p1_y);
					break;
				case CIRCLE:
					CircleXorBresenham(p0_x, p0_y, distance(p0_x, p0_y, p1_x, p1_y));
					p1_x = mouse_x;
					p1_y = mouse_y;
					CircleXorBresenham(p0_x, p0_y, distance(p0_x, p0_y, p1_x, p1_y));
					break;
				}
			}
		}
		else  if (mouse_action == L_MOUSE_UP)
		{
			switch (shape)
			{
			case POLY_FLOOD:
			case POLY_SCAN:
				DrawXorLine(p0_x, p0_y, p1_x, p1_y);
				DDA(p0_x, p0_y, p1_x, p1_y);
				p0_x = p1_x = mouse_x;
				p0_y = p1_y = mouse_y;

				if (polygon.n > 0 &&
					(polygon.vertex[polygon.n - 1].x != p0_x
					|| polygon.vertex[polygon.n - 1].y != p0_y))
					InsertVertex(polygon, p0_x, p0_y);
				break;
			case CIRCLE:
				CircleXorBresenham(p0_x, p0_y, distance(p0_x, p0_y, p1_x, p1_y));
				p1_x = mouse_x;
				p1_y = mouse_y;
				CircleBresenham(p0_x, p0_y, distance(p0_x, p0_y, p1_x, p1_y));
				break;
			}
			mouse_action = NO_ACTION;
		}
		else if (mouse_action == R_MOUSE_DOWN)
		{
			switch (shape)
			{
			case POLY_FLOOD:
			case POLY_SCAN:
				edge_list_type list;
				int num_Edges;
				DDA(polygon.vertex[0].x, polygon.vertex[0].y, polygon.vertex[polygon.n - 1].x, polygon.vertex[polygon.n - 1].y);
				for (int i = 0; i < polygon.n; i++) {
					DrawPixel(polygon.vertex[i].x, polygon.vertex[i].y);
				}
				if (shape == POLY_SCAN) FillPolygon(polygon, list);
				else if (shape == POLY_FLOOD) FloodFillIterative(polygon, color);
				polygon.n = 0;
				break;
			case CIRCLE:
				FillCircle(p0_x, p0_y, color);
				break;
			}
			mouse_action = NO_ACTION;
		}
	}
	CloseGraphics();
}
