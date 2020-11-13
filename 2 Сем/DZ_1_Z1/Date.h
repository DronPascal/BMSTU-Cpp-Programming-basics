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
	Date();	//������������
	Date (const unsigned int &year, const unsigned int &month, const unsigned int &day);
	explicit Date(const char *data);
	Date(const Date &cur);
	~Date();	//����������

	void operator=(const Date *cur);
	const Date & operator+(const unsigned int &num);	//���������� num ����
	const Date & operator-(const unsigned int &num);	//�������� num ����
	const bool & operator>(const Date &cur);
	const bool & operator<(const Date &cur);
	const bool & operator==(const Date &cur);
	const unsigned int & operator-(const Date &cur);	//���-�� ���� ����� ������

	const bool & is_leap(const unsigned int &year) const;
	const bool & is_leap() const;
	void leap_renew(const Date &cur) const;
	void next_day(Date *cur);	//����������� 1 ��� � ����
	void prev_day(Date *cur);	//�������� 1 ���� �� ����

	void set_year(unsigned int &y);	//������� ������
	void set_month(unsigned int m);
	void set_day(unsigned int d);

	const unsigned int & get_year() const;	//������� ����������
	const unsigned int & get_month() const;
	const unsigned int & get_day() const;
	void print() const;

	static int quantity();
};

