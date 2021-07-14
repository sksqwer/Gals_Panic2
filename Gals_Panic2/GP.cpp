#include "GP.h"

FLOAT Distance(Point a, Point b)
{
	return sqrt(pow(a.X - b.X, 2) + pow(a.Y - b.Y, 2));
}

BOOL on_the_line(Point v1, Point v2, Point p)
{
	FLOAT sumlen = Distance(v1, v2);
	FLOAT len1 = Distance(v1, p);
	FLOAT len2 = Distance(v2, p);

	if (len1 + len2 == sumlen)
		return true;
	else
		return false;
}

BOOL is_in_Polygon(std::vector<Point> v, Point p)
{
	bool left = false, right = false, top = false, bottom = false;
	int minx, maxx, miny, maxy;
	int temp;
	int vsize = v.size();
	Point a, b;
	for (int i = 0; i < vsize; i++)
	{
		if (i == vsize - 1)
		{
			a = v[i];
			b = v[0];
		}
		else
		{
			a = v[i];
			b = v[i + 1];
		}


		Getminmax(a, b, minx, maxx, miny, maxy);
		temp = Rel_linedot(a, b, p);

		if (temp == 0) return true;

		if (p.X >= minx && p.X <= maxx)
		{
			if (temp > 0)
				bottom = true;
			else
				top = true;
		}
		if (p.Y >= miny && p.Y <= maxy)
		{
			if (temp > 0)
				right = true;
			else
				left = true;
		}

	}
	if (top && bottom && left && right)
		return true;


	return false;
}
int Rel_linedot(Point a, Point b, Point c)
{
	int res = (a.Y - b.Y) * c.X + (b.X - a.X) * c.Y + a.X * b.Y - b.X * a.Y;
	return res;
}


void Getminmax(Point a, Point b, int & minx, int & maxx, int & miny, int & maxy)
{
	if (a.X > b.X)
	{
		minx = b.X;
		maxx = a.X;
	}
	else
	{
		minx = a.X;
		maxx = b.X;
	}

	if (a.Y > b.Y)
	{
		miny = b.Y;
		maxy = a.Y;
	}
	else
	{
		miny = a.Y;
		maxy = b.Y;
	}
}

bool operator!=(const Point a, const Point b)
{
	if (a.X == b.X && a.Y == b.Y)
		return false;
	else
		return true;
}

bool operator==(const Point a, const Point b)
{
	if (a.X == b.X && a.Y == b.Y)
		return true;
	else
		return false;
}
void Gamemanager::Gamemanager_shutdown()
{
	delete CImage;
	delete HImage;
	delete PImage;
}

void Gamemanager::set()
{
	//image load
	HImage = new Image(L"images/random.bmp");
	CImage = new Image(L"images/cover.bmp");
	PImage = new Image(L"images/pointer.bmp");
	// get polygon
	srand((unsigned int)time(NULL));

	pointer.X = rand() % 400 + 200;
	pointer.Y = rand() % 400 + 200;

	Point temp = pointer;
	inpointV.push_back(temp);
	int random = rand() % 90 + 10;
	temp.X += random;
	inpointV.push_back(temp);
	temp.Y += rand() % 90 + 10;
	inpointV.push_back(temp);
	temp.X -= random;
	inpointV.push_back(temp);
}

void Gamemanager::GameScreen(HWND hWnd, HDC hdc)
{
	HDC memDC = CreateCompatibleDC(hdc);
	HBITMAP p, old;
	int bx = rectview_right, by = rectview_bottom;
	p = CreateCompatibleBitmap(hdc, bx, by);
	old = (HBITMAP)SelectObject(memDC, p);
	Graphics graphics(memDC);

	graphics.SetCompositingQuality(CompositingQuality::CompositingQualityAssumeLinear);
	graphics.SetSmoothingMode(SmoothingMode::SmoothingModeAntiAlias);
	graphics.SetInterpolationMode(InterpolationMode::InterpolationModeHighQualityBicubic);
	graphics.SetPixelOffsetMode(PixelOffsetMode::PixelOffsetModeHighQuality);
	//cover image
	graphics.DrawImage(CImage, 0, 0);

	// image 자르고 붙이기

	int pVsize = inpointV.size();
	Point *polyPoints = new Point[pVsize];
	for (int i = 0; i < pVsize; i++)
	{
		polyPoints[i] = inpointV[i];
	}
	GraphicsPath path;
	path.AddPolygon(polyPoints, pVsize);
	Region region(&path);
	Pen pen(Color(255, 0, 0, 0));
	graphics.DrawPath(&pen, &path);
	graphics.SetClip(&region);

	graphics.DrawImage(HImage, 0, 0);
	delete[] polyPoints;
	
	Graphics graphics2(memDC);
	graphics2.SetCompositingQuality(CompositingQuality::CompositingQualityAssumeLinear);
	graphics2.SetSmoothingMode(SmoothingMode::SmoothingModeAntiAlias);
	graphics2.SetInterpolationMode(InterpolationMode::InterpolationModeHighQualityBicubic);
	graphics2.SetPixelOffsetMode(PixelOffsetMode::PixelOffsetModeHighQuality);


	//drawline

	int PVsize = outpointVertex.size();
	if (PVsize > 0)
	{
		for (int i = 0; i < PVsize - 1; i++)
		{
			Pen pen(Color(255, 0, 0, 0));
			graphics2.DrawLine(&pen, outpointVertex[i], outpointVertex[i + 1]);
			graphics2.DrawEllipse(&pen, outpointVertex[i].X, outpointVertex[i].Y, 1, 1);
		}
		graphics2.DrawLine(&pen, outpointVertex[PVsize - 1], pointer);
		graphics2.DrawEllipse(&pen, outpointVertex[PVsize - 1].X - 2, outpointVertex[PVsize - 1].Y - 2, 4, 4);

	}


	//Pointer Image
	ImageAttributes imAtt;
	imAtt.SetColorKey(Color(255, 0, 255), Color(255, 0, 255), ColorAdjustTypeBitmap);
	graphics2.DrawImage(PImage,
		Rect(pointer.X - 5, pointer.Y - 5, 10, 10),
		0, 0, 10, 10,
		UnitPixel,
		&imAtt);

	BitBlt(hdc, 0, 0, bx, by, memDC, 0, 0, SRCCOPY);
	SelectObject(memDC, old);
	DeleteDC(memDC);
}

void Gamemanager::Update()
{
	DWORD newTime = GetTickCount();
	static DWORD oldTime = newTime;
	if (newTime - oldTime < 16) return;
	oldTime = newTime;

	MovePointer();
}

void Gamemanager::MovePointer()
{
	BOOL SPACE = false;
	if (GetAsyncKeyState(VK_SPACE) & 0x8000)
		SPACE = true;
	int velocity = 1;

	if (!SPACE)
	{
		pre = pointer;
		if (GetAsyncKeyState(VK_RIGHT) & 0x8000) {
			pointer.X++;
		}
		else if (GetAsyncKeyState(VK_LEFT) & 0x8000) {
			pointer.X--;
		}
		if (pointer != pre  && !is_inmove())
			pointer = pre;
		pre = pointer;
		if (GetAsyncKeyState(VK_UP) & 0x8000) {
			pointer.Y--;
		}
		else if (GetAsyncKeyState(VK_DOWN) & 0x8000) {
			pointer.Y++;
		}

		if (pointer == pre)
		{
			if (outpointV.size() > 0)
			{
				BackPoiner();
			}
		}
		else if(!is_inmove())
			pointer = pre;
	}
	else
	{
		pre = pointer;
		if (GetAsyncKeyState(VK_RIGHT) & 0x8000) {
			pointer.X++;
		}
		if (GetAsyncKeyState(VK_LEFT) & 0x8000) {
			pointer.X--;
		}
		if (GetAsyncKeyState(VK_UP) & 0x8000) {
			pointer.Y--;
		}
		if (GetAsyncKeyState(VK_DOWN) & 0x8000) {
			pointer.Y++;
		}

		if (pointer == pre)
		{
			if (outpointV.size() > 0)
			{
				BackPoiner();
			}
		}
		else if (is_inmove())
		{
			if (outpointVertex.size() > 0)
			{
				pointer_v.X = 0;
				pointer_v.Y = 0;
			}
			else
			{
				pointer = pre;
			}




		}
		else if(is_outmove() && !is_in_Polygon(inpointV, pointer))
		{
			Point v = { pointer.X - pre.X, pointer.Y - pre.Y };

			if (v == pointer_v)
			{
				outpointV.push_back(pointer);
			}
			else
			{
				outpointV.push_back(pointer);
				outpointVertex.push_back(pointer);
				pointer_v = v;
			}


		}
		else
		{
			pointer = pre;
		}




	}



}

void Gamemanager::BackPoiner()
{
	pointer = outpointV.back();
	if (outpointV.back() == outpointVertex.back())
	{
		outpointVertex.erase(outpointVertex.end() - 1);
	}
	outpointV.erase(outpointV.end() - 1);
}

bool Gamemanager::is_inmove()
{
	int PVsize = inpointV.size();
	if (PVsize > 0)
	{
		for (int i = 0; i < PVsize - 1; i++)
		{
			if (on_the_line(inpointV[i], inpointV[i + 1], pointer))
				return true;
		}
		if (on_the_line(inpointV[PVsize - 1], inpointV[0], pointer))
			return true;
	}

	return false;
}

bool Gamemanager::is_outmove()
{
	int PVsize = outpointVertex.size();
	if (PVsize > 0)
	{
		for (int i = 0; i < PVsize - 1; i++)
		{
			if (on_the_line(outpointVertex[i], outpointVertex[i + 1], pointer))
				return false;
		}
		if (on_the_line(outpointVertex[PVsize - 1], pre, pointer))
			return false;
	}
	return true;
}

void Gamemanager::Combine_Polygon()
{

}
