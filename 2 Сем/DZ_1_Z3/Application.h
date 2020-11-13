#pragma once
#include <iostream>
#include <string>
#include <conio.h>
#include "Application.h"
#include "Pair.h"

class Application
{
public:
	static void Run()
	{
		setlocale(0, "russian");
		int key, key2;
		Money m1, m2, m3;
		Money_ m_1, m_2, m_3;
		Pair *ptr = nullptr;

		printf("������� m1 (������ ����� ������ Money):\n");
		m1.scanm();
		printf("������� m2 (������ ����� ������ Money:\n");
		m2.scanm();

		printf("������� m_1 (������ ����� ������ Money_):\n");
		m_1.scanm();
		printf("������� m_2 (������ ����� ������ Money_):\n");
		m_2.scanm();

		std::cout << "\nptr - ��������� ������ Pair. Money, Money_ - �������� ������ Pair. m3, m_3 - ������� Money � Money_\n" << std::endl;
		do {
			std::cout << "\t\t\t\t\t\t-----------------" << std::endl;
			std::cout << "\t\t\t\t\t\t1. ptr = &m3" << std::endl;
			std::cout << "\t\t\t\t\t\t2. ptr = &m_3" << std::endl;
			std::cout << "\t\t\t\t\t\t3. �����" << std::endl;
			std::cout << "\t\t\t\t\t\t-----------------" << std::endl;
			printf("������� ��� �������: ");
			std::cin >> key;
			switch (key) {
			case 1: ptr = &m3; break;
			case 2: ptr = &m_3; break;
			case 3: break;
			default: printf("�������� �������. ��������� ����.\n");
			}
			if (key != 3)
			{
				std::cout << "1. m3 = m1 + m2" << std::endl;
				std::cout << "2. m_3 = m_1 + m_2" << std::endl;
				m3 = m1 + m2;
				m_3 = m_1 + m_2;
				printf("ptr->printm() = "); ptr->printm();
			}
		} while (key != 3);
	}
};