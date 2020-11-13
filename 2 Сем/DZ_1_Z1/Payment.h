#pragma once
#include "Date.h"
class Payment
{
	char 
		last_name[15],			//фамили€
		name[15],				//им€
		patronimic[15];			//отчество
	double 
		salary,					//зарплата
		accured,				//начисленна€ сумма
		withheld;				//удержанна€ сумма
	Date
		employment_date;		//год поступлени€ на работу
	int 
		overhead,				//процент надбавки
		income_tax,				//подоходный налог
		worked_per_month,		//кол-во отработанных за мес€ц дней
		working_days_in_month;	//кол-во рабочих дней в мес€це
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

