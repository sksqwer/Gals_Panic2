#pragma once
#ifndef GP_GALS_PANIC_H_
#define GP_GALS_PANIC_H_
#include "framework.h"

FLOAT Distance(Point a, Point b);
BOOL on_the_line(Point v1, Point v2, Point p);
BOOL is_in_Polygon(std::vector<Point> v, Point p);
void Getminmax(Point a, Point b, int &minx, int &maxx, int &miny, int &maxy);
int Rel_linedot(Point a, Point b, Point c);
bool operator!=(const Point a, const Point b);
bool operator==(const Point a, const Point b);

class Gamemanager
{
private:
	enum { rectview_left = 0, rectview_right = 800, rectview_top = 0, rectview_bottom = 800};
	Image *CImage, *HImage, *PImage;
	std::vector<Point> inpointV;
	std::vector<Point> outpointV;
	std::vector<Point> outpointVertex;
	std::vector<Point> MoveV;
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
	void Combine_Polygon();

};


#endif // !GP_GALS_PANIC_H_
