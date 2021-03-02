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
		cout << num <<" ����Ʈ �Ҵ�" <<endl;
	}

	TimerMonster(const TimerMonster& other) {
		num = other.num;
		p = new char[num];
		//memcpy(p, other.p, num);
	}

	//5�� Ǯ��
	TimerMonster& operator=(const TimerMonster& other) {
		if (this == &other)
			return *this;

		num = other.num;
		delete[] p;
		p = new char[num];
		return *this;
	}

	~TimerMonster() {
		cout << num << " ����Ʈ �Ҹ�" << endl;
		delete[] p;
	}

	int getNum()const {
		return num;
	};

	void special() {
		cout << "TImerMonster - �ý��� ����(�и���): " << num << endl;
		int n{ num};
		this_thread::sleep_for(chrono::milliseconds(n));
	}

	//SDL���� ���� ��� - 4�� sort �ذ��
	/*bool operator< (const TimerMonster& rhs) const {
		return num < rhs.num;
	}*/
};

int main() {

	{//1��
		TimerMonster tm;

		tm.special();
	}
	{//2��
		TimerMonster tm1;
		TimerMonster tm2 = tm1;

		tm1.special();
		tm2.special();
	}
	{//3��
		cout << "TimerMonster ��ü�� �?: ";
		int num;
		cin >> num;
		
		//for (int i = 0; i < num; ++i)
			//(TimerMonster{}).special(); //-> �ϳ��� ������ ����Ͱ� ����! �׷��� for�� �ȿ����� �����ϹǷ� X

		// [�߿�!] ����� �� �ϳ� ���� ���� ������ �Ѵ�.
		TimerMonster* p = new TimerMonster[num];

		for (int i = 0; i < num; ++i)
			p[i].special();

		delete[] p;
	}
	cout << endl << endl;
	{//4��
		TimerMonster monsters[10];

		//sort(begin(monsters), end(monsters)); //���� class �� bool operator<�� ���� �ߴٸ� �׳� �̷��� ���� ��� ���� ��� ���� 

		sort(begin(monsters), end(monsters), [](const TimerMonster& a, const TimerMonster& b) {
				return a.getNum() < b.getNum();
			});

		for (TimerMonster& tm : monsters)
			tm.special();
	}
	{//5��
		//�� sort��������
		//TimerMonster t;
		//t=a;
		//a=b;
		//b=t;	//=> ��۸� �����Ͱ� �����!(�Ŀ� ��ȯ�� �� �ȵ�) �׷��Ƿ� "���� �Ҵ� ������"�� �ʿ�!!
	}
}