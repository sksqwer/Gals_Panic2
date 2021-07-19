#pragma once
#ifndef GP_GALS_PANIC_H_
#define GP_GALS_PANIC_H_
#include "framework.h"

FLOAT Distance(Point a, Point b);
BOOL on_the_line(Point v1, Point v2, Point p);
BOOL on_the_line2(Point v1, Point v2, Point p);
BOOL is_in_Polygon(std::vector<Point> v, Point p);
void Getminmax(Point a, Point b, int &minx, int &maxx, int &miny, int &maxy);
int Rel_linedot(Point a, Point b, Point c); 
int CCW(Point a, Point b, Point c);
bool is_ovelap_line(Point a1, Point a2, Point b1, Point b2);
bool operator!=(const Point a, const Point b);
bool operator==(const Point a, const Point b);
int getArea(std::vector<Point>);
int getArea(Point a, Point b, Point c);

class Gamemanager
{
private:
	enum { rectview_left = 0, rectview_right = 800, rectview_top = 0, rectview_bottom = 800};
	Image *CImage, *HImage, *PImage;
	std::vector<Point> inpointV;
	std::vector<Point> outpointV;
	std::vector<Point> outpointVertex;
	Point pointer;
	Point pre;
	Point pointer_v = { 0, 0 };
public:
	void Gamemanager_shutdown();
	void set();
	void GameScreen(HWND hWnd, HDC hdc);
	void Update();
	void MovePointer();
	void BackPoiner();
	bool is_inmove();
	bool is_outmove();
	void space_move();
	void Combine_Polygon();
};


#endif // !GP_GALS_PANIC_H_
