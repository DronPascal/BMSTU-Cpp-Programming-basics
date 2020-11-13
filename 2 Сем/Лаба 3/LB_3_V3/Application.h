#pragma once
#include <iostream>
#include <conio.h>
#include <locale.h>
#include <string>
#include <typeinfo>

#include "List.h"
#include "ListIterator.h"
#include "Algorithm.h"
#include "Student.h"


auto pred = [&](Student x, static double b = 0) {if (b == 0) { std::cout << "\nВведите балл, ниже которого ищем: "; std::cin >> b; }; return (x.AvrMark <= b); };	//ПРЕДИКАТ (findif)
auto pred2 = [&](Student x, static double b = 0) {if (b == 0) { std::cout << "\nВведите балл, выше которого ищем: "; std::cin >> b; }; return (x.AvrMark <= b); };	//ПРЕДИКАТ (foreach)
auto op = [&](Student x) { x.AvrMark = 5; };	//ОПЕРАЦИЯ

template<class my_type, class List = List<my_type>, class list_iterator = typename List::iterator>
void Application(my_type &elem, List list)
{
	setlocale(0, "RUSSIAN");
	int cmd, cmd2;
	do {
		std::cout << "1. Тест методов" << std::endl;
		std::cout << "2. Тест алгоритмов" << std::endl;
		std::cout << "0. Выход" << std::endl;
		std::cout << "\nВведите код команды: ";
		std::cin >> cmd;
		if (cmd == 1)
		{
			std::cout << "1. PushBack(elem) - добавляет копию аргумента elem в конец списка" << std::endl;
			std::cout << "2. PopBack() - удаляет последний элемент" << std::endl;
			std::cout << "3. PushFront(elem) - добавляет копию аргумента elem в начало списка" << std::endl;
			std::cout << "4. PopFront() - удаляет первый элемент" << std::endl;
			std::cout << "5. Insert(pos, elem) - вставляет копию элемента elem перед итератором pos" << std::endl;
			std::cout << "6. Insert(pos, beg, end) - вставляет копии всех элементов интервала [beg, end) перед итератором pos" << std::endl;
			std::cout << "7. Erase(pos) - удаляет элемент в позиции итератора pos" << std::endl;
			std::cout << "8. Resize(num) - изменяет кол-во элементов до num" << std::endl;
			std::cout << "9. Empty() - возвращает результат проверки того, что контейнер пуст" << std::endl;
			std::cout << "10. Size() - возвращает размер списка" << std::endl;
			std::cout << "11. Clear() - удаляет все элементы списка" << std::endl;
			std::cout << "12. -> - изменить элемент списка" << std::endl;
			std::cout << "13. Вывод списка" << std::endl;
			std::cout << "0. Выход" << std::endl;
			do {
				std::cout << "\nВведите код команды: ";
				std::cin >> cmd2;
				switch (cmd2)
				{
				case 1:
				{
					std::cout << "Введите элемент: ";
					my_type elem;
					std::cin >> elem;
					list.PushBack(elem);
					break;
				}
				case 2:
				{
					list.PopBack();
					std::cout << "Последний элемент удален.\n";
					break;
				}
				case 3:
				{
					std::cout << "Введите элемент: ";
					my_type elem;
					std::cin >> elem;
					list.PushFront(elem);
					break;
				}
				case 4:
				{
					list.PopFront();
					std::cout << "Первый элемент удален.\n";
					break;
				}
				case 5:
				{
					std::cout << "Введите элемент: ";
					my_type elem;
					std::cin >> elem;
					std::cout << "Введите позицию элемента перед которым проводится вставка: ";
					int Ind;
					std::cin >> Ind;
					list_iterator pos(&list, Ind);
					list.Insert(pos, elem);
					break;
				}
				case 6:
				{
					try {
						int ind, beg_ind, end_ind;
						std::cout << "Введите индекс элемента перед которым проводится вставка: ";
						std::cin >> ind;
						list_iterator Iter(&list, ind);
						std::cout << "Введите индекс элемента - начала интервала: ";
						std::cin >> beg_ind;
						list_iterator beg(&list, beg_ind);
						std::cout << "Введите индекс элемента - конца интервала: ";
						std::cin >> end_ind;
						list_iterator end(&list, end_ind);
						Iter = list.Insert(Iter, beg, end);
					}
					catch (std::out_of_range e) { std::cerr << e.what() << std::endl; }
					break;
				}
				case 7:
				{
					std::cout << "Введите позицию элемента, который следует удалить: ";
					int Ind;
					std::cin >> Ind;
					list_iterator Iter(&list, Ind);
					try {
						list.Erase(Iter);
					}
					catch (std::out_of_range e) { std::cerr << e.what() << std::endl; }
					break;
				}
				case 8:
				{
					std::cout << "Введите новое кол-во элементов: ";
					int num;
					std::cin >> num;
					list.Resize(num);
					break;
				}
				case 9:
				{
					if (list.Empty()) std::cout << "Список пуст.\n";
					else
						std::cout << "Список не пуст.\n";
					break;
				}
				case 10:
				{
					std::cout << "Текущий размер дека - " << list.Size() << std::endl;
					break;
				}
				case 11:
				{
					list.Clear();
					std::cout << "Все элементы дека удалены.\n";
					break;
				}
				case 12:
				{
					if (typeid(my_type) == typeid(Student))
					{
						std::cout << "Введите позицию элемента, который следует изменить: ";
						int Ind;
						std::cin >> Ind;
						list_iterator Iter(&list, Ind);
						std::cout << "Введите новый номер группы: \n";
						std::cin >> (Iter.operator->()).Group;
					}
					break;
				}
				case 13:
				{
					if (list.Size())
					{
						std::cout << "Список: ";
						for (auto Iter = list.Begin(); Iter != list.End(); ++Iter)
							std::cout << *Iter << " ";
					}
					else std::cout << "Список пуст.";
					std::cout << "\n";
					break;
				}
				default:
					break;
				}
			} while (cmd2 != 0);
		}
		else if (cmd == 2)
		{
			std::cout << "1. findIf(beg, end, pred) - поиск по диапазону [beg, end) произвольного контейнера при заданном условии" << std::endl;
			std::cout << "2. minElement(beg, end) - определение минимального элемента в диапазоне [beg, end) произвольного контейнера" << std::endl;
			std::cout << "3. maxElement(beg, end) - определение максимального элемента в произвольном контейнере" << std::endl;
			std::cout << "4. forEach(beg, end, op) - выполнение произвольной операции над элементами диапазона [beg, end) произвольного контейнера" << std::endl;
			std::cout << "5. Sort(beg, end) заданного диапазона [beg, end) произвольного контейнера" << std::endl;
			std::cout << "6. copyIf(sourceBeg, sourceEnd, destBeg, pred) для копирования элементов диапазона [sourceBeg, sourceEnd) произвольного контейнера-источника в контейнер-приемник, начиная с позиции итератора destBeg, удовлетворяющих предикату pred" << std::endl;
			std::cout << "7. Вывод списка" << std::endl;
			std::cout << "0. Выход" << std::endl;
			do {
				std::cout << "\nВведите код команды: ";
				std::cin >> cmd2;
				switch (cmd2)
				{
				case 1:
				{
					try {
						int beg_ind, end_ind;
						std::cout << "Введите условие: ";
						std::cout << "Введите индекс элемента - начала интервала: ";
						std::cin >> beg_ind;
						list_iterator beg(&list, beg_ind);
						std::cout << "Введите индекс элемента - конца интервала: ";
						std::cin >> end_ind;
						list_iterator end(&list, end_ind);
						list_iterator Iter;
						Iter = FindIf(beg, end, pred);
						std::cout << "Найденный элемент: " << *Iter << "\n";
					}
					catch (std::out_of_range e) { std::cerr << e.what() << std::endl; }
					break;
				}
				case 2:
				{
					try {
						int beg_ind, end_ind;
						std::cout << "Введите индекс элемента - начала интервала: ";
						std::cin >> beg_ind;
						list_iterator beg(&list, beg_ind);
						std::cout << "Введите индекс элемента - конца интервала: ";
						std::cin >> end_ind;
						list_iterator end(&list, end_ind);
						list_iterator Iter;
						Iter = MinElement(beg, end);
						std::cout << "Найденный минимальный элемент: " << *Iter << "\n";
					}
					catch (std::out_of_range e) { std::cerr << e.what() << std::endl; }
					break;
				}
				case 3:
				{
					try {
						int beg_ind, end_ind;
						std::cout << "Введите индекс элемента - начала интервала: ";
						std::cin >> beg_ind;
						list_iterator beg(&list, beg_ind);
						std::cout << "Введите индекс элемента - конца интервала: ";
						std::cin >> end_ind;
						list_iterator end(&list, end_ind);
						list_iterator Iter;
						Iter = MaxElement(beg, end);
						std::cout << "Найденный максимальный элемент: " << *Iter << "\n";
					}
					catch (std::out_of_range e) { std::cerr << e.what() << std::endl; }
					break;
				}
				case 4:
				{
					try {
						int beg_ind, end_ind;
						std::cout << "Введите операцию: ";
						std::cout << "Введите индекс элемента - начала интервала: ";
						std::cin >> beg_ind;
						list_iterator beg(&list, beg_ind);
						std::cout << "Введите индекс элемента - конца интервала: ";
						std::cin >> end_ind;
						list_iterator end(&list, end_ind);
						list_iterator Iter;
						ForEach(beg, end, op);
					}
					catch (std::out_of_range e) { std::cerr << e.what() << std::endl; }
					break;
				}
				case 5:
				{
					try {
						int beg_ind, end_ind;
						std::cout << "Введите индекс элемента - начала интервала: ";
						std::cin >> beg_ind;
						list_iterator beg(&list, beg_ind);
						std::cout << "Введите индекс элемента - конца интервала: ";
						std::cin >> end_ind;
						list_iterator end(&list, end_ind);
						list_iterator Iter;
						Sort(beg, end);
					}
					catch (std::out_of_range e) { std::cerr << e.what() << std::endl; }
					break;
				}
				case 6:
				{
					try {
						int ind, beg_ind, end_ind;
						std::cout << "Введите индекс элемента - начала интервала из первого списка: ";
						std::cin >> beg_ind;
						list_iterator beg(&list, beg_ind);
						std::cout << "Введите индекс элемента - конца интервала из первого списка: ";
						std::cin >> end_ind;
						list_iterator end(&list, end_ind);

						std::cout << "Заполните второй список.\n" << std::endl;
						List list2;
						int beg_ind2, end_ind2;
						list_iterator Iter2 = list2.Begin();
						std::cout << "Введите индекс элемента - начала интервала из первого списка: ";
						std::cin >> beg_ind2;
						list_iterator beg2(&list2, beg_ind2);
						std::cout << "Введите индекс элемента - конца интервала из первого списка: ";
						std::cin >> end_ind2;
						list_iterator end2(&list2, end_ind2);

						Iter2 = list2.Insert(Iter2, beg2, end2);

						if (list2.Size())
						{
							std::cout << "Список 2: ";
							for (auto Iter2 = list2.Begin(); Iter2 != list2.End(); ++Iter2)
								std::cout << *Iter2 << " ";
						}
						else std::cout << "Список 2 пуст.";
						std::cout << "\n";

						std::cout << "Введите индекс элемента второго списка, после которого проводится вставка: ";
						std::cin >> ind;
						list_iterator pos(&list2, end_ind);

						CopyIf(beg, end, pos, pred2);
					}
					catch (std::out_of_range e) { std::cerr << e.what() << std::endl; }
					break;
				}
				case 7:
				{
					if (list.Size())
					{
						std::cout << "Список: ";
						for (auto Iter = list.Begin(); Iter != list.End(); ++Iter)
							std::cout << *Iter << " ";
					}
					else std::cout << "Список пуст.";
					std::cout << "\n";
					break;
				}
				default:
					break;
				}
			} while (cmd2 != 0);
		}
		else if (cmd) std::cout << "Неверный код команды.\n";
	} while (cmd != 0);
}