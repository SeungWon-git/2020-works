#pragma warnings(disable 4996) 
#include <iostream>

using namespace std;

//1��
/*int main1() {
	char c = 'A' + 'a';		

	cout << static_cast<int>(c) << '\n';	//����� �����϶�. (-94)
}
//c = 'A' + 'a' �� = 65 + 97 = 162 => c���ٰ� 162�ְ� cout�ϸ� -94����
//��, 'A'�� �ڵ����� 65�κ�ȯ�Ǿ� ���Ե�. ���� c���� 0B1010'0010�� �� (���� char���·δ� �������� ����� ����)
//�̶� static_cast<int>�� �� �������� ������� ���������Ͱ� �Ǵ��Ͽ� 2�� ����ó���� �Ѵ�. ���� �ᱹ�� -94�� ���´�.


//2��
void change(char a, char b) {};

int main2() {
	char a{ 'O' };
	char z{ 'K' };

	change(a, z);

	cout << a << z << '!' << '\n';	//����� �����϶�. (OK!)
}
// "C���"(����...)���� ���Ͱ��̻������´� ���� ���� �ٲ��� �ʱ⶧���̴�. 


//3��
//��
void makeUpper(char&);

void makeUpper(char& a) {
	///f (a >= 97 && a < -122)
		//a -= 32;
	a = toupper(a);
}*/


//4��
//��	65535
int main() {
	int max = numeric_limits<int>::max();

	int n = 1;
	double sum = 0; //�������� �Ѿ�� ���� �������⶧���� �׺��� ��ũ��

	while (true) {
		sum += n;
		
		if (max < sum) {
			n--;
			break;
		}
		n++;
	}

	cout << "sum = " << sum << " n = " << n << endl;
}