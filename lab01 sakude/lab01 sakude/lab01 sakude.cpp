// lab01 sakude.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <windows.h>
#include "grafbase.h"
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <vector>


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
std::vector<bool> pattern = { 1, 1, 1, 1, 1, 1, 1, 1 };

static COLORREF win_draw_color = RGB(255, 255, 255);  // current draw color
static HBRUSH blackBrush;
HDC  hdc;                                   /* Presentation Space Handle */
HWND WinHandle = NULL;                /* Client area window handle */
/* Foreground colors default to standard EGA palette.                */
/* No map is necessary unless a non-standard palette is implemented. */
static  HPEN hpen;
// 16 Predefined colors

typedef enum {
	FULL, DOTTED, DASHED, DOTTED_DASHED
} my_pattern;

typedef enum {
	LINEDDA, BRESENHAM, CIRCLE
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

my_shape shape = LINEDDA;

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
		window_name,       // window name                 
		WS_OVERLAPPEDWINDOW | WS_VISIBLE,  // Window class style                  
		0, 0,                //window  top, left corner(origin)
		500, 500,                   // window X,Y size                                    
		(HWND)NULL,                   // Parent window         /
		(HMENU)NULL,				// handle to menu 
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

void SetGraphicsPatern(int new_pattern){
	switch (new_pattern){
		case FULL:
			pattern = { 1, 1, 1, 1, 1, 1, 1, 1 };
			break;
		case DOTTED:
			pattern = { 1, 0, 0};
			break;
		case DASHED:
			pattern = { 1, 1, 1, 0, 0, 0};
			break;
		case DOTTED_DASHED:
			pattern = { 1, 1, 1, 0, 0, 0, 1, 0, 0 , 0};
			break;
		default:
			break;
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
		if(length>0){
			dx = ((float)(x2 - x1)) / length;
			dy = ((float)(y2 - y1)) / length;
			x = (float)x1; 
			y = (float)y1;
			for (int i = 0; i <= length; i++)
			{
				//int size = pattern.size();
				if (pattern[i%pattern.size()]) DrawPixel(Arred((int)x), Arred((int)y));
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
			if (pattern[i%pattern.size()]) DrawXorPixel(Arred(x), Arred(y));
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
		if (pattern[i%pattern.size()]) DrawXorPixel(x1, y1);
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

void PlotCircle(int xc, int yc, int x, int y)
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
		if (pattern[i%pattern.size()])  PlotCircle(xc, yc, x, y); // Plot 8 symetrical circle points
		i++;
	}
	PlotCircle(xc, yc, x, y);
}

int distance(int x1, int y1, int x2, int y2){
	return sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2));
}

void Draw(int x1, int y1, int x2, int y2){

	switch (shape)
	{
	case LINEDDA:
		DrawXorLine(x1, y1, x2, y2);
		break;
	case BRESENHAM:
		Bresenham(x1, y1, x2, y2);
		break;
	case CIRCLE:
		CircleBresenham(x1, y1, distance(x1, y1, x2, y2));
		break;
	default:
		break;
	}
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


void main()
{
	int p0_x, p0_y, p1_x, p1_y, cor, padrao, color = MY_MAGENTA;
	char command[4];
	InitGraphics();
	while (key_input != ESC) {	// ESC exits the program
		CheckGraphicsMsg();
		if (key_input == ENTER)	 // Identify Enter
		{
			if (strlen(buffer) >= 3){
				
				if (buffer[0] == 'c' && buffer[1] == 'o' && buffer[2] == 'l'){
					cor = atoi(&buffer[4]);
					if (cor >= 0 && cor < 16)
						color = cor;
					ClearString(buffer);  // Erase buffer
				}
				else if (buffer[0] == 'p' && buffer[1] == 'a' && buffer[2] == 't'){
					padrao = atoi(&buffer[4]);
					if (padrao >= 0 && padrao < 4)
						SetGraphicsPatern(padrao);
					ClearString(buffer);  // Erase buffer
				}
				else if (buffer[0] == 'd' && buffer[1] == 'd' && buffer[2] == 'a'){
					shape = LINEDDA;
					ClearString(buffer);  // Erase buffer
				}
				else if (buffer[0] == 'b' && buffer[1] == 'r' && buffer[2] == 'e'){
					shape = BRESENHAM;
					ClearString(buffer);  // Erase buffer
				}
				else if (buffer[0] == 'c' && buffer[1] == 'i' && buffer[2] == 'r'){
					shape = CIRCLE;
					ClearString(buffer);  // Erase buffer
				}
				else {
					ClearString(buffer);  // Erase buffer
				}
			}
			key_input = -1;
		}
		if (mouse_action == L_MOUSE_DOWN) {  	// Pick first point 
			p0_x = p1_x = mouse_x; p0_y = p1_y = mouse_y;
			//	 mouse_action=NO_ACTION;
		}
		if (mouse_action == L_MOUSE_MOVE_DOWN){//Mouse moving left button
			if (p1_x != mouse_x || p1_y != mouse_y)  	// test if x or y changed
			{	// Erase previous line. 

				SetGraphicsColor((int)MY_LIGHTGREEN, 1);
				Draw(p0_x, p0_y, p1_x, p1_y);
				p1_x = mouse_x; p1_y = mouse_y;
				Draw(p0_x, p0_y, p1_x, p1_y);
			}
		}
		if (mouse_action == L_MOUSE_UP)
		{
			Draw(p0_x, p0_y, p1_x, p1_y); //Erase the last xor line 
			SetGraphicsColor((int)color, 2);
			Draw(p0_x, p0_y, p1_x, p1_y); // Draw  final line
			mouse_action = NO_ACTION;
		}
	}
	CloseGraphics();

}


