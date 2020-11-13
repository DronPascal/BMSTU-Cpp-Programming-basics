#pragma once
#include <iostream>
#include "Date.h"
#include "Payment.h"

class Application
{
public:
	static void Run()
	{
		setlocale(0, "russian");
		int key;
		char dat[15];
		unsigned int days;

		std::cout << "¬ведите первую дату в формате (год.мес€ц.день): " << std::endl;
		std::cin >> dat;
		Date date1(dat);
		std::cout << "¬ведите текущую дату в формате (год.мес€ц.день): " << std::endl;
		std::cin >> dat;
		Date date2(dat);
		do {
			std::cout << "\t\t\t\t\t\t-----------------" << std::endl;
			std::cout << "\t\t\t\t\t\t1. date1 = date2" << std::endl;
			std::cout << "\t\t\t\t\t\t2. date1 + days" << std::endl;
			std::cout << "\t\t\t\t\t\t3. date1 - days" << std::endl;
			std::cout << "\t\t\t\t\t\t4. date1 > date2" << std::endl;
			std::cout << "\t\t\t\t\t\t5. date1 < date2" << std::endl;
			std::cout << "\t\t\t\t\t\t6. date1 == date2" << std::endl;
			std::cout << "\t\t\t\t\t\t7. date1 - date2" << std::endl;
			std::cout << "\t\t\t\t\t\t8. date1.is_leap()" << std::endl;
			std::cout << "\t\t\t\t\t\t9. date1.set_year(y)" << std::endl;
			std::cout << "\t\t\t\t\t\t10. date1.set_month(m)" << std::endl;
			std::cout << "\t\t\t\t\t\t11. date1.set_day(d)" << std::endl;
			std::cout << "\t\t\t\t\t\t12. date1.get_year()" << std::endl;
			std::cout << "\t\t\t\t\t\t13. date1.get_month()" << std::endl;
			std::cout << "\t\t\t\t\t\t14. date1.get_day()" << std::endl;
			std::cout << "\t\t\t\t\t\t15. Date::quantity()" << std::endl;
			std::cout << "\t\t\t\t\t\t16. ¬џ’ќƒ" << std::endl;
			std::cout << "\t\t\t\t\t\t-----------------" << std::endl;
			std::cin >> key;
			switch (key)
			{
			case 1:
				date1 = date2;
				std::cout << "date1 = date2\nAnswer: ";
				date1.print();
				break;
			case 2:
				std::cout << "date1 + days\n";
				std::cout << "¬ведите кол-во дней: ";
				std::cin >> days;
				date1 = date1 + days;
				std::cout << "Answer: ";
				date1.print();
				break;
			case 3:
				std::cout << "date1 - days\n";
				std::cout << "¬ведите кол-во дней: ";
				std::cin >> days;
				date1 = date1 - days;
				std::cout << "Answer: ";
				date1.print();
				break;
			case 4:
				std::cout << "date1 > date2\nAnswer: ";
				date1 > date2 ? printf("True\n") : printf("False\n");
				break;
			case 5:
				std::cout << "date1 < date2\nAnswer: ";
				date1 < date2 ? printf("True\n") : printf("False\n");
				break;
			case 6:
				std::cout << "date1 == date2\nAnswer: ";
				date1 == date2 ? printf("True\n") : printf("False\n");
				break;
			case 7:
				std::cout << "date1 - date2\nAnswer: " << date1 - date2 << std::endl;
				break;
			case 8:
				std::cout << ". date1.is_leap()\nAnswer: ";
				date1.is_leap() ? printf("True\n") : printf("False\n");
				break;
			case 9:
				std::cout << "date1.set_year(y)\n¬ведите год: ";
				unsigned int y; std::cin >> y;
				std::cout << "\nAnswer: "; date1.print();
				break;
			case 10:
				std::cout << "date1.set_month(m)\n¬ведите номер мес€ца: ";
				unsigned int m; std::cin >> m;
				std::cout << "\nAnswer: "; date1.print();
				break;
			case 11:
				std::cout << "date1.set_day(d)\n¬ведите день: ";
				unsigned int d; std::cin >> d;
				std::cout << "\nAnswer: "; date1.print();
				break;
			case 12:
				std::cout << "date1.get_year()\nAnswer: " << date1.get_year() << std::endl;
				break;
			case 13:
				std::cout << "date1.get_month()\nAnswer: " << date1.get_month() << std::endl;
				break;
			case 14:
				std::cout << "date1.get_day()\nAnswer: " << date1.get_day() << std::endl;
				break;
			case 15:
				std::cout << "Date::quantity()\nAnswer: " << Date::quantity() << std::endl;
				break;
			case 16: 
				break;
			default: 
				std::cout << "Ќеверный код операци\n";
			}
		} while (key != 16);

		Payment pers;
		do {
			std::cout << "\t\t\t\t\t\t-----------------" << std::endl;
			std::cout << "\t\t\t\t\t\t1. pers.experience_is()" << std::endl;
			std::cout << "\t\t\t\t\t\t2. pers.accured_is()" << std::endl;
			std::cout << "\t\t\t\t\t\t3. pers.withheld_is()" << std::endl;
			std::cout << "\t\t\t\t\t\t4. pers.handout_is()" << std::endl;
			std::cout << "\t\t\t\t\t\t5. Payment::quantity()" << std::endl;
			std::cout << "\t\t\t\t\t\t6. ¬џ’ќƒ" << std::endl;
			std::cout << "\t\t\t\t\t\t-----------------" << std::endl;
			std::cin >> key;
			switch (key)
			{
			case 1:
				std::cout << "pers.experience_is(cur_date)\n";
				std::cout << "Answer:" << pers.experience_is(date2) << std::endl;
				break;
			case 2:
				std::cout << "pers.accured_is()\n";
				std::cout << "Answer: " << pers.accured_is() << std::endl;
				break;
			case 3:
				std::cout << "pers.withheld_is()\n";
				std::cout << "Answer: " << pers.withheld_is() << std::endl;
				break;
			case 4:
				std::cout << "pers.handout_is()\n";
				std::cout << "Answer: " << pers.handout_is() << std::endl;
				break;
			case 5:
				std::cout << "Payment::quantity()\n";
				std::cout << "Answer: " << Payment::quantity() << std::endl;
				break;
			case 6:
				break;
			default:
				std::cout << "Ќеверный код операци\n";
			}
		} while (key != 6);
	}
};