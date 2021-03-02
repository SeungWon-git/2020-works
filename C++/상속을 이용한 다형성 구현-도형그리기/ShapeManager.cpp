//-----------------------------------------------------------------------------
// ShapeManager.cpp 	도형을 관리하는 클래스	
//
// 2020. 11. 30	by Wulong 
//-----------------------------------------------------------------------------
#include <iostream>
#include "shapeManager.h"
using namespace std;

//---------------------------------
ShapeManager::ShapeManager( int n )
//---------------------------------
{
	nShape = 0;							// 처음 만들어질 때는 관리하는 도형 갯수가 0임
	capacity = n;						// 최대 n개의 도형을 담을 수 있음
	shapes = new Shape * [capacity];
}

//---------------------------
ShapeManager::~ShapeManager() 
//---------------------------
{
	// 모든 객체가 정확하게 삭제되는지 반드시 확인하여야 한다.

	for (int i = 0; i < nShape; ++i)
		delete shapes[i];

	delete[] shapes;					// 도형관리자가 관리하는 도형의 소멸자를 호출함
};

//-----------------------------------
void ShapeManager::insert( Shape* a )
//-----------------------------------
{
	shapes[nShape] = a;
	nShape++;
};

//-----------------------------
void ShapeManager::draw() const
//-----------------------------
{
	cout << "------------------------------------------" << endl;
	cout << "관리하는 모든 도형을 그립니다" << endl;
	cout << "최대 " << capacity << "개의 도형을 담을 수 있습니다" << endl;
	cout << "모두 " << nShape << "개의 도형이 있습니다" << endl;
	cout << "------------------------------------------" << endl << endl;

	for (int i = 0; i < nShape; ++i) {
		cout << "[" << i << "]  ";
		shapes[i]->draw();				// 여기에서 다형성이 구현된다.
	}
	cout << endl;

	cout << "------------------------------------------" << endl;
	cout << "그리기를 마칩니다" << endl;
	cout << "------------------------------------------" << endl << endl;
};

//-----------------------------
void ShapeManager:: selectdelete(int n)
//-----------------------------
{
	for (int i = 0; i < nShape; ++i) {
		if (n == i) {
			shapes[i]->draw();
			delete shapes[i];
		}
	}
	delete[] shapes;
}