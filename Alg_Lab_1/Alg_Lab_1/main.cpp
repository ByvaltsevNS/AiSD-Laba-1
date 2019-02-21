// ���������� ���������: // ���������� � ������������� ���������� �����
//#include "stdafx.h" //����� � �� (������ ��� Visual C++)
#include <iostream>
#include "shape.h"

int main() //������ ��� Visual C++ (����� � main( )) 
{
	setlocale(LC_ALL, "Rus");
	screen_init();

	//== 1.���������� ������ ����� ==
	rectangle hat(point(0, 0), point(14, 5));
	line brim(point(0, 15), 17);
	myshape face(point(15, 10), point(27, 18));
	//h_circle beard(point(40, 10), point(50, 20));
	triangle_with_cross t1(point(35, 20), 5);
	triangle_with_cross t2(point(35, 5), 5);
	triangle_with_cross t3(point(19, 2), 1);
	shape_refresh();
	std::cout << "=== Generated... ===\n";
	std::cin.get(); //�������� �������� �����
   //== 2.���������� ==
	hat.rotate_right();
	brim.resize(2);
	face.resize(2);
	//beard.flip_vertically();
	t2.rotate_left();
	t3.rotate_right();
	t3.resize(2);
	shape_refresh();
	std::cout << "=== Prepared... ===\n";
	std::cin.get(); //�������� ����������
   //== 3.������ ����������� ==
   // face.move(0, -10); // � �������� ���������
	up(brim, face);
	up(hat, brim);
	shape_refresh();
	up_right(t1, brim);
	up_left(t2, brim);
	down(t3, face);
	shape_refresh();
	std::cout << "=== Ready! ===\n";
	std::cin.get(); //�������� ���������
	screen_destroy();
	return 0;
}
