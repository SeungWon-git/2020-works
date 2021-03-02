#pragma warnings(disable 4996) 
#include <iostream>

using namespace std;

//1번
/*int main1() {
	char c = 'A' + 'a';		

	cout << static_cast<int>(c) << '\n';	//결과를 설명하라. (-94)
}
//c = 'A' + 'a' 는 = 65 + 97 = 162 => c에다가 162넣고 cout하면 -94나옴
//즉, 'A'는 자동으로 65로변환되어 대입됨. 따라서 c에는 0B1010'0010이 들어감 (따라서 char형태로는 이진수라서 출력을 못함)
//이때 static_cast<int>로 위 이진수를 정수라고 인터프리터가 판단하여 2의 보수처리를 한다. 따라서 결국엔 -94가 나온다.


//2번
void change(char a, char b) {};

int main2() {
	char a{ 'O' };
	char z{ 'K' };

	change(a, z);

	cout << a << z << '!' << '\n';	//결과를 설명하라. (OK!)
}
// "C언어"(함정...)에서 위와같이생긴형태는 절대 값이 바뀌지 않기때문이다. 


//3번
//답
void makeUpper(char&);

void makeUpper(char& a) {
	///f (a >= 97 && a < -122)
		//a -= 32;
	a = toupper(a);
}*/


//4번
//답	65535
int main() {
	int max = numeric_limits<int>::max();

	int n = 1;
	double sum = 0; //정수값을 넘어가면 무한 루프돌기때문에 그보다 더크게

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