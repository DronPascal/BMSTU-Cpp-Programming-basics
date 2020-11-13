#pragma once
#include <iostream>
#include <conio.h>
#include <locale.h>
#include <string>
#include "List.h"
#include "ListIterator.h"
#include "Algorithm.h"
#include "Student.h"
template<class my_type, class List = List<my_type>, class list_iterator = typename List::iterator>
void Application(List list)
{
	setlocale(0, "RUSSIAN");
	int cmd, cmd2;
	do {
		std::cout << "1. ���� �������" << std::endl;
		std::cout << "2. ���� ����������" << std::endl;
		std::cout << "0. �����" << std::endl;
		std::cout << "\n������� ��� �������: ";
		std::cin >> cmd;
		if (cmd == 1)
		{
			std::cout << "1. PushBack(elem) - ��������� ����� ��������� elem � ����� ������" << std::endl;
			std::cout << "2. PopBack() - ������� ��������� �������" << std::endl;
			std::cout << "3. PushFront(elem) - ��������� ����� ��������� elem � ������ ������" << std::endl;
			std::cout << "4. PopFront() - ������� ������ �������" << std::endl;
			std::cout << "5. Insert(pos, elem) - ��������� ����� �������� elem ����� ���������� pos" << std::endl;
			std::cout << "6. Insert(pos, beg, end) - ��������� ����� ���� ��������� ��������� [beg, end) ����� ���������� pos" << std::endl;
			std::cout << "7. Erase(pos) - ������� ������� � ������� ��������� pos" << std::endl;
			std::cout << "8. Resize(num) - �������� ���-�� ��������� �� num" << std::endl;
			std::cout << "9. Empty() - ���������� ��������� �������� ����, ��� ��������� ����" << std::endl;
			std::cout << "10. Size() - ���������� ������ ������" << std::endl;
			std::cout << "11. Clear() - ������� ��� �������� ������" << std::endl;
			std::cout << "12. -> - �������� ������� ������" << std::endl;
			std::cout << "13. ����� ������" << std::endl;
			std::cout << "0. �����" << std::endl;
			do {
				std::cout << "\n������� ��� �������: ";
				std::cin >> cmd2;
				switch (cmd2)
				{
				case 1:
				{
					std::cout << "������� �������: ";
					my_type elem;
					std::cin >> elem;
					list.PushBack(elem);
					break;
				}
				case 2:
				{
					list.PopBack();
					std::cout << "��������� ������� ������.\n";
					break;
				}
				case 3:
				{
					std::cout << "������� �������: ";
					my_type elem;
					std::cin >> elem;
					list.PushFront(elem);
					break;
				}
				case 4:
				{
					list.PopFront();
					std::cout << "������ ������� ������.\n";
					break;
				}
				case 5:
				{
					std::cout << "������� �������: ";
					my_type elem;
					std::cin >> elem;
					std::cout << "������� ������� �������� ����� ������� ���������� �������: ";
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
						std::cout << "������� ������ �������� ����� ������� ���������� �������: ";
						std::cin >> ind;
						list_iterator Iter(&list, ind);
						std::cout << "������� ������ �������� - ������ ���������: ";
						std::cin >> beg_ind;
						list_iterator beg(&list, beg_ind);
						std::cout << "������� ������ �������� - ����� ���������: ";
						std::cin >> end_ind;
						list_iterator end(&list, end_ind);
						Iter = list.Insert(Iter, beg, end);
					}
					catch (std::out_of_range e) { std::cerr << e.what() << std::endl; }
					break;
				}
				case 7:
				{
					std::cout << "������� ������� ��������, ������� ������� �������: ";
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
					std::cout << "������� ����� ���-�� ���������: ";
					int num;
					std::cin >> num;
					list.Resize(num);
					break;
				}
				case 9:
				{
					if (list.Empty()) std::cout << "������ ����.\n";
					else
						std::cout << "������ �� ����.\n";
					break;
				}
				case 10:
				{
					std::cout << "������� ������ ���� - " << list.Size() << std::endl;
					break;
				}
				case 11:
				{
					list.Clear();
					std::cout << "��� �������� ���� �������.\n";
					break;
				}
				case 12:
				{
					std::cout << "������� ������� ��������, ������� ������� ��������: ";
					int Ind;
					std::cin >> Ind;
					list_iterator Iter(&list, Ind);
					Iter->Group = "ygy";
					//ChangeInfo(Iter.operator->());
					break;
				}
				case 13:
				{
					if (list.Size())
					{
						 std::cout << "������: ";
						for (auto Iter = list.Begin(); Iter != list.End(); ++Iter)
							std::cout << *Iter << " ";
					}
					else std::cout << "������ ����.";
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
			std::cout << "1. findIf(beg, end, pred) - ����� �� ��������� [beg, end) ������������� ���������� ��� �������� �������" << std::endl;
			std::cout << "2. minElement(beg, end) - ����������� ������������ �������� � ��������� [beg, end) ������������� ����������" << std::endl;
			std::cout << "3. maxElement(beg, end) - ����������� ������������� �������� � ������������ ����������" << std::endl;
			std::cout << "4. forEach(beg, end, op) - ���������� ������������ �������� ��� ���������� ��������� [beg, end) ������������� ����������" << std::endl;
			std::cout << "5. Sort(beg, end) ��������� ��������� [beg, end) ������������� ����������" << std::endl;
			std::cout << "6. copyIf(sourceBeg, sourceEnd, destBeg, pred) ��� ����������� ��������� ��������� [sourceBeg, sourceEnd) ������������� ����������-��������� � ���������-��������, ������� � ������� ��������� destBeg, ��������������� ��������� pred" << std::endl;
			std::cout << "7. ����� ������� ������" << std::endl;
			std::cout << "0. �����" << std::endl;
			do {
				auto pred = [](my_type x)	//�������� (findif)
				{
					//if (typeid(my_type) == typeid(Student))
					{
						double b;
						std::cout << "\n������� ����, ���� �������� ����: ";
						std::cin >> b;
						return (x.AvrMark < b);
					}
					/*else if (typeid(my_type) == typeid(char))
					{
						char c;
						std::cout << "\n������� ������, ������ �������� ����: ";
						std::cin >> c;
						return ((x) < c);
					}*/
				};	

				auto op = [](my_type &x)	//��������
				{
					//if (typeid(my_type) == typeid(Student))
						x.AvrMark = 5;
					//else if (typeid(my_type) == typeid(char))
					//	x = 'X';
				 };	

				auto pred2 = [](my_type x) //�������� (foreach)
				{
					//if (typeid(my_type) == typeid(Student))
					{
						double b;
						std::cout << "\n������� ����, ���� �������� ����: ";
						std::cin >> b;
						return (x.AvrMark > b);
					}
					/*else if (typeid(my_type) == typeid(char))
					{
						char c;
						std::cout << "\n������� ������, ������ �������� ����: ";
						std::cin >> c;
						return ((+x) > c);
					}*/
				};


				std::cout << "\n������� ��� �������: ";
				std::cin >> cmd2;
				switch (cmd2)
				{
				case 1:
				{
					try {
						int beg_ind, end_ind;
						std::cout << "������� ������ �������� - ������ ���������: ";
						std::cin >> beg_ind;
						list_iterator beg(&list, beg_ind);
						std::cout << "������� ������ �������� - ����� ���������: ";
						std::cin >> end_ind;
						list_iterator end(&list, end_ind);
						list_iterator Iter;
						Iter = FindIf(beg, end, pred);
						
						std::cout << "��������� �������: " << *Iter << "\n";
					}
					catch (std::out_of_range e) { std::cerr << e.what() << std::endl; }
					break;
				}
				case 2:
				{
					try {
						int beg_ind, end_ind;
						std::cout << "������� ������ �������� - ������ ���������: ";
						std::cin >> beg_ind;
						list_iterator beg(&list, beg_ind);
						std::cout << "������� ������ �������� - ����� ���������: ";
						std::cin >> end_ind;
						list_iterator end(&list, end_ind);
						list_iterator Iter;
						Iter = MinElement(beg, end);
						std::cout << "��������� ����������� �������: " << *Iter << "\n";
					}
					catch (std::out_of_range e) { std::cerr << e.what() << std::endl; }
					break;
				}
				case 3:
				{
					try {
						int beg_ind, end_ind;
						std::cout << "������� ������ �������� - ������ ���������: ";
						std::cin >> beg_ind;
						list_iterator beg(&list, beg_ind);
						std::cout << "������� ������ �������� - ����� ���������: ";
						std::cin >> end_ind;
						list_iterator end(&list, end_ind);
						list_iterator Iter;
						Iter = MaxElement(beg, end);
						std::cout << "��������� ������������ �������: " << *Iter << "\n";
					}
					catch (std::out_of_range e) { std::cerr << e.what() << std::endl; }
					break;
				}
				case 4:
				{
					try {
						int beg_ind, end_ind;
						std::cout << "������� ������ �������� - ������ ���������: ";
						std::cin >> beg_ind;
						list_iterator beg(&list, beg_ind);
						std::cout << "������� ������ �������� - ����� ���������: ";
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
						std::cout << "������� ������ �������� - ������ ���������: ";
						std::cin >> beg_ind;
						list_iterator beg(&list, beg_ind);
						std::cout << "������� ������ �������� - ����� ���������: ";
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
						int beg_ind, end_ind;
						std::cout << "������� ������ �������� - ������ ��������� �� ������� ������: ";
						std::cin >> beg_ind;
						list_iterator beg(&list, beg_ind);
						std::cout << "������� ������ �������� - ����� ��������� �� ������� ������: ";
						std::cin >> end_ind;
						list_iterator end(&list, end_ind);

						std::cout << "��������� ������ ������.\n" << std::endl;
						List list2;
						int beg_ind2, end_ind2;
						list_iterator Iter2;
						std::cout << "������� ������ �������� - ������ ��������� �� ������� ������: ";
						std::cin >> beg_ind2;
						list_iterator beg2(&list, beg_ind2);
						std::cout << "������� ������ �������� - ����� ��������� �� ������� ������: ";
						std::cin >> end_ind2;
						list_iterator end2(&list, end_ind2);

						Iter2 = list2.Insert(Iter2, beg2, end2);

						if (list2.Size())
						{
							std::cout << "������ 2: ";
							for (auto Iter2 = list2.Begin(); Iter2 != list2.End(); ++Iter2)
								std::cout << *Iter2 << " ";
						}
						else std::cout << "������ 2 ����.";
						std::cout << "\n";

						std::cout << "������� ������ �������� ������� ������, ����� �������� ���������� �������: ";
						std::cin >> end_ind;
						list_iterator pos(&list2, end_ind);
						
						CopyIf(beg, end, pos, pred2);

						if (list2.Size())
						{
							std::cout << "������ 2 (���������������): ";
							for (auto Iter2 = list2.Begin(); Iter2 != list2.End(); ++Iter2)
								std::cout << *Iter2 << " ";
						}
					}
					catch (std::out_of_range e) { std::cerr << e.what() << std::endl; }
					break;
				}
				case 7:
				{
					if (list.Size())
					{
						std::cout << "������: ";
						for (auto Iter = list.Begin(); Iter != list.End(); ++Iter)
							std::cout << *Iter << " ";
					}
					else std::cout << "������ ����.";
					std::cout << "\n";
					break;
				}
				default:
					break;
				}
			} while (cmd2 != 0);
		}
		else if (cmd) std::cout << "�������� ��� �������.\n";
	} while (cmd != 0);
}