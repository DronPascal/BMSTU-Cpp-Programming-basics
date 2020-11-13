#pragma once
#include <iostream>
#include <fstream>
#include <algorithm>
#include <list>
#include <iterator>
#include <functional>
#include <ctime>

//Use Ctrl+M+L

//1.1.Функция, выгружающая данные с использованием файловых потоков.
bool createDataByStream(const std::string& filename)
{
	srand(time(0));
	std::ofstream fout(filename + ".txt");
	if (fout.is_open())
	{
		std::cout << "Файл " + filename + " успешно открыт." << std::endl;
		for (int i = 0; i < 100; i++)
			fout << rand() % 101 - 50 << std::endl;
		fout.close();
		return 1;
	}
	else
	{
		std::cout << "Файл " + filename + " не удалось открыть." << std::endl;
		return 0;
	}
}

//1.2.	Функция, использующая алгоритм generate для инициализации вспомогательного контейнера и алгоритм copy для вывода его содержимого в файл с использованием потоковых итераторов.
bool createDataByGenerate(const std::string& filename)
{
	srand((int)time(0));
	std::ofstream fout(filename + ".txt");
	if (fout.is_open())
	{
		std::cout << "Файл " + filename + " успешно открыт." << std::endl;
		std::list<int> list;
		list.resize(100);
		std::generate(list.begin(), list.end(), [] {return rand() % 101 - 50; });
		std::copy(list.begin(), list.end(), std::ostream_iterator<int>(fout, "\n"));
		fout.close();
		return 1;
	}
	else
	{
		std::cout << "Файл " + filename + " не удалось открыть." << std::endl;
		return 0;
	}
}

//2.1.Функция, загружающая данные с использованием файловых потоков.
template<typename T>
bool loadData1(const std::string& filename, T& container)
{
	std::ifstream fin(filename + ".txt");
	if (fin.is_open())
	{
		std::cout << "Файл " + filename + " успешно открыт." << std::endl;
		int temp;
		while (fin >> temp)
			container.push_back(temp);
		fin.close();
		return 1;
	}
	else
	{
		std::cout << "Файл " + filename + " не удалось открыть." << std::endl;
		return 0;
	}
}

//2.2.Функция, загружающая данные с использованием потоковых итераторов и алгоритма copy.
template<typename T>
bool loadData2(const std::string& filename, T& container)
{
	std::ifstream fin(filename + ".txt");
	if (fin.is_open())
	{
		std::cout << "Файл " + filename + " успешно открыт." << std::endl;
		std::istream_iterator<int> input(fin);
		std::istream_iterator<int> end_of_stream;
		copy(input, end_of_stream, inserter(container, container.begin()));
		fin.close();
		return 1;
	}
	else
	{
		std::cout << "Файл " + filename + " не удалось открыть." << std::endl;
		return 0;
	}
}

//3.1.Как шаблонная функция, принимающая последовательный контейнер данного типа.Должна добавлять в конец контейнера результата сумму и среднее арифметическое элементов контейнера по абсолютной величине.
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

//3.2.	Как функция, принимающая аргументы-итераторы.
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

//3.3.	Как функция, использующая алгоритм for_each(). Конкретную операцию передавать в виде функционального объекта или лямбда-функции.
template<typename T>
void modify(T& container, std::function<void(int&)> func)
{
	std::for_each(container.begin(), container.end(), func);
}

//3.4.	Как функция, использующая алгоритм transform() и потоковые итераторы для одновременного считывая данных из файла, их преобразования и вывода в файл результата.
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

//4.1.Функция, выгружающая данные с использованием файловых потоков.
bool outputResult1(const std::string& filename)
{
	std::ifstream fin(filename + ".txt");
	if (fin.is_open())
	{
		std::cout << "Файл " + filename + " успешно открыт." << std::endl;
		int temp;
		while (fin >> temp)
			std::cout << temp << "  ";
		std::cout << std::endl;
		fin.close();
		return 1;
	}
	else
	{
		std::cout << "Файл " + filename + " не удалось открыть." << std::endl;
		return 0;
	}
}

//4.2.	Функция, использующая алгоритм copy для вывода контейнера в файл с использованием потоковых итераторов.
template<typename T>
void outputResult2(T& container, const std::string& filename)
{
	std::ofstream fout(filename + ".txt");
	copy(container.begin(), container.end(), std::ostream_iterator<int>(fout, "\n"));
	fout.close();
}
