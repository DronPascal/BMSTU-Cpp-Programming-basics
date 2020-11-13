#pragma once
#include <iostream>
class Pair
{
public:
	Pair() {};
	virtual void scanm() = 0;
	virtual void printm() = 0;
};

class Money : public Pair
{
	long int rubles;
	unsigned char penny;
public:
	Money() : 
		rubles(0),
		penny(0) {};

	void operator=(const Money &cur) 
	{
		rubles = cur.rubles;
		penny = cur.penny;
	}
	const Money & operator+(const Money &cur) 
	{
		static Money res1;
		int pen = penny + cur.penny;
		res1.rubles = rubles + cur.rubles + (pen / 100);
		res1.penny = pen % 100;
		return res1;
	}

	void scanm() override
	{
		printf("Введите кол-во рублей и копеек :\n");
		printf("Рублей: ");
		scanf("%d", &rubles);
		printf("Копеек: ");
		scanf("%d", &penny);
	}
	void printm() override
	{
		int rub = rubles + (penny / 100);
		printf("%d,%d\n", rub, penny % 100);
	}
};

class Money_ : public Pair
{
	int m0_01, m0_05, m0_1, m0_5;
	int m1, m2, m5, m10, m50, m100, m500, m1000, m5000;
public:
	Money_() : 
		m0_01(0), m0_05(0), m0_1(0), m0_5(0),
		m1(0), m2(0), m5(0), m10(0), m50(0), m100(0), m500(0), m1000(0), m5000(0) {};
	
	void operator=(const Money_ &cur)
	{
		m0_01 = cur.m0_01;
		m0_05 = cur.m0_05;
		m0_1 = cur.m0_1;
		m0_5 = cur.m0_5;
		m1 = cur.m1;
		m2 = cur.m2;
		m5 = cur.m5;
		m10 = cur.m10;
		m50 = cur.m50;
		m100 = cur.m100;
		m500 = cur.m500;
		m1000 = cur.m1000;
		m5000 = cur.m5000;

	}

	const Money_ & operator+(const Money_ &cur)
	{
		static Money_ res2;
		res2.m0_01 = m0_01 + cur.m0_01;
		res2.m0_05 = m0_05 + cur.m0_05;
		res2.m0_1 = m0_1 + cur.m0_1;
		res2.m0_5 = m0_5 + cur.m0_5;
		res2.m1 = m1 + cur.m1;
		res2.m2 = m2 + cur.m2;
		res2.m5 = m5 + cur.m5;
		res2.m10 = m10 + cur.m10;
		res2.m50 = m50 + cur.m50;
		res2.m100 = m100 + cur.m100;
		res2.m500 = m500 + cur.m500;
		res2.m1000 = m1000 + cur.m1000;
		res2.m5000 = m5000 + cur.m5000;
		return res2;
	}

	void scanm() override
	{
		printf("Введите кол-во купюр и копеек каждого номинала:\n");
		printf("1 копейка: ");
		scanf("%d", &m0_01);
		printf("5 копеек: ");
		scanf("%d", &m0_05);
		printf("10 копеек: ");
		scanf("%d", &m0_1);
		printf("50 копеек: ");
		scanf("%d", &m0_5);

		printf("1 рубль: ");
		scanf("%d", &m1);
		printf("2 рубля: ");
		scanf("%d", &m2);
		printf("5 рублей: ");
		scanf("%d", &m5);
		printf("10 рублей: ");
		scanf("%d", &m10);
		printf("50 рублей: ");
		scanf("%d", &m50);
		printf("100 рублей: ");
		scanf("%d", &m100);
		printf("500 рублей: ");
		scanf("%d", &m500);
		printf("1000 рублей: ");
		scanf("%d", &m1000);
		printf("5000 рублей: ");
		scanf("%d", &m5000);
	}

	void printm() 
	{
		int pen = m0_01 + m0_05 * 5 + m0_1 * 10 + m0_5 * 50;
		int rub = m1 + m2 * 2 + m5 * 5 + m10 * 10 + m50 * 50 + m100 * 100 + m500 * 500 + m1000 * 1000 + m5000 * 5000 + (pen / 100);
		printf("%d,%d\n", rub, pen % 100);
	}
};