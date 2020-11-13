#include <iostream>
#include <list>
#include <iterator>

#include "functions_part_1.h"
#include "functions_part_2.h"


int main()
{
	//Файлы заданий хранятся в соответствующих папках в папке решения проекта.

	//			ТЕСТ: ЧАСТЬ 1

	setlocale(0, "russian");
	std::cout << "###		ТЕСТ: ЧАСТЬ 1		###\n\n";
	std::string dir1("Part_1/");
	std::string file1(dir1 + "First"), file2(dir1 + "Second");

	std::cout << "Создание первого файла (метод 1):  ";
	createDataByStream(file1);
	std::cout << "Создание второго файла (метод 2):  ";
	createDataByGenerate(file2);

	std::list<int> list1, list2;
	std::cout << "Создание первого контейнера (метод 1):  ";
	loadData1(file1, list1);
	std::cout << "Создание второго контейнера (метод 2):  ";
	loadData2(file2, list2);

	//изменяем без лишних слов (все работает)
	//первые две ф-ии = ЗАМЕНА ВСЕХ НУЛЕЙ СРЕДНИМ АРИФМЕТИЧЕСКИМ
	std::cout << "\nlist1 до преобразования:  "; for (auto i : list1) std::cout << i << "  ";
	std::cout << "\n\nlist1 после преобразования:  "; for (auto i : modify(list1)) std::cout << i << "  ";
	std::cout << "\nПоследние два числа - это сумма и среднее арифметическое чисел первоначального контейнера.";
	modify(list1.begin(), list1.end());
	modify(list1, [](int& n) { n++; });	//+1 ко всем числам
	modify(file1, dir1 + "ModifiedFirst", [](const int& n) {return n - 1; });	//в ModifiedFirst все числа на 1 меньше, чем в First
	std::cout << "\n\nВыыод первого файла в консоль в строку:  ";
	outputResult1(file1);
	std::cout << "\nВыыод измененного первого файла в консоль в строку (n-1):  ";
	outputResult1(dir1 + "ModifiedFirst");

	//перевод контейнера в файл
	outputResult2(list1, dir1 + "FirstFromContainer");

	std::cout << "\nВывод файла из чисел первого контейнера (First+1):  ";
	outputResult1(dir1 + "FirstFromContainer");



	//			ТЕСТ: ЧАСТЬ 2

	/*
	Это тест, поэтому
	при первом же запуске программы удалятся записи лекарств аптеки №1 и
	будет предложено заменить лекарств c называнием Aspirin
	Поэтому я установил точки останова в местах уделения.
	*/

	std::cout << "\n\n###		ТЕСТ: ЧАСТЬ 2		###\n\n";
	std::string dir2("Part_2/");
	std::string drugBase(dir2 + "drugDatadase"), founded_by_num, founded_by_name, founded_by_date;
	//РАСКОММЕНТИРОВАТЬ ДЛЯ САМОСТАЯТЕЛЬНОГО ЗАПОЛНЕНИЯ БАЗЫ
	createDrugDatabase(drugBase);		//База лекарств уже заполнена. (drugDatabase.txt (.bin))
	addToDrugDatabase(drugBase);		//Для быстрого заполнения базы скопировать в консоль содержимое файла
										//БЫСТРОЕ ЗАПОЛНЕНИЕ БАЗЫ.txt

	convertToTxt(drugBase);

	founded_by_num = findByNumber(drugBase, 1);
	convertToTxt(founded_by_num);
	founded_by_name = findByName(drugBase, "Aspirin");
	convertToTxt(founded_by_name);
	founded_by_date = findByDate(drugBase, "09.02.2000");
	convertToTxt(founded_by_date);

	removeDrugs(drugBase, founded_by_num); //удаление лекарств аптеки №1
	convertToTxt(drugBase);

	replaceDrugs(drugBase, founded_by_name);	//замена лекарств c называнием Aspirin
	convertToTxt(drugBase);
}
