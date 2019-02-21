#pragma once
#include "screen.h"

//int min(int a, int b, int c) {
//	return (a > b) ? ((b > c) ? c : b) : ((a > c) ? c : a);
//}
//int max(int a, int b, int c) {
//	return (a > b) ? ((a > c) ? a : c) : ((b > c) ? b : c);
//}
//
//void swap(int &a, int &b) {
//	int c = a;
//	a = b;
//	b = c;
//}
//======== ���� shape.h -- ���������� ����� =========
//==1. ��������� ������ � ����� ������� �������� ==
char screen[YMAX][XMAX];
enum color { black = '*', white = '.' };
void screen_init()
{
	for (auto y = 0; y < YMAX; ++y)
		for (auto &x : screen[y])
			x = white;
}

void screen_destroy()
{
	for (auto y = 0; y < YMAX; ++y)
		for (auto &x : screen[y])
			x = black;
}

bool on_screen(int a, int b) // �������� ��������� �� �����
{
	return 0 <= a && a < XMAX && 0 <= b && b < YMAX;
}

void put_point(int a, int b)
{
	if (on_screen(a, b)) 
		screen[b][a] = black;
}

void put_line(int x0, int y0, int x1, int y1)
/*
��������� ������� ������ �� (x0,y0) �� (x1,y1).
��������� ������: b(x-x0) + a(y-y0) = 0.
�������������� �������� abs(eps),
��� eps = 2*(b(x-x0)) + a(y-y0) (�������� ���������� ��� ������).
*/
{
	int dx = 1;
	int a = x1 - x0;
	if (a < 0)
		dx = -1, a = -a;
	int dy = 1;
	int b = y1 - y0;
	if (b < 0) 
		dy = -1, b = -b;
	int two_a = 2 * a;
	int two_b = 2 * b;
	int xcrit = -b + two_a;
	int eps = 0;

	for (;;) { //������������ ������ ����� �� ������
		put_point(x0, y0);
		if (x0 == x1 && y0 == y1) 
			break;
		if (eps <= xcrit) 
			x0 += dx, eps += two_b;
		if (eps >= a || a < b)
			y0 += dy, eps -= two_a;
	}
}

void screen_clear() { screen_init(); } //������� ������

void screen_refresh() // ���������� ������
{
	for (int y = YMAX - 1; 0 <= y; --y) { // � ������� ������ �� ������
		for (auto x : screen[y])    // �� ������ ������� �� �������
			std::cout << x;
		std::cout << '\n';
	}
}

//== 2. ���������� ����� ==
struct shape { // ����������� ������� ����� "������"
	static shape* list; // ������ ������ ����� (���� �� ��� ������!)
	shape* next;
	shape() { next = list; list = this; } //������ �������������� � ������
	virtual point north() const = 0; //����� ��� ��������
	virtual point south() const = 0;
	virtual point east() const = 0;
	virtual point west() const = 0;
	virtual point neast() const = 0;
	virtual point seast() const = 0;
	virtual point nwest() const = 0;
	virtual point swest() const = 0;
	virtual void draw() = 0; //���������
	virtual void move(int, int) = 0; //�����������
	virtual void resize(int) = 0;//��������� �������
};

shape * shape::list = nullptr; //������������� ������ �����

class rotatable : virtual public shape { //������, ��������� � �������� 
public:
	virtual void rotate_left() = 0; //��������� �����
	virtual void rotate_right() = 0; //��������� ������
};
class reflectable : virtual public shape { // ������, ���������
	   // � ����������� ���������
public:
	virtual void flip_horisontally() = 0; // �������� �������������
	virtual void flip_vertically() = 0; // �������� �����������
};
class line : public shape {
	/* ������� ������ ["w", "e" ].
	north( ) ���������� ����� "���� ������ ������� � ��� ������
	�� �����, ��� ����� ��� �������� �����", � �. �. */
protected:
	point w, e;
public:
	line(point a, point b) : w(a), e(b) { };
	line(point a, int L) : w(point(a.x + L - 1, a.y)), e(a) {  };
	point north() const { return point((w.x + e.x) / 2, e.y < w.y ? w.y : e.y); }
	point south() const { return point((w.x + e.x) / 2, e.y < w.y ? e.y : w.y); }
	point east() const { return point(e.x < w.x ? w.x : e.x, (w.y + e.y) / 2); }
	point west() const { return point(e.x > w.x ? w.x : e.x, (w.y + e.y) / 2);}
	point neast() const { return point(e.x < w.x ? w.x : e.x, e.y < w.y ? w.y : e.y); }
	point seast() const { return point(e.x < w.x ? w.x : e.x, e.y < w.y ? e.y : w.y); }
	point nwest() const { return point(e.x > w.x ? w.x : e.x, e.y < w.y ? w.y : e.y); }
	point swest() const { return point(e.x > w.x ? w.x : e.x, e.y < w.y ? e.y : w.y); }
	void move(int a, int b) { w.x += a; w.y += b; e.x += a; e.y += b; }
	void draw() { put_line(w, e); }
	void resize(int d) // ���������� � (d) ���
	{
		e.x += (e.x - w.x) * (d - 1); 
		e.y += (e.y - w.y) * (d - 1);
	}
};
// �������������
class rectangle : public rotatable {
	/* nw ------ n ------ ne
	   |            |
	   |            |
	   w      c            e
	   |            |
	   |            |
	   sw ------ s ------ se */
protected:
	point sw, ne;
public:
	rectangle(point a, point b) : sw(a), ne(b) { }
	point north() const { return point((sw.x + ne.x) / 2, ne.y); }
	point south() const { return point((sw.x + ne.x) / 2, sw.y); }
	point east() const { return point(ne.x, (sw.y + ne.y) / 2); }
	point west() const { return point(sw.x, (sw.y + ne.y) / 2); }
	point neast() const { return ne; }
	point seast() const { return point(ne.x, sw.y); }
	point nwest() const { return point(sw.x, ne.y); }
	point swest() const { return sw; }
	void rotate_right() // ������� ������ ������������ se
	{
		int w = ne.x - sw.x, h = ne.y - sw.y;
		sw.x = ne.x - h * 2; ne.y = sw.y + w / 2;
	}
	void rotate_left() // ������� ����� ������������ sw
	{
		int w = ne.x - sw.x, h = ne.y - sw.y;
		ne.x = sw.x + h * 2; ne.y = sw.y + w / 2;
	}
	void move(int a, int b)
	{
		sw.x += a; sw.y += b; ne.x += a; ne.y += b;
	}
	void resize(int d)
	{
		ne.x += (ne.x - sw.x) * (d - 1); ne.y += (ne.y - sw.y) * (d - 1);
	}
	void draw();
};
void rectangle::draw()
{
	put_line(nwest(), ne);   put_line(ne, seast());
	put_line(seast(), sw);   put_line(sw, nwest());
}

void shape_refresh() // ����������� ���� �����
{
	screen_clear();
	for (shape* p = shape::list; p; p = p->next) p->draw();
	screen_refresh();
}

void up(shape& p, const shape& q) // ��������� p ��� q
{ //��� ������� �������, � �� ���� ������!
	point n = q.north();
	point s = p.south();
	p.move(n.x - s.x, n.y - s.y + 1);
}

//========================================================
// ������ �������: �������������� �������� - ��������������
class h_circle : public rectangle, public reflectable {
	bool reflected;
public:
	h_circle(point a, point b, bool r = true) : rectangle(a, b), reflected(r) { }
	void draw();
	void flip_horisontally() { }; // �������� �������������
	void flip_vertically() { reflected = !reflected; }; // �������� �����������
};

void h_circle::draw() //�������� ���������� ��� �����������
{
	int x0 = (sw.x + ne.x) / 2;
	int y0 = reflected ? sw.y : ne.y;
	int radius = (ne.x - sw.x) / 2;
	int x = 0;
	int y = radius;
	int delta = 2 - 2 * radius;
	int error = 0;
	while (y >= 0) {
		if (reflected) { put_point(x0 + x, y0 + y * 0.7); put_point(x0 - x, y0 + y * 0.7); }
		else { put_point(x0 + x, y0 - y * 0.7); put_point(x0 - x, y0 - y * 0.7); }
		error = 2 * (delta + y) - 1;
		if (delta < 0 && error <= 0) { ++x; delta += 2 * x + 1; continue; }
		error = 2 * (delta - x) - 1;
		if (delta > 0 && error > 0) { --y; delta += 1 - 2 * y; continue; }
		++x; delta += 2 * (x - y);  --y;
	}
}
// ������ �������: �������������� ������� ��������������
void down(shape &p, const shape &q)
{
	point n = q.south();
	point s = p.north();
	p.move(n.x - s.x, n.y - s.y - 1);
}
// C������ ������ - ����������
class myshape : public rectangle {
	//��� ������ �������� ���������������
	int w, h;
	line l_eye; // ����� ���� � ��� ������ �������� �����
	line r_eye; // ������ ����
	line mouth; // ���
public:
	myshape(point, point);
	void draw();
	void move(int, int);
	void resize(int) { }
};
myshape::myshape(point a, point b)
	: rectangle(a, b),
	w(neast().x - swest().x + 1),
	h(neast().y - swest().y + 1),
	l_eye(point(swest().x + 2, swest().y + h * 3 / 4), 2),
	r_eye(point(swest().x + w - 4, swest().y + h * 3 / 4), 2),
	mouth(point(swest().x + 2, swest().y + h / 4), w - 4)
{ }

void myshape::draw()
{
	rectangle::draw();
	int a = (swest().x + neast().x) / 2;
	int b = (swest().y + neast().y) / 2;
	put_point(point(a, b));
}
void myshape::move(int a, int b)
{
	rectangle::move(a, b);
	l_eye.move(a, b);
	r_eye.move(a, b);
	mouth.move(a, b);
}

//========================================================
//�������������� ������
class cross : public rotatable {
protected:
	line vert, hor;
public:
	cross(point o, int len);
	void move(int a, int b);
	void draw() {}
	void resize(int d);
	void rotate_left() {}
	void rotate_right() {}
	point north() const { return vert.north(); }
	point south() const { return vert.south(); }
	point east() const { return hor.east(); }
	point west() const { return hor.west(); }
	point neast() const { return point(hor.east().x, vert.north().y); }
	point seast() const { return point(hor.east().x, vert.south().y); }
	point nwest() const { return point(hor.west().x, vert.north().y); }
	point swest() const { return point(hor.west().x, vert.south().y); }
};

cross::cross(point o, int len) : vert(point(o.x, o.y - len), point(o.x, o.y + len)),
	hor(point(o.x - len, o.y), point(o.x + len, o.y)) {};

void cross::move(int a, int b) {
	vert.move(a, b);
	hor.move(a, b);
}

void cross::resize(int d) {
	vert.move((vert.north().y - vert.east().y), 0);
	vert.resize(d);
	hor.move(0, (hor.east().x - hor.north().x));
	hor.resize(d);
}

//������������� ����������� � �������
class triangle_with_cross : public cross {
protected:
	int quarter; //�������� ������������ ���������, � ������� ����� ������ ����
public:
	triangle_with_cross(point o, int len);
	void draw();
	void rotate_left();
	void rotate_right();
};

triangle_with_cross::triangle_with_cross(point o, int len) : cross(o, len), quarter(3) {};

void triangle_with_cross::draw() {
	//std::cout << "im here and quarter = " << quarter << std::endl;
	if (quarter == 3) {
		put_line(nwest(), seast());  //����������
		put_line(nwest(), swest()); //����. �����
		put_line(swest(), seast()); //���. �����
	}
	else if (quarter == 2) {
		put_line(swest(), neast()); //����������
		put_line(nwest(), swest()); //����. �����
		put_line(nwest(), neast()); //���. �����
	}
	else if (quarter == 1) {
		put_line(nwest(), seast()); //����������
		put_line(neast(), seast()); //����. �����
		put_line(nwest(), neast()); //���. �����
	}
	else if (quarter == 4) {
		put_line(swest(), neast()); //����������
		put_line(seast(), neast()); //����. �����
		put_line(swest(), seast()); //���. �����
	}
}

void triangle_with_cross::rotate_left() {
	if (++quarter == 5)
		quarter = 1;
}

void triangle_with_cross::rotate_right() {
	if (--quarter == 0)
		quarter = 4;
}

//��� ������� ������������� - ������ �����
void up_left(shape &p, const shape &q)
{
	point n = q.nwest();
	point s = p.swest();
	p.move(n.x - s.x, n.y - s.y + 1);
}
//��� ������� ������������� - ������ ������
void up_right(shape &p, const shape &q)
{
	point n = q.neast();
	point s = p.seast();
	p.move(n.x - s.x, n.y - s.y + 1);
}