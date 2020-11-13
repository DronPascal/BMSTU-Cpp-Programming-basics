#include "Date.h"
#include <string>

static int days_in_month[] = {0, 31, 28/*+1 в високосном*/, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

Date::Date()	//конструкторы
{
	dat.year = 0;
	dat.month = 0;
	dat.day = 0;
	++date_counter;
	leap_renew(*this);
}

Date::Date(const unsigned int &year, const unsigned int &month, const unsigned int &day)
{
	dat.year = year;
	dat.month = month;
	dat.day = day;
	++date_counter;
	leap_renew(*this);
}

Date::Date(const char *date)
{
	char conv[10];
	int i = 0, k=0;
	while (date[i] != '.')
	{
		conv[k] = date[i];
		i++;
		k++;
	}
	conv[k] = '\0';
	dat.year = atoi(conv);
	k = 0; i++;
	while (date[i] != '.')
	{
		conv[k] = date[i];
		i++;
		k++;
	}
	conv[k] = '\0';
	dat.month = atoi(conv);
	k = 0; i++;
	while (date[i] != '\0')
	{
		conv[k] = date[i];
		i++;
		k++;
	}
	conv[k] = '\0';
	dat.day = atoi(conv);
	++date_counter;
	leap_renew(*this);
}

Date::Date(const Date &cur)
{
	dat.year = cur.dat.year;
	dat.month = cur.dat.month;
	dat.day = cur.dat.day;
	++date_counter;
	leap_renew(*this);
}

Date::~Date()
{
	--date_counter;
}

void Date::operator=(const Date *cur)	//date1 = date2
{
	dat.year = cur->dat.year;
	dat.month = cur->dat.month;
	dat.day = cur->dat.day;
	leap_renew(*this);
}

const Date & Date::operator+(const unsigned int &num) 	//date1 + days
{
	Date res(*this);
	for (int i = 0; i < num; i++) next_day(&res);
	return res;
}

const Date & Date::operator-(const unsigned int &num)	//date1 - days
{
	Date res(*this);
	for (int i = 0; i < num; i++) prev_day(&res);
	return res;
}


const bool & Date::operator>(const Date &cur) 	//date1 > date2
{
	
	return dat.year > cur.dat.year ? 1 : 
			dat.year < cur.dat.year ? 0 : 
				dat.month > cur.dat.month ? 1 : 
					dat.month < cur.dat.month ? 0 : 
						dat.day > cur.dat.day ? 1 : 
							dat.day < cur.dat.day ? 0 : 0;
}
const bool & Date::operator<(const Date &cur)	//date1 < date2
{
	return !(*this > cur);
}
const bool & Date::operator==(const Date &cur)	//date1 == date2
{
	return	((dat.year == cur.dat.year) && 
			(dat.month == cur.dat.month) && 
			(dat.day == cur.dat.day)) ? 1 : 0;
}

const unsigned int & Date::operator-(const Date &cur)	//date1 - date2
{
	if (*this == cur) return 0;
	unsigned int day_counter = 0;
	Date res, sec;
	if (*this > cur)
	{
		res = cur;
		sec = *this;
	}
	else
	{
		res = (*this);
		sec = cur;
	}
	do {
		next_day(&res);
		day_counter++;
	} while (!((res) == sec));
	return day_counter;
}

const bool & Date::is_leap(const unsigned int &year) const	//определение високосности
{
	if (year % 4 != 0) return 0;
	else
		if (year % 100 == 0 && year % 400 != 0) return 0;
		else return 1;
}

const bool & Date::is_leap() const
{
	return is_leap(dat.year);
}

void Date::leap_renew(const Date &cur) const	//обновление календаря
{
	if (is_leap(cur.dat.year)) {
		days_in_month[2] = 29;
	}
	else {
		days_in_month[2] = 28;
	}
}
void Date::next_day(Date *cur)	//+1 день
{
	cur->dat.day += 1;
	if (cur->dat.day > days_in_month[cur->dat.month]) {
		cur->dat.day = 1;
		cur->dat.month += 1;
		if (cur->dat.month > 12) {
			cur->dat.month = 1;
			cur->dat.year += 1;
			leap_renew(*cur);
		}
	}
}

void Date::prev_day(Date *cur)	//-1 день
{
	cur->dat.day -= 1;
	if (cur->dat.day < 1) {
		cur->dat.month -= 1;
		cur->dat.day = days_in_month[cur->dat.month];
		if (cur->dat.month < 1) {
			cur->dat.month = 12;
			cur->dat.year -= 1;
			leap_renew(*cur);
		}
	}
}

void Date::set_year(unsigned int &y)	//изменить год/месяц/день
{
	dat.year = y;
	leap_renew(*this);
}
void Date::set_month(unsigned int m)
{
	m == 0 ? m = 1 : 0;
	m > 12 ? m = 12 : 0;
	dat.month = m;
	leap_renew(*this);
}
void Date::set_day(unsigned int d)
{
	d == 0 ? d = 1 : 0;
	d > days_in_month[dat.month] ? d = days_in_month[dat.month] : 0;
	dat.day = d;
	leap_renew(*this);
}


const unsigned int & Date::get_year() const	//вернуть год/месяц/день
{
	return dat.year;
}
const unsigned int & Date::get_month() const
{
	return dat.month;
}
const unsigned int & Date::get_day() const
{
	return dat.day;
}
void Date::print() const	//вывести полную дату на экран
{
	printf("%d.%d.%d\n", dat.year, dat.month, dat.day);
}

int Date::quantity()
{
	return date_counter;
}

int Date::date_counter = 0;