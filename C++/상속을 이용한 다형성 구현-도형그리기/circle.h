//-----------------------------------------------------------------------------
// Circle.h		�߽����� ��ǥ�� ���������� ���� ������ �� �ִ�.
//				(���� �����ϴ� �ٸ� ����� �ִ�)
//
// 2020. 11. 30	by Wulong 
//-----------------------------------------------------------------------------
#include "point.h"
#include "shape.h"

// ���� �� include ��ſ� ������ ���� ���漱��(forward declaration)�� �� ���� �ִ�
// struct Point;
// class Shape;

class Circle : public Shape
{
	Point center;						// �߽����� ��ǥ
	double rad;							// ������	

public:

	Circle();

	Circle( const Point&, double );

	Circle( const Circle& );			// ���� ���縦 �� ���� ���ٸ� ���� ��������ڸ� ���α׷��� �ʿ�� ����.
										// ���� ���α׷��Ѵٸ� �޸𸮰� �״�� ����ǵ��� �ؾ��Ѵ�.
										// 

	// �� ������ ���� �ٰ� ���� �����Ͽ� ��ġ�� �� �ִ�
	// Circle(const Circle&) = default;

	~Circle();			// �Ҹ��ڵ� ���α׷��� �ʿ䰡 ���ٴ� ���� ������ ����

	virtual void draw() const override;
};

