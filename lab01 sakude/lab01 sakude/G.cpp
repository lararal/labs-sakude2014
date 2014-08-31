#include "stdafx.h"
#include <windows.h>
#include "grafbase.h"
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <vector>
#include "G.h"

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

wchar_t wind_class[] = L"Window Application";
wchar_t wind_name[] = L"Lab 01 CCI-36     ";


// Set the X dimension of the current window in pixels.
void G::SetMaxX(int maxX)
{
	numXpixels = maxX;
}

// Set the X dimension of the current window in pixels.
void G::SetMaxY(int maxY)
{
	numYpixels = maxY;
}

// Draws a pixel at the specified point on the screen.
// Caution!! GpiSetPel has been found to crash programs at some locations! 
void G::DrawPixel(int x, int y)
{
	SetPixel(hdc, x, y, win_draw_color);
}
       
// Initializes the process for Window services
// obtains anchor block handle
// creates the main frame window which creates the main client window
static void InitGraphics();

// Reset display to default mode.
static void CloseGraphics(void)
{
	// Delete pen and destroy window
	DeleteObject(hpen);
	ReleaseDC(WinHandle, hdc);
	DestroyWindow(WinHandle);
}

// Returns the X dimension of the current window in pixels.
static int GetMaxX(void)
{
	return numXpixels;
}

// Returns the Y dimension of the current window in pixels.
static int GetMaxY(void)
{
	return numYpixels;
}

// Set current graphics drawing color.
static void SetGraphicsColor(int new_color, int width)
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

static void SetGraphicsPatern(int new_pattern){
	switch (new_pattern){
	case FULL:
		pattern = { 1, 1, 1, 1, 1, 1, 1, 1 };
		break;
	case DOTTED:
		pattern = { 1, 0, 0 };
		break;
	case DASHED:
		pattern = { 1, 1, 1, 0, 0, 0 };
		break;
	case DOTTED_DASHED:
		pattern = { 1, 1, 1, 0, 0, 0, 1, 0, 0, 0 };
		break;
	default:
		break;
	}
}

////// Returns the color value of the pixel at the specified point on the screen.
////int GetPixel(int x, int y)
////{
////	return (int)GetPixel(hdc, x, y);
////}

static void CheckGraphicsMsg(void)
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

static void ClearString(char *str)
{
	str[0] = 0x00;
}

////void EraseMessage()
////{
////	RECT rec = { START_TEXT_X, start_text_y,
////		END_TEXT_X, end_text_y };
////	HBRUSH backgrdBrush = (HBRUSH)GetStockObject(BLACK_BRUSH);
////	// Clear input input box
////	FillRect(hdc, &rec, backgrdBrush);
////}

////void  PrintMessage(char *buffer)
////{
////	// Write input text in the graphics window
////	// Input line is in the upper portion of the graphics window
////	if (strlen(buffer)>0)
////		TextOutA(hdc, START_TEXT_X, start_text_y, (LPCSTR)buffer, strlen(buffer));
////}

static void DDA(int x1, int y1, int x2, int y2)
{   
	// Atenção: x, y, dx, dy real  e i, length inteiro
	int length;
	float x, y, dx, dy;
	length = Max(abs(x2 - x1), abs(y2 - y1));
	if (length>0) {
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

static void DrawXorPixel(int x, int y)
{
	unsigned int mask = win_draw_color;
	COLORREF cor = GetPixel(hdc, x, y);
	cor ^= mask; // bit-bit Xor operation mask with color
	SetPixel(hdc, x, y, cor);
}

static void DrawXorLine(int x1, int y1, int x2, int y2)
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

static void Bresenham(int x1, int y1, int x2, int y2) {
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

static void PlotCircle(int xc, int yc, int x, int y)
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

static void CircleBresenham(int xc, int yc, int r) {
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

static int distance(int x1, int y1, int x2, int y2){
	return sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2));
}

static void Draw(int x1, int y1, int x2, int y2){
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

// Mouse Handler for Win 95
static LRESULT CALLBACK WinProc(HWND hWnd, UINT messg, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;

	char str[3] = " ";
	switch (messg)
	{
	case WM_PAINT:
		BeginPaint(hWnd, &ps);
		ValidateRect(hWnd, NULL);
		EndPaint(hWnd, &ps);
		break;
	case WM_CHAR:
		break;
	case WM_SIZE:
		// resize 
		SetMaxX(LOWORD(lParam));  // width of client area 
		SetMaxY(HIWORD(lParam));
		PostMessage(WinHandle, WM_PAINT, wParam, lParam);
		break;
	case WM_MOUSEMOVE:
		key_input = wParam;
		mouse_x = LOWORD(lParam);
		mouse_y = HIWORD(lParam);
		key_input = wParam;
		mouse_action = L_MOUSE_MOVE_DOWN;
		break;
	case WM_LBUTTONDOWN:
		mouse_x = LOWORD(lParam);
		mouse_y = HIWORD(lParam);
		key_input = wParam;
		mouse_action = L_MOUSE_DOWN;
		break;
	case WM_LBUTTONUP:
		mouse_x = LOWORD(lParam);
		mouse_y = HIWORD(lParam);
		key_input = wParam;
		mouse_action = L_MOUSE_UP;
		break;
	case WM_RBUTTONDOWN:
		key_input = wParam;
		mouse_x = LOWORD(lParam);
		mouse_y = HIWORD(lParam);
		mouse_action = R_MOUSE_DOWN;
		break;
	case WM_LBUTTONDBLCLK:
		mouse_x = LOWORD(lParam);
		mouse_y = HIWORD(lParam);
		key_input = wParam;
		mouse_action = L_DOUBLE_CLICK;
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

SHORT G::numXpixels;
SHORT G::numYpixels;

int G::key_input;
int G::mouse_action;
int G::mouse_x;
int G::mouse_y;

BOOL G::graphics;

std::vector<bool> G::pattern;

// current draw color
COLORREF G::win_draw_color;
HBRUSH G::blackBrush;
HDC  G::hdc;
HWND G::WinHandle;
// Foreground colors default to standard EGA palette.
// No map is necessary unless a non-standard palette is implemented.
HPEN G::hpen;
// 16 Predefined colors

my_shape G::shape;