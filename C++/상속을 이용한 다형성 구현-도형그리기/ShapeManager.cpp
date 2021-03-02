//-----------------------------------------------------------------------------
// ShapeManager.cpp 	������ �����ϴ� Ŭ����	
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
	nShape = 0;							// ó�� ������� ���� �����ϴ� ���� ������ 0��
	capacity = n;						// �ִ� n���� ������ ���� �� ����
	shapes = new Shape * [capacity];
}

//---------------------------
ShapeManager::~ShapeManager() 
//---------------------------
{
	// ��� ��ü�� ��Ȯ�ϰ� �����Ǵ��� �ݵ�� Ȯ���Ͽ��� �Ѵ�.

	for (int i = 0; i < nShape; ++i)
		delete shapes[i];

	delete[] shapes;					// ���������ڰ� �����ϴ� ������ �Ҹ��ڸ� ȣ����
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
	cout << "�����ϴ� ��� ������ �׸��ϴ�" << endl;
	cout << "�ִ� " << capacity << "���� ������ ���� �� �ֽ��ϴ�" << endl;
	cout << "��� " << nShape << "���� ������ �ֽ��ϴ�" << endl;
	cout << "------------------------------------------" << endl << endl;

	for (int i = 0; i < nShape; ++i) {
		cout << "[" << i << "]  ";
		shapes[i]->draw();				// ���⿡�� �������� �����ȴ�.
	}
	cout << endl;

	cout << "------------------------------------------" << endl;
	cout << "�׸��⸦ ��Ĩ�ϴ�" << endl;
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