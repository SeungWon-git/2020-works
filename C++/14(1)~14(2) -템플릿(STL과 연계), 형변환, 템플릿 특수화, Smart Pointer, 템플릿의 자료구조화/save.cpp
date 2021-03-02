#pragma warning(disable:4996)
#include <iostream>	// (in/out)입출력 기능은 여기
#include <fstream>	//파일 읽고 쓰기 기능은 여기
#include <chrono>	//시간 관련기능은 여기
#include <iomanip>	//입출력 조작 가능 여기 (put_time함수 포함)
#include <string>

void save(std::string fileName)
{
	//[문제]먼저 파일을 읽어 출력해보자.

	//[순서]

	//1.입력파일을 연다.
	std::ifstream in{ fileName };

	if (!in) {
		std::cout << "파일을 열수 없습니다\n";
	}

	//2.출력할 파일을 덧붙여 쓰기 모드로 연다.
	std::string outFileName{ "2020학년 2학기 C++ 강의저장.txt" };
	std::ofstream out{ outFileName ,std::ios::app };

	//3.출력 파일의 시간을 기록한다.
	auto tp = std::chrono::system_clock::now();
	auto ct = std::chrono::system_clock::to_time_t(tp);
	auto lt = localtime(&ct);

	out.imbue(std::locale("korean"));
	out << "\n\n\n";
	out << "----------------------------------------------" << '\n';
	out << fileName << std::put_time(lt, " - %c - (%A)") << std::endl;
	out << "----------------------------------------------" << "\n\n";

	//4.입력파일을 읽어 출력파일에 기록한다.
	int c;

	while ((c = in.get()) != EOF)
		out.put(c);	//객체에 out에 연결된 파일에 출력

	std::cout << fileName << "를 " << outFileName << "에 저장하였습니다.\n";
}