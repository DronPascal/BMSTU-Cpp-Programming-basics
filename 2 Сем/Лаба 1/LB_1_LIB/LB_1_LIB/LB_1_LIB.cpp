// LB_1_LIB.cpp: определяет точку входа для консольного приложения.
//
#include "stdafx.h"
#include <locale.h>
#include <stdio.h>
#include <conio.h>
#include <ctime>
#include "stdlib.h"
#include "Windows.h"
#include "HeaderSLib.h"
#include "HeaderDLib.h"

void main()
{
	int *arr, n, i;
	srand(time(NULL));
	setlocale(0, "russian");
	//заполнение
	printf("Введите кол-во чисел: ");
	scanf("%4d", &n);
	arr = new int[n];
	puts("\nМассив: ");
	for (i = 0; i < n; i++)
	{
		arr[i] = rand() % 100;
		printf("%4d", arr[i]);
	}

	printf("\n\nСортировка...\n");

	MergeSort(arr, n);	//из статической

	//MergeSortDll(arr,n);	//неявное связывание

	/*	//явное связывание
	typedef void(*pFunction)(int*, int);	
	HINSTANCE hDLL = LoadLibrary(L"..\\..\\\\MergeSortDLib\\Debug\\MergeSortDLib.dll");	//вызов явно подключенной дин. библиотеки
	pFunction MergeSortDll2 =(pFunction)GetProcAddress(hDLL,"MergeSortDll");
	(*MergeSortDll2)(arr, n);
	FreeLibrary(hDLL);
	//*/
	
	printf("\nМассив отсортирован методом слияния:\n");

	for (i = 0; i < n; i++)
	{
		printf("%4d", arr[i]);
	}
	printf("\nНажмите клавишу для завершения");
	delete [] arr;

	getch();
}