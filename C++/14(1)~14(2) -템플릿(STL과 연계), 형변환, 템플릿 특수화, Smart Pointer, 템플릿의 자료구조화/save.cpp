#pragma warning(disable:4996)
#include <iostream>	// (in/out)����� ����� ����
#include <fstream>	//���� �а� ���� ����� ����
#include <chrono>	//�ð� ���ñ���� ����
#include <iomanip>	//����� ���� ���� ���� (put_time�Լ� ����)
#include <string>

void save(std::string fileName)
{
	//[����]���� ������ �о� ����غ���.

	//[����]

	//1.�Է������� ����.
	std::ifstream in{ fileName };

	if (!in) {
		std::cout << "������ ���� �����ϴ�\n";
	}

	//2.����� ������ ���ٿ� ���� ���� ����.
	std::string outFileName{ "2020�г� 2�б� C++ ��������.txt" };
	std::ofstream out{ outFileName ,std::ios::app };

	//3.��� ������ �ð��� ����Ѵ�.
	auto tp = std::chrono::system_clock::now();
	auto ct = std::chrono::system_clock::to_time_t(tp);
	auto lt = localtime(&ct);

	out.imbue(std::locale("korean"));
	out << "\n\n\n";
	out << "----------------------------------------------" << '\n';
	out << fileName << std::put_time(lt, " - %c - (%A)") << std::endl;
	out << "----------------------------------------------" << "\n\n";

	//4.�Է������� �о� ������Ͽ� ����Ѵ�.
	int c;

	while ((c = in.get()) != EOF)
		out.put(c);	//��ü�� out�� ����� ���Ͽ� ���

	std::cout << fileName << "�� " << outFileName << "�� �����Ͽ����ϴ�.\n";
}