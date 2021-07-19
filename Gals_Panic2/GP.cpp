#include "GP.h"

FLOAT Distance(Point a, Point b)
{
	return sqrt(pow(a.X - b.X, 2) + pow(a.Y - b.Y, 2));
}

BOOL on_the_line(Point v1, Point v2, Point p)
{
	if (v1 == p)
		return false;

	FLOAT sumlen = Distance(v1, v2);
	FLOAT len1 = Distance(v1, p);
	FLOAT len2 = Distance(v2, p);

	if (len1 + len2 == sumlen)
		return true;
	else
		return false;
}

BOOL on_the_line2(Point v1, Point v2, Point p)
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
	int left = 0, right = 0, top = 0, bottom = 0;
	int vsize = v.size();
	for (int i = 0; i < vsize; i++)
	{
		Point a = v[i], b, temp;
		if (i == v.size() - 1)
			b = v[0];
		else
			b = v[i + 1];

		temp.X = 0;
		temp.Y = p.Y;
		if (is_ovelap_line(a, b, temp, p))
			left++;

		temp.X = p.X;
		temp.Y = 0;
		if (is_ovelap_line(a, b, temp, p))
			top++;

		temp.X = 800;
		temp.Y = p.Y;
		if (is_ovelap_line(a, b, temp, p))
			right++;

		temp.X = p.X;
		temp.Y = 800;
		if (is_ovelap_line(a, b, temp, p))
			bottom++;

	}
	if (left % 2 && top % 2 && right % 2 && bottom % 2)
		return true;
	else
		return false;
}
int Rel_linedot(Point a, Point b, Point c)
{
	int res = (a.Y - b.Y) * c.X + (b.X - a.X) * c.Y + a.X * b.Y - b.X * a.Y;
	return res;
}

int CCW(Point a, Point b, Point c)
{
	int op = a.X * b.Y + b.X * c.Y + c.X + a.Y;
	op -= (a.Y * b.X + b.Y * c.X + c.Y * a.X);
	if (op > 0) return 1;
	else if (op == 0) return 0;
	else return -1;
}

bool is_ovelap_line(Point a, Point b, Point c, Point d)
{
	int abc = getArea(a, b, c);
	int abd = getArea(a, b, d);
	int cda = getArea(c, d, a);
	int cdb = getArea(c, d, b);

	if ((abc*abd <= 0) && (cda*cdb <= 0))
	{
		return true;
	}
	else
		return false;
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
			Pen pen(Color(255, 255, 255, 255));
			graphics2.DrawLine(&pen, outpointVertex[i], outpointVertex[i + 1]);
//			graphics2.DrawEllipse(&pen, outpointVertex[i].X, outpointVertex[i].Y, 1, 1);
		}
		graphics2.DrawLine(&pen, outpointVertex[PVsize - 1], pointer);
//		graphics2.DrawEllipse(&pen, outpointVertex[PVsize - 1].X - 2, outpointVertex[PVsize - 1].Y - 2, 4, 4);

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
			space_move();
			pre = pointer;
		}
		else if (GetAsyncKeyState(VK_LEFT) & 0x8000) {
			pointer.X--;
			space_move();
			pre = pointer;
		}
		if (GetAsyncKeyState(VK_UP) & 0x8000) {
			pointer.Y--;
			space_move();
			pre = pointer;
		}
		else if (GetAsyncKeyState(VK_DOWN) & 0x8000) {
			pointer.Y++;
			space_move();
			pre = pointer;
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
	if (outpointV.empty())
	{
		pointer_v.X = 0;
		pointer_v.Y = 0;
	}
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
				return true;
		}
		if (on_the_line(outpointVertex[PVsize - 1], outpointVertex[0], pointer))
			return true;
	}
	return false;
}

void Gamemanager::space_move()
{
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
			outpointVertex.push_back(pre);
			outpointVertex.push_back(pointer);
			Combine_Polygon();

			outpointVertex.clear();
			outpointV.clear();

			pointer_v.X = 0;
			pointer_v.Y = 0;
		}
		



	}
	else if (is_in_Polygon(inpointV, pointer) || is_outmove())
	{
		pointer = pre;
	}
	else
	{
		Point v = { pointer.X - pre.X, pointer.Y - pre.Y };

		if (v == pointer_v)
		{
			outpointV.push_back(pre);
		}
		else
		{
			outpointV.push_back(pre);
			outpointVertex.push_back(pre);
			pointer_v = v;
		}

	}

	/*else if (is_outmove())
	{
		Point v = { pointer.X - pre.X, pointer.Y - pre.Y };

		if (v == pointer_v)
		{
			outpointV.push_back(pre);
		}
		else
		{
			outpointV.push_back(pre);
			outpointVertex.push_back(pre);
			pointer_v = v;
		}

	}
	else
	{
		pointer = pre;
	}*/
}

void Gamemanager::Combine_Polygon()
{
	// CombineV

	std::vector<Point> CombineV;
	int PVsize = outpointVertex.size();
	int Area = getArea(outpointVertex);
   	if (Area > 0)
	{
		for (int i = PVsize - 1; i >= 0; i--)
		{
			CombineV.push_back(outpointVertex[i]);
		}
	}
	else
	{
		for (int i = 0; i < PVsize; i++)
		{
			CombineV.push_back(outpointVertex[i]);
		}
	}
	Point StartP = CombineV[0], endP = CombineV.back();
	Point startLine, endLine;
	int index_startLine, index_endLine;
	
	PVsize = inpointV.size();
	if (PVsize > 0)
	{
		for (int i = 0; i < PVsize - 1; i++)
		{
			if (on_the_line(inpointV[i], inpointV[i + 1], StartP))
			{
				index_startLine = i;
				startLine = inpointV[i];
			}

			if (on_the_line(inpointV[i], inpointV[i + 1], endP))
			{
				index_endLine = i;
				endLine = inpointV[i];
			}
		}
		if (on_the_line(inpointV[PVsize - 1], inpointV[0], StartP))
		{
			index_startLine = PVsize - 1;
			startLine = inpointV[PVsize - 1];
		}

		if (on_the_line(inpointV[PVsize - 1], inpointV[0], endP))
		{
			index_endLine = PVsize - 1;
			endLine = inpointV[PVsize - 1];
		}
	}
	
	if (index_endLine == index_startLine)
	{

		if (!on_the_line(StartP, startLine, endP))
		{
			PVsize = inpointV.size();
			for (int i = (index_endLine + 1) % PVsize; i != (index_endLine) % PVsize; i = ++i % PVsize)
			{
				CombineV.push_back(inpointV[i]);
			}
			CombineV.push_back(inpointV[index_endLine]);

		}
	}
	else
	{
		PVsize = inpointV.size();
		for (int i = (index_endLine + 1) % PVsize; i != (index_startLine + 1) % PVsize; i = ++i % PVsize)
		{
			CombineV.push_back(inpointV[i]);
		}
	}

		inpointV.clear();



		for (int i = 0; i < CombineV.size(); i++)
			inpointV.push_back(CombineV[i]);





}

int getArea(std::vector<Point> v)
{
	int Area = 0;
	for (int i = 0; i < v.size(); i++)
	{
		Point a = v[i], b;
		if (i == v.size() - 1)
			b = v[0];
		else
			b = v[i + 1];

		if(b.X != a.X)
			Area += (b.X - a.X) * (a.Y +  b.Y) * 0.5;
	}

	return Area;
}

int getArea(Point a, Point b, Point c)
{
	int Area = 0;
	if (a.X != b.X)
		Area += (b.X - a.X) * (a.Y + b.Y);
	if (b.X != c.X)
		Area += (c.X - b.X) * (b.Y + c.Y);
	if (c.X != a.X)
		Area += (a.X - c.X) * (c.Y + a.Y);

	return Area;
}
