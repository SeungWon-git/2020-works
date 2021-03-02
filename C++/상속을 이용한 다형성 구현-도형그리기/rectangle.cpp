//-----------------------------------------------------------------------------
// Rectangle.cpp
//
// 2020. 11. 30	by Wulong 
//-----------------------------------------------------------------------------
#include <iostream>
#include "rectangle.h"

Rectangle::Rectangle() : p1(), p2()
{
}

Rectangle::Rectangle( const Point& a, const Point& b ) : p1( a ), p2( b )
{
};

Rectangle::Rectangle( const Rectangle& other ) : p1( other.p1 ), p2( other.p2 )
{
}

Rectangle::~Rectangle()
{
}

void Rectangle::draw() const
{
	std::cout << "»ç°¢Çü - (" << p1.x << "," << p1.y << "), (" << p2.x << "," << p2.y << ")" << std::endl;
}
