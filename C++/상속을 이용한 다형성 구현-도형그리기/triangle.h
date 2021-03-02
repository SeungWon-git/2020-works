//-----------------------------------------------------------------------------
// Triangle.h 		삼각형은 세 점으로 정의할 수 있다
//
// 2020. 11. 30	by Wulong 
//-----------------------------------------------------------------------------
#include "point.h"						// point를 Triangle에서 사용
#include "shape.h"						// Shape을 상속 받음	

class Triangle : public Shape
{
	Point p1, p2, p3;

public:

	Triangle();
	Triangle( const Point&, const Point&, const Point& );
	Triangle( const Triangle& );
	~Triangle();

	virtual void draw() const override;	// 가상함수 overriding
};

