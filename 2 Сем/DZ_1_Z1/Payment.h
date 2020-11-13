#pragma once
#include "Date.h"
class Payment
{
	char 
		last_name[15],			//�������
		name[15],				//���
		patronimic[15];			//��������
	double 
		salary,					//��������
		accured,				//����������� �����
		withheld;				//���������� �����
	Date
		employment_date;		//��� ����������� �� ������
	int 
		overhead,				//������� ��������
		income_tax,				//���������� �����
		worked_per_month,		//���-�� ������������ �� ����� ����
		working_days_in_month;	//���-�� ������� ���� � ������
public:
	static int payment_counter;
	Payment ();
	~Payment();
	const int & experience_is(Date &cur_date) const;
	const double & accured_is();
	const double & withheld_is();
	const double  & handout_is();

	static int quantity();
};

