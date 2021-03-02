//-----------------------------------------------------------------------------
// Circle.cpp
//
// 2020. 11. 30	by Wulong 
//-----------------------------------------------------------------------------
#include <iostream>
#include "circle.h"

Circle::Circle() : center(), rad( 0.0 )
{
}

Circle::Circle( const Point& c, double r ) : center( c ), rad( r )
{
}


// 복사생성자를 프로그램할 이유가 있다면 멤버변수의 값을 복사해야한다.
// 복사생성자를 프로그램하면서 아무것도 하지 않으면 멤버변수는 복사되지 않는다.

Circle::Circle( const Circle& other ) : center( other.center ), rad( other.rad )
{
}

Circle::~Circle()
{
}

void Circle::draw() const
{
	// 그림을 그리는 대신 내용을 문자로 출력한다
	std::cout << "원 - 중심점(" << center.x << "," << center.y << ") 반지름 " << rad << std::endl;
};
