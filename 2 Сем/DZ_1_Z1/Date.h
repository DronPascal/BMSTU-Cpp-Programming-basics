#pragma once
class Date
{
	struct dat
	{
		unsigned int
			year,
			month,
			day;
	};
	dat dat;

public:
	static int date_counter;
	Date();	//конструкторы
	Date (const unsigned int &year, const unsigned int &month, const unsigned int &day);
	explicit Date(const char *data);
	Date(const Date &cur);
	~Date();	//деструктор

	void operator=(const Date *cur);
	const Date & operator+(const unsigned int &num);	//прибавляет num дней
	const Date & operator-(const unsigned int &num);	//вычитает num дней
	const bool & operator>(const Date &cur);
	const bool & operator<(const Date &cur);
	const bool & operator==(const Date &cur);
	const unsigned int & operator-(const Date &cur);	//кол-во дней между датами

	const bool & is_leap(const unsigned int &year) const;
	const bool & is_leap() const;
	void leap_renew(const Date &cur) const;
	void next_day(Date *cur);	//прибавление 1 дня к дате
	void prev_day(Date *cur);	//вычитает 1 день из даты

	void set_year(unsigned int &y);	//функции записи
	void set_month(unsigned int m);
	void set_day(unsigned int d);

	const unsigned int & get_year() const;	//функции считывания
	const unsigned int & get_month() const;
	const unsigned int & get_day() const;
	void print() const;

	static int quantity();
};

