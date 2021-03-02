//-----------------------------------------------------------------------------
// Circle.h		중심점의 좌표와 반지름으로 원을 정의할 수 있다.
//				(원을 정의하는 다른 방법도 있다)
//
// 2020. 11. 30	by Wulong 
//-----------------------------------------------------------------------------
#include "point.h"
#include "shape.h"

// 위의 두 include 대신에 다음과 같이 전방선언(forward declaration)을 할 수도 있다
// struct Point;
// class Shape;

class Circle : public Shape
{
	Point center;						// 중심점의 좌표
	double rad;							// 반지름	

public:

	Circle();

	Circle( const Point&, double );

	Circle( const Circle& );			// 깊은 복사를 할 일이 없다면 내가 복사생성자를 프로그램할 필요는 없다.
										// 만약 프로그램한다면 메모리가 그대로 복사되도록 해야한다.
										// 

	// 위 설명을 다음 줄과 같이 선언하여 대치할 수 있다
	// Circle(const Circle&) = default;

	~Circle();			// 소멸자도 프로그램할 필요가 없다는 것을 생각해 보자

	virtual void draw() const override;
};

