//-----------------------------------------------------------------------------
// Rectangle.h 		�簢���� �밢���� �ִ� �� 2���� ������ �� �ִ�
//
// 2020. 11. 30	by Wulong 
//-----------------------------------------------------------------------------
#include "point.h"
#include "shape.h"

class Rectangle : public Shape
{
	Point p1, p2;

public:

	Rectangle();
	Rectangle( const Point&, const Point& );
	Rectangle( const Rectangle& );
	~Rectangle();

	virtual void draw() const override;
};

