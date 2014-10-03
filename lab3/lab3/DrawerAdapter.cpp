#include "stdafx.h"
#include "DrawerAdapter.h"


SHORT DrawerAdapter::numXpixels = 500;
SHORT DrawerAdapter::numYpixels = 500;
int DrawerAdapter::start_text_y = numYpixels - 20;
int DrawerAdapter::end_text_y = 21 + start_text_y;
int DrawerAdapter::key_input = NO_ACTION;
int DrawerAdapter::mouse_action = NO_ACTION;
int DrawerAdapter::mouse_x; 
int DrawerAdapter::mouse_y;
char DrawerAdapter::buffer[200] = "";
HBRUSH DrawerAdapter::blackBrush = NULL;
HDC  DrawerAdapter::hdc = NULL;    
HWND DrawerAdapter::WinHandle = NULL; 
HPEN DrawerAdapter::hpen = NULL;
int DrawerAdapter::menu_item = 0;

DrawerAdapter::DrawerAdapter(){
	color_trans_map =
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
	pattern = {
		{ 1, 1, 1, 1, 1, 1, 1, 1 },
		{ 1, 1, 1, 1, 1, 1, 1, 1 },
		{ 1, 1, 1, 1, 1, 1, 1, 1 },
		{ 1, 1, 1, 1, 1, 1, 1, 1 },
		{ 1, 1, 1, 1, 1, 1, 1, 1 },
		{ 1, 1, 1, 1, 1, 1, 1, 1 },
		{ 1, 1, 1, 1, 1, 1, 1, 1 },
		{ 1, 1, 1, 1, 1, 1, 1, 1 },
	};

}

DrawerAdapter::~DrawerAdapter(){
}

void DrawerAdapter::UpdatePattern(int p){

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

void DrawerAdapter::SetMaxX(int maxX)
{
	numXpixels = maxX;
}

void DrawerAdapter::SetMaxY(int maxY)
{
	numYpixels = maxY;
	start_text_y = numYpixels - 20;
	end_text_y = 21 + start_text_y;
}

void DrawerAdapter::DrawPixel(int x, int y)
{
	SetPixel(hdc, x, y, win_draw_color);
}

void DrawerAdapter::MenuBar()
{
	menu = CreateMenu();
	menu_draw = CreatePopupMenu();
	menu_color = CreatePopupMenu();
	menu_pattern = CreatePopupMenu();

	AppendMenu(
		menu,      // handle to menu to be changed
		MF_POPUP,      // menu-item flags
		(UINT)menu_draw,  // menu-item identifier or handle to drop-down menu or submenu
		(LPCTSTR)L"&Tool" // menu-item content
		);
	InsertMenu(menu_draw, 0, MF_STRING, 21, (LPCTSTR)L"&Polygon - Empty");
	AppendMenu(menu_draw, MF_STRING, 22, (LPCTSTR)L"&Polygon - ScanFill");
	AppendMenu(menu_draw, MF_STRING, 23, (LPCTSTR)L"&Polygon - FloodFill");
	//AppendMenu(menu_draw, MF_STRING, 24, (LPCTSTR)L"&Circle");
	AppendMenu(menu_draw, MF_STRING, 24, (LPCTSTR)L"&Segment");
	AppendMenu(menu_draw, MF_STRING, 25, (LPCTSTR)L"&Pick");
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

void DrawerAdapter::InitGraphics()
{

	HINSTANCE hInst = NULL;
	HWND hWnd;
	WNDCLASS wc;
	LPCWSTR window_class = (LPCWSTR)L"Window Application";
	//
	LPCWSTR window_name = (LPCWSTR)L"Lab 03 CCI-36     ";

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

	if (!RegisterClass(&wc))
	{
		printf(" Error in RegisterClass...\n");
		exit(1);
	}

	// Create window
	hWnd = CreateWindow(
		window_class,           // Desktop window class name             
		L"Lab 3 CCI-36",       // window name                 
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

void DrawerAdapter::CloseGraphics(void)
{

	// Delete pen and destroy window
	DeleteObject(hpen);
	ReleaseDC(WinHandle, hdc);
	DestroyWindow(WinHandle);          /* Tidy up...                 */
}

int DrawerAdapter::GetMaxX(void)
{
	return numXpixels;
}

int DrawerAdapter::GetMaxY(void)
{
	return numYpixels;
}

void DrawerAdapter::SetGraphicsColor(int new_color, int width)
{
	HPEN hpenOld;
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

void DrawerAdapter::ClearGraphicsScreen()
{
	RECT rect;
	HBRUSH hbrush = CreateSolidBrush(color_trans_map[MY_BLACK]);
	GetWindowRect(WinHandle, &rect);
	FillRect(hdc, &rect, hbrush);
}

int DrawerAdapter::GetPixels(int x, int y)
{
	return (int)GetPixel(hdc, x, y);
}


void DrawerAdapter::CheckGraphicsMsg(void)
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
void DrawerAdapter::ClearString(char *str)
{
	str[0] = 0x00;
}
void DrawerAdapter::EraseMessage()
{
	RECT rec = { START_TEXT_X, start_text_y,
		END_TEXT_X, end_text_y };
	HBRUSH backgrdBrush = (HBRUSH)GetStockObject(BLACK_BRUSH);
	// Clear input input box
	FillRect(hdc, &rec, backgrdBrush);
}

void  DrawerAdapter::PrintMessage(char *buffer)
{
	// Write input text in the graphics window
	// Input line is in the upper portion of the graphics window
	if (strlen(buffer)>0)
		TextOutA(hdc, START_TEXT_X, start_text_y, (LPCSTR)buffer, strlen(buffer));

}
void DrawerAdapter::DDA(int x1, int y1, int x2, int y2)
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
void DrawerAdapter::DrawXorPixel(int x, int y)
{
	unsigned int mask = win_draw_color;
	COLORREF cor = GetPixel(hdc, x, y);
	cor ^= mask; // bit-bit Xor operation mask with color
	SetPixel(hdc, x, y, cor);
}
void DrawerAdapter::DrawXorLine(int x1, int y1, int x2, int y2)
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
void DrawerAdapter::Bresenham(int x1, int y1, int x2, int y2) {
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
void DrawerAdapter::PlotXorCircle(int xc, int yc, int x, int y)
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
void DrawerAdapter::PlotCircle(int xc, int yc, int x, int y)
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
void DrawerAdapter::CircleXorBresenham(int xc, int yc, int r) {
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
void DrawerAdapter::CircleBresenham(int xc, int yc, int r) {
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


int DrawerAdapter::distance(int x1, int y1, int x2, int y2){
	return sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2));
}


void DrawerAdapter::DrawEllipse(int x, int y, int x_radius, int y_radius, int filled){
	int x1, y1, x2, y2;
	//Win32 Ellipse function requires ellipse bounding box as input
	
	// calculate the bounding box
	if (x_radius>0){
		x1 = x - x_radius;
		x2 = x + x_radius;
	}
	else if (x_radius == 0){// make the ellipse 2 pixels wide (a line)
		x1 = x;
		x2 = x + 1;
	}
	else return; // wrong radius
	if (y_radius>0){
		y1 = y - y_radius;
		y2 = y + y_radius;
	}
	else if (y_radius == 0){ // make the ellipse 2 pixels wide (a line)
		y1 = y;
		y2 = y + 1;
	}
	else return;
	if (!filled)
	{	//Need to select NULL_BRUSH to avoid filling the ellipse
		HBRUSH brush = (HBRUSH)GetStockObject(NULL_BRUSH);
		SelectObject(hdc, brush);
		Ellipse(hdc, x1, y1, x2, y2);  // Draw ellipse
	}
	else{// Draw a solid ellipse with a brush of current color 
		HBRUSH brush = CreateSolidBrush(win_draw_color);
		SelectObject(hdc, brush);
		Ellipse(hdc, x1, y1, x2, y2);
		// remove brush
		DeleteObject(brush);
	}
}


LRESULT CALLBACK DrawerAdapter::WinProc(HWND hWnd, UINT messg, WPARAM wParam, LPARAM lParam)
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
void DrawerAdapter::PolyInsert(edge_list_type &list, int x1, int y1, int x2, int y2)
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
void DrawerAdapter::InsertVertex(polygon_type &poly, int x, int y)
{	// insert x,y as the last element
	if (poly.n<MAX_POLY)
	{
		poly.vertex[poly.n].x = x;
		poly.vertex[poly.n].y = y;
		poly.n++;
	}
}
void DrawerAdapter::InsertVertex(float_polygon_type &poly, float x, float y)
{	// insert x,y as the last element
	if (poly.n<MAX_POLY)
	{
		poly.vertex[poly.n].x = x;
		poly.vertex[poly.n].y = y;
		poly.n++;
	}
}
void DrawerAdapter::GetPoint(polygon_type polygon, int k, int &x, int &y)
{
	x = polygon.vertex[k].x;
	y = polygon.vertex[k].y;

}
void DrawerAdapter::LoadPolygon(polygon_type &polygon, edge_list_type &list, int &num_Edges)
{
	int x1, x2, y1, y2, k = 0;

	list.n = 0;
	GetPoint(polygon, k, x1, y1);
	num_Edges = 0;

	for (; k < polygon.n; k++)
	{
		GetPoint(polygon, (k + 1) % polygon.n, x2, y2);
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

void DrawerAdapter::Include(edge_list_type &list, int &end_Edge, int final_Edge, int scan)
{	// include all edges that intersept y_scan
	while (end_Edge < final_Edge && list.edge[end_Edge + 1].Ymax >= scan)
	{
		end_Edge++;
	}
}

void DrawerAdapter::FillIn(int x1, int x2, int y)
{
	int px, py;

	py = y % 8;

	if (x1 != x2)
	{
		for (int x = x1; x <= x2; x++)
		{
			px = x % 8;

			if (pattern[y % 8][x % 8]) DrawPixel(x, y);

		}
	}
}

void DrawerAdapter::UpdateXValues(edge_list_type &list, int last_Edge, int &start_Edge, int scan)
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
void DrawerAdapter::FillScan(edge_list_type &list, int end_Edge, int start_Edge, int scan)
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
void DrawerAdapter::XSort(edge_list_type &list, int start_Edge, int last_Edge)
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
void DrawerAdapter::FillPolygon(polygon_type &polygon, edge_list_type &list)
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

bool DrawerAdapter::Empty(int x, int y)
{
	return(GetPixels(x, y) == 0);
}

bool DrawerAdapter::Empty(int x, int y, int color)
{
	int colorPixel = GetPixels(x, y);
	return(colorPixel != color_trans_map[color]);
}

void DrawerAdapter::FloodFillRec(int x, int y)
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

void DrawerAdapter::FloodFillIterative(polygon_type poly, int color){
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

void DrawerAdapter::FillCircle(int r_x, int r_y, int color){
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

/************************/
/* Line Clipping        */
/************************/

float DrawerAdapter::wxs = 0.0, DrawerAdapter::wxh = 1.0, DrawerAdapter::wys = 0.0, DrawerAdapter::wyh = 1.0;  
float DrawerAdapter::vxs = 0.0, DrawerAdapter::vxh = 1.0, DrawerAdapter::vys = 0.0, DrawerAdapter::vyh = 1.0;
int DrawerAdapter::inside = 0, DrawerAdapter::botton = 1, DrawerAdapter::top = 2, DrawerAdapter::right = 4, DrawerAdapter::left = 8;
float DrawerAdapter::vwsx = 1, DrawerAdapter::vwsy = 1;
float DrawerAdapter::x_start = 0, DrawerAdapter::x_end = 0, DrawerAdapter::y_start = 0, DrawerAdapter::y_end = 0, DrawerAdapter::height = 0, DrawerAdapter::width = 0;

void DrawerAdapter::SetWindow(float x1, float x2, float y1, float y2)
{
	wxs = x1;
	wxh = x2;
	wys = y1;
	wyh = y2;
	vwsx = (vxh - vxs) / (wxh - wxs);
	vwsy = (vyh - vys) / (wyh - wys);
}

void DrawerAdapter::SetViewport(float x1, float x2, float y1, float y2)
{
	vxs = x1;
	vxh = x2;
	vys = y1;
	vyh = y2;
	vwsx = (vxh - vxs) / (wxh - wxs);
	vwsy = (vyh - vys) / (wyh - wys);
}

void DrawerAdapter::ViewingTransformation(float *x, float *y)
{
	*x = (*x - wxs)*vwsx + vxs;
	*y = (*y - wys)*vwsy + vys;
}

void DrawerAdapter::NormalizedToDevice(float xn, float yn, int *xd, int *yd)
{
	*xd = (int)(x_start + width*xn);
	*yd = (int)(y_end - (y_start + height*yn));
}

void DrawerAdapter::InverseViewingTransformation(float *x, float *y)
{
	*x = (*x - vxs) / vwsx + wxs;
	*y = (*y - vys) / vwsy + wys;
}

void DrawerAdapter::DeviceToNormalized(int xd, int yd, float *xn, float *yn)
{
	*xn = (xd - x_start) / (float)width;
	*yn = (y_end - yd - y_start) / (float)height;
}

void DrawerAdapter::XYEdgeIntersection(float *x1, float *x2, float *y1, float *y2,
	float wy, float *x, float *y)
{
	*x = *x1 + (*x2 - *x1)*(wy - *y1) / (*y2 - *y1);
	*y = wy;
}

void DrawerAdapter::SetCode2D(float *x, float *y, int *c)
{
	*c = inside;
	if (*x < wxs)
		*c |= left;
	else if (*x > wxh)
		*c |= right;
	if (*y < wys)
		*c |= botton;
	else if (*y > wyh)
		*c |= top;
}

bool DrawerAdapter::LineIntersectiom(float_point_type P1, float_point_type P2, win_edge_type edge)
{
	switch (edge)
	{
	case LEFT: return ((P1.x - wxs)*(P2.x - wxs)<0);
	case RIGHT: return ((P1.x - wxh)*(P2.x - wxh)<0);
	case TOP: return ((P1.y - wyh)*(P2.y - wyh)<0);
	case BOTTOM: return ((P1.y - wys)*(P2.y - wys)<0);
	}
	return false;
}
bool DrawerAdapter::Visible(float_point_type P, win_edge_type edge)
{
	switch (edge)
	{

	case LEFT: return (P.x >= wxs);
	case RIGHT: return (P.x <= wxh);
	case TOP: return (P.y <= wyh);
	case BOTTOM: return (P.y >= wys);
	}
	return false;
}
float_point_type DrawerAdapter::Intersection(float_point_type P1, float_point_type P2, win_edge_type edge)
{
	float_point_type p;
	switch (edge)
	{
	case LEFT:
		p.x = wxs;
		p.y = (P1.y + ((float)(P2.y - P1.y)) / (P2.x - P1.x)*(p.x - P1.x));
		break;
	case RIGHT:
		p.x = wxh;
		p.y = (P1.y + ((float)(P2.y - P1.y)) / (P2.x - P1.x)*(p.x - P1.x));
			break;
	case TOP: 
		p.y = wyh;
		p.x = (P1.x + ((float)(P2.x - P1.x)) / (P2.y - P1.y)*(p.y - P1.y));
		break;
	case BOTTOM: 
		p.y = wys;
		p.x = (P1.x + ((float)(P2.x - P1.x)) / (P2.y - P1.y)*(p.y - P1.y));
	}
	return p;
}

void DrawerAdapter::ClipEdge(float_point_type P1, float_point_type P2, win_edge_type edge, float_polygon_type &poly_out){
	float_point_type Pi;
	if (Visible(P1, edge)) // P is at the same side of window
		InsertVertex(poly_out, P1.x, P1.y);
	if (LineIntersectiom(P1, P2, edge))
	{
		Pi = Intersection(P1, P2, edge);
		InsertVertex(poly_out, Pi.x, Pi.y);
	}

}

void DrawerAdapter::ClipPolygon(float_polygon_type poly, float_polygon_type &poly_out){
	win_edge_type edge;
	for (int edg = 0; edg<4; edg++){
		poly_out.n = 0; // Reset poly_out	
		edge = (win_edge_type)edg;
		poly.vertex[poly.n] = poly.vertex[0];
		for (int i = 0; i<poly.n; i++)
			ClipEdge(poly.vertex[i], poly.vertex[i + 1], edge, poly_out);
		poly = poly_out;// Copy poly_out to poly
	}
}

bool DrawerAdapter::Clip2D(float *x1, float *y1, float *x2, float *y2)
{
	int c, c1, c2;
	float x, y;

	SetCode2D(x1, y1, &c1);
	SetCode2D(x2, y2, &c2);

	while ((c1 != inside) || (c2 != inside))
	{
		if ((c1&c2) != inside) return false;
		
		c = c1;
		if (c == inside) c = c2;

		if (left & c)
			XYEdgeIntersection(y1, y2, x1, x2, wxs, &y, &x);
		else if (right & c)
			XYEdgeIntersection(y1, y2, x1, x2, wxh, &y, &x);
		else if (botton & c)
			XYEdgeIntersection(x1, x2, y1, y2, wys, &x, &y);
		else if (top & c)
			XYEdgeIntersection(x1, x2, y1, y2, wyh, &x, &y);
		if (c == c1)
		{
			*x1 = x;
			*y1 = y;
			SetCode2D(x1, y1, &c1);
		}
		else
		{
			*x2 = x;
			*y2 = y;
			SetCode2D(x2, y2, &c2);
		}
	}
	return true;
}

void DrawerAdapter::InitGraf()
{
	y_end = (float)GetMaxY();
	x_end = (float)GetMaxX();

	x_start = 0.0f;
	y_start = 0.0f;
	width = (float)(x_end - x_start);
	height = (float)(y_end - y_start);
	SetViewport(0.0f, 1.0f, 0.0f, 1.0f);
	SetWindow(0.0f, (float)x_end, 0.0f, (float)y_end);
}


/*
void LineAbs2(float x2, float y2)

{
	float x1, y1;
	int xi1, yi1, xi2, yi2;

	x1 = x_current;
	y1 = y_current;
	x_current = x2;
	y_current = y2;

	ViewingTransformation(&x1, &y1);
	ViewingTransformation(&x2, &y2);
	NormalizedToDevice(x1, y1, &xi1, &yi1);
	NormalizedToDevice(x2, y2, &xi2, &yi2);
	//DrawLine(xi1, yi1, xi2, yi2);

}
void MoveAbs2D(float x, float y)

{
	x_current = x;
	y_current = y;
}


void MoveRel2D(float dx, float dy)

{
	x_current += dx;
	y_current += dy;
}
void DrawLine2(float x1, float y1, float x2, float y2, int color)
{
	SetGraphicsColor(color, 2);
	MoveAbs2D(x1, y1);
	LineAbs2(x2, y2);

}

void DrawLine2D(float x2, float y2)

{
	float x1, y1;
	int xi1, yi1, xi2, yi2;

	x1 = x_current;
	y1 = y_current;

	//TO DO <MATRIZ>.DoTransformation(&x1, &y1);
	//TO DO  Faça Transformacao x2 y2

		if (Clip2D(&x1, &y1, &x2, &y2))
		{
		ViewingTransformation(&x1, &y1);
		//TO DO ViewingTransformation for point 2
			NormalizedToDevice(x1, y1, &xi1, &yi1);
		//TO DO NormalizedToDevice for point 2
			DrawLine(xi1, yi1, xi2, yi2);
		}
}

void LineAbs2D(float x, float y)

{
	DrawLine2D(x, y);
	x_current = x;
	y_current = y;
}


void LineRel2D(float dx, float dy)

{
	dx += x_current;
	dy += y_current;
	DrawLine2D(dx, dy);
	x_current = dx;
	y_current = dy;
}
*/


