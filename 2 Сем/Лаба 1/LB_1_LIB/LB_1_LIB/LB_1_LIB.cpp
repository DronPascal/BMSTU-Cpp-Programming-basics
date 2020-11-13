// LB_1_LIB.cpp: ���������� ����� ����� ��� ����������� ����������.
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
	//����������
	printf("������� ���-�� �����: ");
	scanf("%4d", &n);
	arr = new int[n];
	puts("\n������: ");
	for (i = 0; i < n; i++)
	{
		arr[i] = rand() % 100;
		printf("%4d", arr[i]);
	}

	printf("\n\n����������...\n");

	MergeSort(arr, n);	//�� �����������

	//MergeSortDll(arr,n);	//������� ����������

	/*	//����� ����������
	typedef void(*pFunction)(int*, int);	
	HINSTANCE hDLL = LoadLibrary(L"..\\..\\\\MergeSortDLib\\Debug\\MergeSortDLib.dll");	//����� ���� ������������ ���. ����������
	pFunction MergeSortDll2 =(pFunction)GetProcAddress(hDLL,"MergeSortDll");
	(*MergeSortDll2)(arr, n);
	FreeLibrary(hDLL);
	//*/
	
	printf("\n������ ������������ ������� �������:\n");

	for (i = 0; i < n; i++)
	{
		printf("%4d", arr[i]);
	}
	printf("\n������� ������� ��� ����������");
	delete [] arr;

	getch();
}