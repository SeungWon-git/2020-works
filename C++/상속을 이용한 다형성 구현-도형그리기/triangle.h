//-----------------------------------------------------------------------------
// Triangle.h 		�ﰢ���� �� ������ ������ �� �ִ�
//
// 2020. 11. 30	by Wulong 
//-----------------------------------------------------------------------------
#include "point.h"						// point�� Triangle���� ���
#include "shape.h"						// Shape�� ��� ����	

class Triangle : public Shape
{
	Point p1, p2, p3;

public:

	Triangle();
	Triangle( const Point&, const Point&, const Point& );
	Triangle( const Triangle& );
	~Triangle();

	virtual void draw() const override;	// �����Լ� overriding
};

