#include <iostream>
#include <conio.h>
#include <locale.h>
#include <string>
#include <typeinfo>

#include "List.h"
#include "ListIterator.h"
#include "Algorithm.h"
#include "Application.h"
#include "Student.h"


int main()
{
	setlocale(0, "RUSSIAN");
	int cmd;
	do {
		std::cout << "1. Список char" << std::endl;
		std::cout << "2. Список Student" << std::endl;
		std::cout << "\nВведите код команды: ";
		std::cin >> cmd;
		if (cmd == 1)
		{
			char a;
			List<char> list{ 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h' };
			Application(a, list);
		}
		else if (cmd == 2)
		{
			Student a;
			List<Student> list{ {"Andrew Pascal", "FN11-22b", 5.0}, {"Alexandr Shibanov", "FN11-22b", 2.4}, {"Denis Vasin", "FN11-23b", 4.5}, {"Alina Zakurajnaya", "FN11-21b", 3.7}, {"Nikita Demidov", "FN11-21b", 4.7}, {"Luba Kochneva", "FN11-22b", 5.0} };
			Application(a, list);
		}
		else std::cout << "Неверный код команды.\n";
	} while (cmd != 1 && cmd != 2);

	
	
}

