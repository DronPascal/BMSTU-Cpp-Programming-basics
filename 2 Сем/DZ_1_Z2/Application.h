#pragma once
#include <iostream>
#include <string>
#include <conio.h>
#include "Application.h"
#include "Array.h"
#include "Octal.h"
#include "Hex.h"

class Application
{
public:
	static void Run()
	{
		setlocale(0, "russian");
		int key, key2;
		char num[101];
		Array *ptr = nullptr, a3;

		Octal o1;
		key = 0;
		do {
			std::cout << "Введите первое восьмеричное число o1: ";
			std::cin >> num;
			try { o1 = num; key = 1; }
			catch (std::invalid_argument e) { std::cerr << e.what() << std::endl; }
		} while (key == 0);
		Octal o2;
		key = 0;
		do {
			std::cout << "Введите второе восьмеричное число o2: ";
			std::cin >> num;
			try { o2 = num; key = 1; }
			catch (std::invalid_argument e) { std::cerr << e.what() << std::endl; }
		} while (key == 0);
		Octal o3;


		Hex h1(num);
		key = 0;
		do {
			std::cout << "Введите первое шестнадцатеричное число h1: ";
			std::cin >> num;
			try { h1 = num; key = 1; }
			catch (std::invalid_argument e) { std::cerr << e.what() << std::endl; }
		} while (key == 0);
		Hex h2;
		key = 0;
		do {
			std::cout << "Введите второе шестнадцатеричное число h2: ";
			std::cin >> num;
			try { h2 = num; key = 1; }
			catch (std::invalid_argument e) { std::cerr << e.what() << std::endl; }
		} while (key == 0);
		Hex h3;

		do {
			std::cout << "\t\t\t\t\t\t-----------------" << std::endl;
			std::cout << "\t\t\t\t\t\t1. суммирование оператором +" << std::endl;
			std::cout << "\t\t\t\t\t\t2. суммирование функцией summ" << std::endl;
			std::cout << "\t\t\t\t\t\t3. вывода цифры по ее индексу в записи числа" << std::endl;
			std::cout << "\t\t\t\t\t\t4. ВЫХОД" << std::endl;
			std::cout << "\t\t\t\t\t\t-----------------" << std::endl;
			printf("Введите код команды: ");
			std::cin >> key;
			switch (key) {
			case 1:
			{
				std::cout << "\t\t\t\t\t\t-----------------" << std::endl;
				std::cout << "\t\t\t\t\t\t1. o1 + o2" << std::endl;
				std::cout << "\t\t\t\t\t\t2. h1 + h2" << std::endl;
				std::cout << "\t\t\t\t\t\t3. o1 + h1" << std::endl;
				std::cout << "\t\t\t\t\t\t4. h1 + o1" << std::endl;
				std::cout << "\t\t\t\t\t\t5. ВЫХОД В ГЛАВНОЕ МЕНЮ" << std::endl;
				std::cout << "\t\t\t\t\t\t-----------------" << std::endl;
				do {
					printf("Введите код команды: ");
					std::cin >> key2;
					switch (key2)
					{
					case 1:
						try {
							o3 = o1 + o2;
							o1.print(); printf(" + "); o2.print(); printf(" = "); o3.print(); printf("\n");
						}
						catch (std::length_error e) {
							std::cerr << e.what() << std::endl;
						}
						break;
					case 2:
						try {
							h3 = h1 + h2;
							h1.print(); printf(" + "); h2.print(); printf(" = "); h3.print(); printf("\n");
						}
						catch (std::length_error e) {
							std::cerr << e.what() << std::endl;
						}
						break;
					case 3:
						try {
							a3 = o1 + h2;
							o1.print(); printf(" + "); h2.print(); printf(" = "); a3.print(); printf("\n");
						}
						catch (std::length_error e) {
							std::cerr << e.what() << std::endl;
						}
						break;
					case 4:
						try {
							a3 = h1 + o2;
							h1.print(); printf(" + "); o2.print(); printf(" = "); a3.print(); printf("\n");
						}
						catch (std::length_error e) {
							std::cerr << e.what() << std::endl;
						}
						break;
					case 5:
						break;
					default:
						std::cout << "Неверный код команды\n";
					}
				} while (key2 != 5);
				break;
			}
			case 2:
			{
				std::cout << "\nptr - указатель на объект класса Array.\n" << std::endl;
				std::cout << "\t\t\t\t\t\t-----------------" << std::endl;
				std::cout << "\t\t\t\t\t\t1. ptr = &a3" << std::endl;
				std::cout << "\t\t\t\t\t\t2. ptr = &o3" << std::endl;
				std::cout << "\t\t\t\t\t\t3. ptr = &h3" << std::endl;
				std::cout << "\t\t\t\t\t\t-----------------" << std::endl;
				do {
					printf("Введите код команды: ");
					std::cin >> key2;
					switch (key2) {
					case 1: ptr = &a3; break;
					case 2: ptr = &o3; break;
					case 3: ptr = &h3; break;
					case 4: break;
					default: printf("Неверная команда. Повторите ввод.\n");
					}
				} while (key2 != 1 && key2 != 2 && key2 != 3);
				std::cout << "\t\t\t\t\t\t-----------------" << std::endl;
				std::cout << "\t\t\t\t\t\t1. ptr->summ(o1, o2)" << std::endl;
				std::cout << "\t\t\t\t\t\t2. ptr->summ(h1, h2)" << std::endl;
				std::cout << "\t\t\t\t\t\t3. ВЫХОД В ГЛАВНОЕ МЕНЮ" << std::endl;
				std::cout << "\t\t\t\t\t\t-----------------" << std::endl;
				do {
					printf("Введите код команды: ");
					std::cin >> key2;
					switch (key2)
					{
					case 1:
						try {
							o3 = ptr->summ(o1, o2);
							o1.print(); printf(" + "); o2.print(); printf(" = "); o3.print(); printf("\n");
						}
						catch (std::length_error e) {
							std::cerr << e.what() << std::endl;
						}
						break;
					case 2:
						try {
							h3 = ptr->summ(h1, h2);
							h1.print(); printf(" + "); h2.print(); printf(" = "); h3.print(); printf("\n");
						}
						catch (std::length_error e) {
							std::cerr << e.what() << std::endl;
						}
						break;
					case 3:
						break;
					default:
						std::cout << "Неверный код команды\n";
					}
				} while (key2 != 3);
				break;
			}
			case 3: 
			{
				std::cout << "\t\t\t\t\t\t-----------------" << std::endl;
				std::cout << "\t\t\t\t\t\t1. число o1" << std::endl;
				std::cout << "\t\t\t\t\t\t2. число р1" << std::endl;
				std::cout << "\t\t\t\t\t\t-----------------" << std::endl;
				do {
					printf("Введите код команды: ");
					std::cin >> key2;
					int ind;
					switch (key2) {
					case 1:
						try {
							printf("Введите индекс : "); scanf("%d", &ind); printf("Цифра: %c\n", o1[ind]); break;
						}
						catch (std::out_of_range e) {
							std::cerr << e.what() << std::endl;
						}
					case 2:
						try {
							printf("Введите индекс : "); scanf("%d", &ind); printf("Цифра: %c\n", h1[ind]); break;
						}
						catch (std::out_of_range e) {
							std::cerr << e.what() << std::endl;
						}
					default: printf("Неверная команда. Повторите ввод.\n");
					}
				} while (key2 != 1 && key2 != 2);
			}break;
			default: printf("Неверная команда. Повторите ввод.\n");
			}
		} while (key != 4);
	}
};