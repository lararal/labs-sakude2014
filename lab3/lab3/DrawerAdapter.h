#pragma once
#include "stdafx.h"
#include <windows.h>
#include "PolyFill.h"
#include "grafbase.h"
#include <stdio.h>
#include <string.h>
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

class DrawerAdapter
{
public:
	static SHORT numXpixels;//640;			// size of window in X
	static SHORT numYpixels;//480             // size of window in Y
	static int start_text_y;
	static int end_text_y;					// size of input line in Y
	static int key_input;
	static int mouse_action;
	int static mouse_x, mouse_y;
	static char buffer[200];		
	HMENU menu, menu_draw, menu_color, menu_pattern;
	std::vector<std::vector<int>> pattern;

	COLORREF win_draw_color = RGB(255, 255, 255);  // current draw color
	static HBRUSH blackBrush;
	static HDC  hdc;                                   /* Presentation Space Handle */
	static HWND WinHandle;                /* Client area window handle */
	static  HPEN hpen;
	static int menu_item;
	
	typedef enum {
		MY_BLACK, MY_BLUE, MY_GREEN, MY_CYAN, MY_RED, MY_MAGENTA,
		MY_BROWN, MY_LIGHTGRAY, MY_DARKGRAY, MY_LIGHTBLUE, MY_LIGHTGREEN,
		MY_LIGHTCYAN, MY_LIGHTRED, MY_LIGHTMAGENTA, MY_YELLOW, MY_WHITE
	} my_color;
	std::vector<COLORREF> color_trans_map;

	DrawerAdapter();
	~DrawerAdapter();
	void UpdatePattern(int);

	static void SetMaxX(int);
	static void SetMaxY(int);
	void DrawPixel(int, int);
	void MenuBar();
	void InitGraphics();
	void CloseGraphics(void);
	int GetMaxX(void);
	int GetMaxY(void);
	void SetGraphicsColor(int new_color, int width);
	int GetPixels(int x, int y);
	void CheckGraphicsMsg(void);
	static void ClearString(char *str);
	static void EraseMessage();
	static void  PrintMessage(char *buffer);
	void DDA(int x1, int y1, int x2, int y2);
	void DrawXorPixel(int x, int y);
	void DrawXorLine(int x1, int y1, int x2, int y2);
	void Bresenham(int x1, int y1, int x2, int y2);
	void PlotXorCircle(int xc, int yc, int x, int y);
	void PlotCircle(int xc, int yc, int x, int y);
	void CircleXorBresenham(int xc, int yc, int r);
	void CircleBresenham(int xc, int yc, int r);
	int distance(int x1, int y1, int x2, int y2);
	static LRESULT CALLBACK WinProc(HWND hWnd, UINT messg, WPARAM wParam, LPARAM lParam);
	void PolyInsert(edge_list_type &list, int x1, int y1, int x2, int y2);
	void InsertVertex(polygon_type &poly, int x, int y);
	void GetPoint(polygon_type polygon, int k, int &x, int &y);
	void LoadPolygon(polygon_type &polygon, edge_list_type &list, int &num_Edges);
	void Include(edge_list_type &list, int &end_Edge, int final_Edge, int scan);
	void FillIn(int x1, int x2, int y);
	void UpdateXValues(edge_list_type &list, int last_Edge, int &start_Edge, int scan);
	void FillScan(edge_list_type &list, int end_Edge, int start_Edge, int scan);
	void XSort(edge_list_type &list, int start_Edge, int last_Edge);
	void FillPolygon(polygon_type &polygon, edge_list_type &list);
	bool Empty(int x, int y);
	bool Empty(int x, int y, int color);
	void FloodFillRec(int x, int y);
	void FloodFillIterative(polygon_type poly, int color);
	void FillCircle(int r_x, int r_y, int color);
		
};

