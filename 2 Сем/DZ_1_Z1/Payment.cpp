#include "Payment.h"
#include <iostream>

Payment::Payment()
{
	std::cout << "\n��������� ������ � ���������:\n";
	std::cout << "�������: "; std::cin >> last_name;
	std::cout << "���: "; std::cin >> name;
	std::cout << "��������: "; std::cin >> patronimic;
	std::cout << "�������� ���������: "; std::cin >> salary;
	std::cout << "���� ����������� �� ������:\n";
	unsigned int num;
	std::cout << "	���: ";
	std::cin >> num;
	employment_date.set_year(num);
	std::cout << "	�����: ";
	std::cin >> num;
	employment_date.set_month(num);
	std::cout << "	����: ";
	std::cin >> num;
	employment_date.set_day(num);
	std::cout << "������� ��������: "; std::cin >> overhead;
	std::cout << "���-�� ������������ �� ����� ����: "; std::cin >> worked_per_month;
	std::cout << "���-�� ������� ���� � ������: "; std::cin >> working_days_in_month;
	income_tax = 1;
	++payment_counter;
}

Payment::~Payment()
{
	--payment_counter;
}

const int & Payment::experience_is(Date &cur_date) const
{
	return cur_date - employment_date;
}
const double & Payment::accured_is()
{
	double res = (salary / working_days_in_month) * worked_per_month * (1 + overhead / 100);
	accured = res;
	return res;
}
const double & Payment::withheld_is()
{
	double res = accured * ((double(income_tax) / 100) + ((100 - double(income_tax)) * 13 / 10000));
	withheld = res;
	return res;
}
const double & Payment::handout_is()
{
	return accured - withheld;
}

int Payment::quantity()
{
	return payment_counter;
}

int Payment::payment_counter = 0;