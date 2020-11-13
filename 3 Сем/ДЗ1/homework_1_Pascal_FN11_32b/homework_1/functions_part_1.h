#pragma once
#include <iostream>
#include <fstream>
#include <algorithm>
#include <list>
#include <iterator>
#include <functional>
#include <ctime>

//Use Ctrl+M+L

//1.1.�������, ����������� ������ � �������������� �������� �������.
bool createDataByStream(const std::string& filename)
{
	srand(time(0));
	std::ofstream fout(filename + ".txt");
	if (fout.is_open())
	{
		std::cout << "���� " + filename + " ������� ������." << std::endl;
		for (int i = 0; i < 100; i++)
			fout << rand() % 101 - 50 << std::endl;
		fout.close();
		return 1;
	}
	else
	{
		std::cout << "���� " + filename + " �� ������� �������." << std::endl;
		return 0;
	}
}

//1.2.	�������, ������������ �������� generate ��� ������������� ���������������� ���������� � �������� copy ��� ������ ��� ����������� � ���� � �������������� ��������� ����������.
bool createDataByGenerate(const std::string& filename)
{
	srand((int)time(0));
	std::ofstream fout(filename + ".txt");
	if (fout.is_open())
	{
		std::cout << "���� " + filename + " ������� ������." << std::endl;
		std::list<int> list;
		list.resize(100);
		std::generate(list.begin(), list.end(), [] {return rand() % 101 - 50; });
		std::copy(list.begin(), list.end(), std::ostream_iterator<int>(fout, "\n"));
		fout.close();
		return 1;
	}
	else
	{
		std::cout << "���� " + filename + " �� ������� �������." << std::endl;
		return 0;
	}
}

//2.1.�������, ����������� ������ � �������������� �������� �������.
template<typename T>
bool loadData1(const std::string& filename, T& container)
{
	std::ifstream fin(filename + ".txt");
	if (fin.is_open())
	{
		std::cout << "���� " + filename + " ������� ������." << std::endl;
		int temp;
		while (fin >> temp)
			container.push_back(temp);
		fin.close();
		return 1;
	}
	else
	{
		std::cout << "���� " + filename + " �� ������� �������." << std::endl;
		return 0;
	}
}

//2.2.�������, ����������� ������ � �������������� ��������� ���������� � ��������� copy.
template<typename T>
bool loadData2(const std::string& filename, T& container)
{
	std::ifstream fin(filename + ".txt");
	if (fin.is_open())
	{
		std::cout << "���� " + filename + " ������� ������." << std::endl;
		std::istream_iterator<int> input(fin);
		std::istream_iterator<int> end_of_stream;
		copy(input, end_of_stream, inserter(container, container.begin()));
		fin.close();
		return 1;
	}
	else
	{
		std::cout << "���� " + filename + " �� ������� �������." << std::endl;
		return 0;
	}
}

//3.1.��� ��������� �������, ����������� ���������������� ��������� ������� ����.������ ��������� � ����� ���������� ���������� ����� � ������� �������������� ��������� ���������� �� ���������� ��������.
template<typename T>
T modify(T& container)
{
	int sum = 0, k = 0, sr;
	for (auto cur : container)
	{
		sum += cur;
		k++;
	}
	sr = int(sum / k);
	T result(container);
	for (auto cur = result.begin(); cur != result.end(); cur++)
		if (*cur == 0)
		{
			*cur = sr;
		}
	result.push_back(sum);
	result.push_back(sr);
	return result;
}

//3.2.	��� �������, ����������� ���������-���������.
template<typename T>
std::list<int> modify(T beg, T end)
{
	int sum = 0, k = 0, sr;
	for (T i = beg; i != end; i++)
	{
		sum += *i;
		k++;
	}
	sr = int(sum / k);
	std::list<int> result(beg, end);
	for (auto cur = result.begin(); cur != result.end(); cur++)
		if (*cur == 0)
		{
			*cur = sr;
		}
	result.push_back(sum);
	result.push_back(sr);
	return result;
}

//3.3.	��� �������, ������������ �������� for_each(). ���������� �������� ���������� � ���� ��������������� ������� ��� ������-�������.
template<typename T>
void modify(T& container, std::function<void(int&)> func)
{
	std::for_each(container.begin(), container.end(), func);
}

//3.4.	��� �������, ������������ �������� transform() � ��������� ��������� ��� �������������� �������� ������ �� �����, �� �������������� � ������ � ���� ����������.
void modify(const std::string& filename, const std::string& resultFilename, std::function<int(const int&)> func)
{
	std::ofstream fout(resultFilename + ".txt");
	std::ifstream fin(filename + ".txt");
	std::istream_iterator<int> input(fin);
	std::ostream_iterator<int> output(fout, "\n");
	std::istream_iterator<int> end_of_stream;
	std::transform(input, end_of_stream, output, func);
	fin.close();
	fout.close();
}

//4.1.�������, ����������� ������ � �������������� �������� �������.
bool outputResult1(const std::string& filename)
{
	std::ifstream fin(filename + ".txt");
	if (fin.is_open())
	{
		std::cout << "���� " + filename + " ������� ������." << std::endl;
		int temp;
		while (fin >> temp)
			std::cout << temp << "  ";
		std::cout << std::endl;
		fin.close();
		return 1;
	}
	else
	{
		std::cout << "���� " + filename + " �� ������� �������." << std::endl;
		return 0;
	}
}

//4.2.	�������, ������������ �������� copy ��� ������ ���������� � ���� � �������������� ��������� ����������.
template<typename T>
void outputResult2(T& container, const std::string& filename)
{
	std::ofstream fout(filename + ".txt");
	copy(container.begin(), container.end(), std::ostream_iterator<int>(fout, "\n"));
	fout.close();
}
