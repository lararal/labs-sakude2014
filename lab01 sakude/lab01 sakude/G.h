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

static class G {

public:
	static SHORT numXpixels;
	static SHORT numYpixels;

	static int key_input;
	static int mouse_action;
	static int mouse_x;
	static int mouse_y;

	static BOOL graphics;

	static std::vector<bool> pattern;

	// current draw color
	static COLORREF win_draw_color;
	static HBRUSH blackBrush;
	static HDC  hdc;
	static HWND WinHandle;
	// Foreground colors default to standard EGA palette.
	// No map is necessary unless a non-standard palette is implemented.
	static HPEN hpen;
	// 16 Predefined colors

	static my_shape shape;

	// Set the X dimension of the current window in pixels.
	static void SetMaxX(int maxX);

	// Set the X dimension of the current window in pixels.
	static void SetMaxY(int maxY);

	// Draws a pixel at the specified point on the screen.
	// Caution!! GpiSetPel has been found to crash programs at some locations! 
	static void DrawPixel(int x, int y);

	// Initializes the process for Window services
	// obtains anchor block handle
	// creates the main frame window which creates the main client window
	static void InitGraphics();

	// Reset display to default mode.
	static void CloseGraphics(void);

	// Returns the X dimension of the current window in pixels.
	static int GetMaxX(void);

	// Returns the Y dimension of the current window in pixels.
	static int GetMaxY(void);

	// Set current graphics drawing color.
	static void SetGraphicsColor(int new_color, int width);

	static void SetGraphicsPatern(int new_pattern);

	////// Returns the color value of the pixel at the specified point on the screen.
	////int GetPixel(int x, int y)
	////{
	////	return (int)GetPixel(hdc, x, y);
	////}

	static void CheckGraphicsMsg(void);

	static void ClearString(char *str);

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

	static void DDA(int x1, int y1, int x2, int y2);

	static void DrawXorPixel(int x, int y);

	static void DrawXorLine(int x1, int y1, int x2, int y2);

	static void Bresenham(int x1, int y1, int x2, int y2);

	static void PlotCircle(int xc, int yc, int x, int y);

	static void CircleBresenham(int xc, int yc, int r);

	static int distance(int x1, int y1, int x2, int y2);

	static void Draw(int x1, int y1, int x2, int y2);

	// Mouse Handler for Win 95
	static LRESULT CALLBACK WinProc(HWND hWnd, UINT messg, WPARAM wParam, LPARAM lParam);

	void X();
};
