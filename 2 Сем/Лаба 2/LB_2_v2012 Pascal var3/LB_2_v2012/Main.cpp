
#include <iostream>
#include <string>
#include <conio.h>
#include "Vector.h"
#include "Matrix.h"
#include "exeption.h"





int main()
{setlocale(0, "russian");
	printf("==============================VECTOR=TEST==============================\n\n");
	//ВЕКТОРЫ
	int k;
	double n1;
	bool flag = true;
	Vector v1, v2;
	std::cout << "Введите вектор v1.Сначала введите его размер,а потом заполняйте значения" << std::endl;
	std::cin >> v1 ;
	std::cout << "Введите вектор v2.Сначала введите его размер,а потом заполняйте значения" << std::endl;
	std::cin >> v2;
	do
	{
		std::cout << "\t\t\t\t\t\t-----------------" << std::endl;
		std::cout << "\t\t\t\t\t\t1.v1=v2" << std::endl;
		std::cout << "\t\t\t\t\t\t2.v1= +v1" << std::endl;
		std::cout << "\t\t\t\t\t\t3.v1= -v1" << std::endl;
		std::cout << "\t\t\t\t\t\t4.v1+=v2" << std::endl;
		std::cout << "\t\t\t\t\t\t5.v1-=v2" << std::endl;
		std::cout << "\t\t\t\t\t\t6.v1= v1++" << std::endl;
		std::cout << "\t\t\t\t\t\t7.v1= ++v1" << std::endl;
		std::cout << "\t\t\t\t\t\t8.v1= v1+v2" << std::endl;
		std::cout << "\t\t\t\t\t\t9.v1= v1--" << std::endl;
		std::cout << "\t\t\t\t\t\t10.v1= --v1" << std::endl;
		std::cout << "\t\t\t\t\t\t11.v1= v1-v2" << std::endl;
		std::cout << "\t\t\t\t\t\t12.v1= v1*n" << std::endl;
		std::cout << "\t\t\t\t\t\t13.v1= n*v1" << std::endl;
		std::cout << "\t\t\t\t\t\t14.v1*v2" << std::endl;
		std::cout << "\t\t\t\t\t\t15.v1[n]" << std::endl;
		std::cout << "\t\t\t\t\t\t16.double(v1)" << std::endl;
		std::cout << "\t\t\t\t\t\t17.v2.length()" << std::endl;
		std::cout << "\t\t\t\t\t\t18.v2.size()" << std::endl;
		std::cout << "\t\t\t\t\t\t19.ВЫХОД" << std::endl;
		std::cout << "\t\t\t\t\t\t-----------------" << std::endl;
		std::cin >> k;
		switch (k)
		{
		case 1:
			std::cout << "v1 = v2\nAnswer: ";
			try {
				v1 = v2;
				std::cout << v1 << std::endl;
			}
			catch (cls_IncompatibleDimException e)
			{
				std::cerr << e.what() << std::endl;
			}
			break;
		case 2:
			std::cout << "+v1\nAnswer: ";
			+v1;
			std::cout << v1 << std::endl;
			break;
		case 3:
			std::cout << "-v1\nAnswer: ";
			-v1;
			std::cout << v1 << std::endl;
			break;
		case 4:
			std::cout << "v1 += v2\nAnswer: ";
			try {
				v1 += v2;
				std::cout << v1 << std::endl;
			}
			catch (cls_IncompatibleDimException e)
			{
				std::cerr << e.what() << std::endl;
			}
			break;
		case 5:
			std::cout << "v1 -= v2\nAnswer: ";
			try {
				v1 -= v2;
				std::cout << v1 << std::endl;
			}
			catch (cls_IncompatibleDimException e)
			{
				std::cerr << e.what() << std::endl;
			}
			break;
		case 6:
			std::cout << "v1++\nAnswer: ";
			std::cout << v1++ << std::endl;
			break;
		case 7:
			std::cout << "++v1\nAnswer: ";
			std::cout << ++v1 << std::endl;
			break;
		case 8:
			try {
				v1 = v1 + v2;
				std::cout << "v1+v2\nAnswer: ";
				std::cout << v1 << std::endl;
			}
			catch (cls_IncompatibleDimException e)
			{
				std::cerr << e.what() << std::endl;
			}
			break;
		case 9:
			std::cout << "v1--\nAnswer: ";
			std::cout << v1-- << std::endl;
			break;
		case 10:
			std::cout << "--v1\nAnswer: ";
			std::cout << --v1 << std::endl;
			break;
		case 11:
			try {
				v1 = v1 - v2;
				std::cout << "v1-v2\nAnswer: ";
				std::cout << v1 << std::endl;
			}
			catch (cls_IncompatibleDimException e)
			{
				std::cerr << e.what() << std::endl;
			}
			break;
		case 12:
			std::cout << "Введите число" << std::endl;
			std::cin >> n1;
			std::cout << "v1*n\nAnswer: ";
			v1 = v1*n1;
			std::cout << v1 << std::endl;
			break;
		case 13:
			std::cout << "Введите число" << std::endl;
			std::cin >> n1;
			std::cout << "n*v1\nAnswer: ";
			v1 = n1*v1 ;
			std::cout << v1 << std::endl;
			break;
		case 14:
			std::cout << "v1*v2\nAnswer: ";
			try
			{
				double sklr = v1 * v2;
				std::cout << sklr << std::endl;
			}
			catch (cls_IncompatibleDimException e)
			{
				std::cerr << e.what() << std::endl;
			}
			break;
		case 15:
			std::cout << "Введите число" << std::endl;
			std::cin >> n1;
			std::cout << "v1[n]\nAnswer: ";
			try { std::cout << v1[n1] << std::endl << std::endl; }
			catch (cls_OutOfRangeException e)
			{
				std::cerr << e.what() << std::endl;
			}
			break;
		case 16:
			std::cout << "double(v1)\nAnswer: ";
			std::cout << double(v1) << std::endl << std::endl;
			break;
		case 17:
			std::cout << "v2.length()\nAnswer: ";
			std::cout << v2.length() << std::endl << std::endl;
			break;
		case 18:
			std::cout << "v2.size()\nAnswer: ";
			std::cout << v2.size() << std::endl << std::endl;
			break;
		default:
			flag = false;
			break;
		}
	} while (flag);
	
	printf("==============================MATRIX=TEST==============================\n\n");
	//МАТРИЦЫ
	Matrix m1, m2; 
	flag = true;
	int n, m;
	std::cout << "Введите матрицу m1 \n" << std::endl;
	std::cin >> m1;
	std::cout << "Введите матрицу m2 \n" << std::endl;
	std::cin >> m2;
	do
	{
		std::cout << "\t\t\t\t\t\t-----------------" << std::endl;
		std::cout << "\t\t\t\t\t\t1.m1=m2" << std::endl;
		std::cout << "\t\t\t\t\t\t2.m1= +m1" << std::endl;
		std::cout << "\t\t\t\t\t\t3.m1= -m1" << std::endl;
		std::cout << "\t\t\t\t\t\t4.m1+=m2" << std::endl;
		std::cout << "\t\t\t\t\t\t5.m1-=m2" << std::endl;
		std::cout << "\t\t\t\t\t\t6.m1++" << std::endl;
		std::cout << "\t\t\t\t\t\t7.++m1" << std::endl;
		std::cout << "\t\t\t\t\t\t8.m1= m1+m2" << std::endl;
		std::cout << "\t\t\t\t\t\t9.m1--" << std::endl;
		std::cout << "\t\t\t\t\t\t10.--m1" << std::endl;
		std::cout << "\t\t\t\t\t\t11.m1= m1-m2" << std::endl;
		std::cout << "\t\t\t\t\t\t12.m1= m1*n" << std::endl;
		std::cout << "\t\t\t\t\t\t13.m1= n*m1" << std::endl;
		std::cout << "\t\t\t\t\t\t14.m1=m1*m2" << std::endl;
		std::cout << "\t\t\t\t\t\t15.m1(n,m)" << std::endl;
		std::cout << "\t\t\t\t\t\t16.m1.transp()" << std::endl;
		std::cout << "\t\t\t\t\t\t17.m1.raws()" << std::endl;
		std::cout << "\t\t\t\t\t\t18.m1.columns()" << std::endl;
		std::cout << "\t\t\t\t\t\t19.ВЫХОД" << std::endl;
		std::cout << "\t\t\t\t\t\t-----------------" << std::endl;
		std::cin >> k;
		switch (k)
		{
		case 1:
			std::cout << "m1 = m2\nAnswer: ";
			try {
				m1 = m2;
				std::cout << m1 << std::endl;
			}
			catch (cls_IncompatibleDimException e)
			{
				std::cerr << e.what() << std::endl;
			}
			break;
		case 2:
			std::cout << "m1= +m1\nAnswer: ";
			std::cout << +m1 << std::endl;
			break;
		case 3:
			std::cout << "m1= -m1\nAnswer: ";
			std::cout << -m1 << std::endl;
			break;
		case 4:
			std::cout << "m1 += m2\nAnswer: ";
			try
			{
				m1 += m2;
				std::cout << m1 << std::endl;
			}
			catch (cls_IncompatibleDimException e)
			{
				std::cerr << e.what() << std::endl;
			}
			break;
		case 5:
			std::cout << "m1 -= m2\nAnswer: ";
			try {
				m1 -= m2;
				std::cout << m1 << std::endl;
			}
			catch (cls_IncompatibleDimException e)
			{
				std::cerr << e.what() << std::endl;
			}
			break;
		case 6:
			std::cout << "m1++\nAnswer: ";
			try
			{
				std::cout << m1++ << std::endl;
			}
			catch (cls_IncompatibleDimException e)
			{
				std::cerr << e.what() << std::endl;
			}
			break;
		case 7:
			std::cout << "++m1\nAnswer: ";
			try
			{
				std::cout << ++m1 << std::endl;
			}
			catch (cls_IncompatibleDimException e)
			{
				std::cerr << e.what() << std::endl;
			}
			break;
		case 8:
			std::cout << "m1+m2\nAnswer: ";
			try
			{
				m1 = m1 + m2;
				std::cout << m1 << std::endl;
			}
			catch (cls_IncompatibleDimException e)
			{
				std::cerr << e.what() << std::endl;
			}
			break;
		case 9:
			std::cout << "m1--\nAnswer: ";
			try
			{
				std::cout << m1-- << std::endl;
			}
			catch (cls_IncompatibleDimException e)
			{
				std::cerr << e.what() << std::endl;
			}
			break;
		case 10:
			std::cout << "--m1\nAnswer: ";
			try
			{
				std::cout << --m1 << std::endl;
			}
			catch (cls_IncompatibleDimException e)
			{
				std::cerr << e.what() << std::endl;
			}
			break;
		case 11:
			std::cout << "m1= m1-m2\nAnswer: ";
			try
			{
				m1 = m1 - m2;
				std::cout << m1 << std::endl;
			}
			catch (cls_IncompatibleDimException e)
			{
				std::cerr << e.what() << std::endl;
			}
			break;
		case 12:
			std::cout << "Введите число" << std::endl;
			std::cin >> n1;
			std::cout << "m1*n\nAnswer: ";
			m1 = m1 * n1;
			std::cout << m1 << std::endl;
			break;
		case 13:
			std::cout << "Введите число" << std::endl;
			std::cin >> n1;
			std::cout << "n*m1\nAnswer: ";
			m1 = n1*m1;
			std::cout << m1 << std::endl;
			break;
		case 14:
			std::cout << "m1*m2\nAnswer: ";
			try
			{
				m1 = m1 * m2;
				std::cout << m1 << std::endl << std::endl;
			}
			catch (cls_IncompatibleDimException e)
			{
				std::cerr << e.what() << std::endl;
			}
			break;
		case 15:
			std::cout << "Введите строку n" << std::endl;
			std::cin >> n;
			std::cout << "Введите столбец m" << std::endl;
			std::cin >> m;
			std::cout << "m1(n,m)\nAnswer: ";
			try { std::cout << m1(n,m) << std::endl << std::endl; }
			catch (cls_OutOfRangeException e)
			{
				std::cerr << e.what() << std::endl;
			}
			break;
		case 16:
			std::cout << "m1.transp()\nAnswer: ";
			std::cerr << m1.transp() << std::endl;
			break;
		case 17:
			std::cout << "m1.raws()\nAnswer: ";
			std::cerr << m1.raws() << "\n" << std::endl;
			break;
		case 18:
			std::cout << "m1.columns()\nAnswer: ";
			std::cerr << m1.columns() << "\n" << std::endl;
			break;
		default:
			flag = false;
			break;
		}
	}  while (flag);

	//МАТРИЦА И ВЕКТОР
	printf("==============================MATRIX=AND=VECTOR=TEST==============================\n\n");
	std::cout << "Ваши итоговые векторы и матрицы: " << std::endl;
	std::cout << "m1: " << m1 << std::endl;
	std::cout << "m2: " << m2 << std::endl;
	std::cout << "v1: " << v1 << std::endl;
	std::cout << "v2: " << v2 << std::endl;
	flag = true;
	do
	{
		std::cout << "\t\t\t\t\t\t-----------------" << std::endl;
		std::cout << "\t\t\t\t\t\t1.m1*v1" << std::endl;
		std::cout << "\t\t\t\t\t\t2.m1*v2" << std::endl;
		std::cout << "\t\t\t\t\t\t3.m2*v1" << std::endl;
		std::cout << "\t\t\t\t\t\t4.m2*v2" << std::endl;
		std::cout << "\t\t\t\t\t\t5.ВЫХОД" << std::endl;
		std::cout << "\t\t\t\t\t\t-----------------" << std::endl;
		std::cin >> k;
		switch (k)
		{
		case 2:
			std::cout << "m1*v2\nAnswer: ";
			try
			{
				std::cout << m1 * v2 << std::endl << std::endl;
			}
			catch (cls_IncompatibleDimException e)
			{
				std::cerr << e.what() << std::endl;
			}
			break;
		case 1:
			std::cout << "m1*v1\nAnswer: ";
			try
			{
				std::cout << m1 * v1 << std::endl << std::endl;
			}
			catch (cls_IncompatibleDimException e)
			{
				std::cerr << e.what() << std::endl;
			}
			break;
		case 3:
			std::cout << "m2*v1\nAnswer: ";
			try
			{
				std::cout << m2 * v1 << std::endl << std::endl;
			}
			catch (cls_IncompatibleDimException e)
			{
				std::cerr << e.what() << std::endl;
			}
			break;
		case 4:
			std::cout << "m2*v2\nAnswer: ";
			try
			{
				std::cout << m2 * v2 << std::endl << std::endl;
			}
			catch (cls_IncompatibleDimException e)
			{
				std::cerr << e.what() << std::endl;
			}
			break;
		default:
			flag = false;
			break;
		}
	} while (flag);
	return 0;
}