#pragma warning(disable:4996)
#include <iostream>	
#include <random>	
#include <thread>
#include <chrono>
using namespace std;

default_random_engine dre;
uniform_int_distribution<> uid{ 10,1000 };

class TimerMonster {
	int num;
	char* p;

public:
	TimerMonster() :num{uid(dre)} {
		p = new char[num];
		cout << num <<" 바이트 할당" <<endl;
	}

	TimerMonster(const TimerMonster& other) {
		num = other.num;
		p = new char[num];
		//memcpy(p, other.p, num);
	}

	//5번 풀이
	TimerMonster& operator=(const TimerMonster& other) {
		if (this == &other)
			return *this;

		num = other.num;
		delete[] p;
		p = new char[num];
		return *this;
	}

	~TimerMonster() {
		cout << num << " 바이트 소멸" << endl;
		delete[] p;
	}

	int getNum()const {
		return num;
	};

	void special() {
		cout << "TImerMonster - 시스템 정지(밀리초): " << num << endl;
		int n{ num};
		this_thread::sleep_for(chrono::milliseconds(n));
	}

	//SDL에서 쓰는 방법 - 4번 sort 해결법
	/*bool operator< (const TimerMonster& rhs) const {
		return num < rhs.num;
	}*/
};

int main() {

	{//1번
		TimerMonster tm;

		tm.special();
	}
	{//2번
		TimerMonster tm1;
		TimerMonster tm2 = tm1;

		tm1.special();
		tm2.special();
	}
	{//3번
		cout << "TimerMonster 객체를 몇개?: ";
		int num;
		cin >> num;
		
		//for (int i = 0; i < num; ++i)
			//(TimerMonster{}).special(); //-> 하나를 여러번 만든것과 같다! 그래서 for문 안에서만 생존하므로 X

		// [중요!] 방법은 딱 하나 프리 스토어에 만들어야 한다.
		TimerMonster* p = new TimerMonster[num];

		for (int i = 0; i < num; ++i)
			p[i].special();

		delete[] p;
	}
	cout << endl << endl;
	{//4번
		TimerMonster monsters[10];

		//sort(begin(monsters), end(monsters)); //만약 class 내 bool operator<를 정의 했다면 그냥 이렇게 람다 사용 없이 사용 가능 

		sort(begin(monsters), end(monsters), [](const TimerMonster& a, const TimerMonster& b) {
				return a.getNum() < b.getNum();
			});

		for (TimerMonster& tm : monsters)
			tm.special();
	}
	{//5번
		//위 sort과정에서
		//TimerMonster t;
		//t=a;
		//a=b;
		//b=t;	//=> 댕글링 포인터가 생긴다!(후에 반환이 잘 안됨) 그러므로 "복사 할당 연산자"가 필요!!
	}
}