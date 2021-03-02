//-----------------------------------------------------------------------------
// 다형성구현.cpp 
//
//	알아보고 익혀야 할 것 
//
//		- 다형성(Ploymorphism)이란 무엇인가?
//		- C++ 언어에서는 어떻게 다형성을 구현하는가?
//
// 2020. 11. 30	by Wulong 
//-----------------------------------------------------------------------------
#include "point.h"
#include "triangle.h"
#include "circle.h"
#include "rectangle.h"
#include "ShapeManager.h"

//--------
int main()
//--------
{
	//-----------------------------------------------
	// 이 프로그램은 프로젝트를 확인하기 위한 것이다.
	// 실습 순서대로 문제를 해결해 보자.
	//-----------------------------------------------


	// 1. 관리 클래스를 만듦
	ShapeManager sm( 100 );				// 최대 100개의 도형을 관리함


	// 2. 도형들을 관리 클래스에 삽입함
	sm.insert( new Triangle() );
	sm.insert( new Triangle( Point( 0, 0 ), Point( 1, 1 ), Point( 2, 2 ) ) );
	sm.insert( new Circle( Point( 1.23, 4.56 ), 7.89 ) );

	for (int i = 0; i < 10; ++i)
		sm.insert( new Rectangle( Point( i, i + 1 ), Point( i * 2, i * 3 ) ) );

	// 3. 관리하고 있는 모든 도형을 그림
	sm.draw();

	// 이 프로그램에서 잘못된 점을 찾을 수 있는가?

	sm.selectdelete(2);


	sm.~ShapeManager();
}
