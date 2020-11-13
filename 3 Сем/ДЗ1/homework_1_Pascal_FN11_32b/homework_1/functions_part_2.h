#pragma once
#include <iostream>
#include <fstream>
#include <algorithm>
#include <list>
#include <iterator>
#include <functional>
#include <ctime>
#include <iomanip>
#include <map>
#include <utility>
#include <unordered_set>
#include <string>
#include <stdio.h>
#include <algorithm>

//Use Ctrl+M+L

struct Drug	//структура лекарство
{
	int	 pharmacy_number;
	char drug_name[16];
	int	 availanle_packages_number,
		packege_price;
	char receipt_date[16];
	int	 shelf_life;

	bool operator==(const Drug& second_drug) const
	{
		return (second_drug.pharmacy_number == pharmacy_number &&
			std::string(second_drug.drug_name) == std::string(drug_name) &&
			second_drug.availanle_packages_number == availanle_packages_number &&
			second_drug.packege_price == packege_price &&
			std::string(second_drug.receipt_date) == std::string(receipt_date) &&
			second_drug.shelf_life == shelf_life);
	}

	friend std::ostream& operator << (std::ostream& p_out, const Drug& p_drug)
	{
		p_out
			<< std::setw(11) << std::left << p_drug.pharmacy_number
			<< std::setw(20) << std::left << p_drug.drug_name
			<< std::setw(12) << std::left << p_drug.availanle_packages_number
			<< std::setw(10) << std::left << p_drug.packege_price
			<< std::setw(14) << std::left << p_drug.receipt_date
			<< std::setw(14) << std::left << p_drug.shelf_life << std::endl;
		return p_out;
	}
};

namespace std
{
	template<> struct hash<Drug>
	{
		std::size_t operator()(Drug const& d) const noexcept
		{
			std::size_t seed = d.pharmacy_number;
			seed ^= stdext::hash_value(d.drug_name) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
			seed ^= stdext::hash_value(d.availanle_packages_number) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
			seed ^= stdext::hash_value(d.packege_price) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
			seed ^= stdext::hash_value(d.receipt_date) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
			seed ^= stdext::hash_value(d.shelf_life) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
			return seed;
		}
	};
}

std::map<int, Drug> Drugs_map;	//бесполезная таблица типо массива <индекс, структура> всех добавленных записей во время сессии (до завершения работы программы)
std::unordered_set<Drug> Drugs_unordered_set;	//для быстрой проверки существует ли уже такая запись в файле

//первичный ввод
bool createDrugDatabase(const std::string& filename)
{
	std::fstream fout(filename + ".bin", std::ios::binary | std::ios::out);
	if (fout.is_open())
	{
		std::cout << "Бинарный файл " + filename + " успешно открыт." << std::endl;
		Drug new_drug;
		int k;
		do
		{
			std::cout << "\nВведите данные о лекарстве:" << std::endl;
			std::cout << "Номер аптеки: ";
			std::cin >> new_drug.pharmacy_number;
			std::cout << "Название лекарства: ";
			std::cin >> new_drug.drug_name;
			std::cout << "Кол-во упаковок в наличии: ";
			std::cin >> new_drug.availanle_packages_number;
			std::cout << "Стоимость упаковки: ";
			std::cin >> new_drug.packege_price;
			std::cout << "Дата поступления: ";
			std::cin >> new_drug.receipt_date;
			std::cout << "Срок хранения: ";
			std::cin >> new_drug.shelf_life;

			if (Drugs_unordered_set.empty() || Drugs_unordered_set.find(new_drug) == Drugs_unordered_set.end())
			{
				Drugs_unordered_set.insert(new_drug);
				Drugs_map.insert(std::pair<int, Drug>(Drugs_map.size(), new_drug));
				fout.write(reinterpret_cast <char*>(&new_drug), sizeof(Drug));
			}
			else
			{
				std::cout << "Такая запись о лекарстве уже существует." << std::endl;
			}

			std::cout << "\nВведите 1 - для добавления лекарства, 2 - для сохранения базы лекарств:";
			std::cin >> k;
		} while (k != 2);
		fout.close();
		return 1;
	}
	else
	{
		std::cout << "Файл " + filename + " не удалось открыть." << std::endl;
		return 0;
	}
}

//дозапись
bool addToDrugDatabase(const std::string& filename)
{
	if (Drugs_unordered_set.empty())
	{
		std::fstream fin(filename + ".bin", std::ios::binary | std::ios::in);
		if (fin.is_open())
		{
			Drug cur_drug;
			while (fin.read(reinterpret_cast <char*>(&cur_drug), sizeof(Drug)))
				Drugs_unordered_set.insert(cur_drug);
			fin.close();
		}
	}
	std::cout << "Если хотите добавить лекарство в базу, введите 1: ";
	int m;
	std::cin >> m;
	if (m == 1)
	{
		std::fstream fout(filename + ".bin", std::ios::binary | std::ios::app);
		if (fout.is_open())
		{
			std::cout << "Бинарный файл " + filename + " успешно открыт." << std::endl;
			Drug new_drug;
			int k;
			do
			{
				std::cout << "\nВведите данные о добавляемом лекарстве:" << std::endl;
				std::cout << "Номер аптеки: ";
				std::cin >> new_drug.pharmacy_number;
				std::cout << "Название лекарства: ";
				std::cin >> new_drug.drug_name;
				std::cout << "Кол-во упаковок в наличии: ";
				std::cin >> new_drug.availanle_packages_number;
				std::cout << "Стоимость упаковки: ";
				std::cin >> new_drug.packege_price;
				std::cout << "Дата поступления: ";
				std::cin >> new_drug.receipt_date;
				std::cout << "Срок хранения: ";
				std::cin >> new_drug.shelf_life;

				if (Drugs_unordered_set.empty() || Drugs_unordered_set.find(new_drug) == Drugs_unordered_set.end())
				{
					Drugs_unordered_set.insert(new_drug);
					Drugs_map.insert(std::pair<int, Drug>(Drugs_map.size(), new_drug));
					fout.write(reinterpret_cast <char*>(&new_drug), sizeof(Drug));
				}
				else
				{
					std::cout << "Такая запись о лекарстве уже существует." << std::endl;
				}

				std::cout << "\nВведите 1 - для добавления лекарства, 2 - для сохранения базы лекарств:";
				std::cin >> k;
			} while (k != 2);
			fout.close();
			return 1;
		}
		else
		{
			std::cout << "Файл " + filename + " не удалось открыть." << std::endl;
			return 0;
		}
	}
}

//преобразовать .bin в .txt
bool convertToTxt(const std::string& filename)
{
	std::fstream fin(filename + ".bin", std::ios::binary | std::ios::in);
	std::ofstream fout(filename + ".txt");
	if (fin.is_open())
	{
		std::cout << "Бинарный файл " + filename + " успешно открыт." << std::endl;
		fout
			<< "ВЕДОМОСТЬ ЛЕКАРСТВ\n"
			<< std::setw(11) << std::left << "№ аптеки"
			<< std::setw(20) << std::left << "Название лекарства"
			<< std::setw(12) << std::left << "В наличии"
			<< std::setw(10) << std::left << "Цена"
			<< std::setw(14) << std::left << "Дата"
			<< std::setw(14) << std::left << "Срок годности\n";
		Drug cur_drug;
		while (fin.read(reinterpret_cast <char*>(&cur_drug), sizeof(Drug)))
			fout << cur_drug;
		fin.close();
		fout.close();
		return 1;
	}
	else
	{
		std::cout << "Файл " + filename + " не удалось открыть." << std::endl;
		return 0;
	}
}

//записать в .bin найденные по номеру аптеки записи о лекарствах и вернуть название файла
std::string findByNumber(const std::string& filename, const int num)
{
	std::fstream fin(filename + ".bin", std::ios::binary | std::ios::in);
	std::fstream fout(filename + "_pharmacy_number_" + std::to_string(num) + ".bin", std::ios::binary | std::ios::out);
	int founded = 0;
	if (fin.is_open())
	{
		Drug cur_drug;
		while (fin.read(reinterpret_cast <char*>(&cur_drug), sizeof(Drug)))
			if (cur_drug.pharmacy_number == num)
			{
				fout.write(reinterpret_cast <char*>(&cur_drug), sizeof(Drug));
				founded++;
			}
		fin.close();
		fout.close();
	}
	if (founded)
	{
		std::cout << "Результат поиска по номеру аптеки сохранен в файл \"" + filename + "_pharmacy_number_" + std::to_string(num) + ".bin\"" + " и позже может быть преобразован в txt." << std::endl;
		return filename + "_pharmacy_number_" + std::to_string(num);
	}
	else
	{
		std::cout << "Поиск не дал результатов. Файл результатов не был создан." << std::endl;
		remove((filename + "_pharmacy_number_" + std::to_string(num) + ".bin").c_str());
	}
}

//записать в .bin найденные по названию лекарства записи о нем и вернуть название файла
std::string findByName(const std::string& filename, const std::string& name)
{
	std::fstream fin(filename + ".bin", std::ios::binary | std::ios::in);
	std::fstream fout(filename + "_drug_name_" + name + ".bin", std::ios::binary | std::ios::out);
	int founded = 0;
	if (fin.is_open())
	{
		Drug cur_drug;
		while (fin.read(reinterpret_cast <char*>(&cur_drug), sizeof(Drug)))
			if (cur_drug.drug_name == name)
			{
				fout.write(reinterpret_cast <char*>(&cur_drug), sizeof(Drug));
				founded++;
			}
		fin.close();
		fout.close();
	}
	if (founded)
	{
		std::cout << "Результат поиска по названию лекарства сохранен в файл \"" + filename + "_drug_name_" + name + ".bin\"" + " и позже может быть преобразован в txt." << std::endl;
		return filename + "_drug_name_" + name;
	}
	else
	{
		std::cout << "Поиск не дал результатов. Файл результатов не был создан." << std::endl;
		remove((filename + "_drug_name_" + name + ".bin").c_str());
	}
}

//записать в .bin найденные по дате поступления записи о лекарствах и вернуть название файла
std::string findByDate(const std::string& filename, const std::string& date)
{
	std::fstream fin(filename + ".bin", std::ios::binary | std::ios::in);
	std::fstream fout(filename + "_receipt_date_" + date + ".bin", std::ios::binary | std::ios::out);
	int founded = 0;
	if (fin.is_open())
	{
		Drug cur_drug;
		while (fin.read(reinterpret_cast <char*>(&cur_drug), sizeof(Drug)))
			if (cur_drug.receipt_date == date)
			{
				fout.write(reinterpret_cast <char*>(&cur_drug), sizeof(Drug));
				founded++;
			}
		fin.close();
		fout.close();
	}
	if (founded)
	{
		std::cout << "Результат поиска по дате поступления сохранен в файл \"" + filename + "_receipt_date_" + date + ".bin\"" + " и позже может быть преобразован в txt." << std::endl;
		return filename + "_receipt_date_" + date;
	}
	else
	{
		std::cout << "Поиск не дал результатов. Файл результатов не был создан." << std::endl;
		remove((filename + "_receipt_date_" + date + ".bin").c_str());
	}
}

//удалить из .bin все записи, содержащиеся во втором .bin
void removeDrugs(const std::string& filename, const std::string& removable)
{
	std::fstream fin(filename + ".bin", std::ios::binary | std::ios::in);
	std::fstream fin_rem(removable + ".bin", std::ios::binary | std::ios::in);
	std::unordered_set<Drug> removable_drugs;
	if (fin_rem.is_open())
	{
		Drug cur_drug;
		while (fin_rem.read(reinterpret_cast <char*>(&cur_drug), sizeof(Drug)))
			removable_drugs.insert(cur_drug);
		fin_rem.close();
	}

	std::fstream fout(filename + "_temporarity.bin", std::ios::binary | std::ios::out);

	if (fin.is_open())
	{
		Drug cur_drug;
		while (fin.read(reinterpret_cast <char*>(&cur_drug), sizeof(Drug)))
			if (removable_drugs.find(cur_drug) == removable_drugs.end())
			{
				fout.write(reinterpret_cast <char*>(&cur_drug), sizeof(Drug));
			}
			else
			{
				Drugs_unordered_set.erase(cur_drug);
			}
		fin.close();
		fout.close();
	}

	remove((filename + ".bin").c_str());
	rename((filename + "_temporarity.bin").c_str(), (filename + ".bin").c_str());
	remove((filename + "_temporarity.bin").c_str());
}

//заменить в .bin все записи, содержащиеся во втором .bin на новые записи (ввести с клавиатуры)
void replaceDrugs(const std::string& filename, const std::string& replaceble)
{
	std::fstream fin(filename + ".bin", std::ios::binary | std::ios::in);
	std::fstream fin_rem(replaceble + ".bin", std::ios::binary | std::ios::in);
	std::unordered_set<Drug> removable_drugs;
	if (fin_rem.is_open())
	{
		Drug cur_drug;
		while (fin_rem.read(reinterpret_cast <char*>(&cur_drug), sizeof(Drug)))
			removable_drugs.insert(cur_drug);
		fin_rem.close();
	}

	std::fstream fout(filename + "_temporarity.bin", std::ios::binary | std::ios::out);

	if (fin.is_open())
	{
		Drug cur_drug;
		while (fin.read(reinterpret_cast <char*>(&cur_drug), sizeof(Drug)))
			if (removable_drugs.find(cur_drug) == removable_drugs.end())
				fout.write(reinterpret_cast <char*>(&cur_drug), sizeof(Drug));
			else
			{
				Drugs_unordered_set.erase(cur_drug);
				std::cout << "Описание заменяемого лекарства: "
					<< " " << cur_drug.pharmacy_number
					<< " " << cur_drug.drug_name
					<< " " << cur_drug.availanle_packages_number
					<< " " << cur_drug.packege_price
					<< " " << cur_drug.receipt_date
					<< " " << cur_drug.shelf_life << std::endl;
				std::cout << "\nЗаменяем на: " << std::endl;

				bool ok = 0;
				do
				{
					std::cout << "Номер аптеки: ";
					std::cin >> cur_drug.pharmacy_number;
					std::cout << "Название лекарства: ";
					std::cin >> cur_drug.drug_name;
					std::cout << "Кол-во упаковок в наличии: ";
					std::cin >> cur_drug.availanle_packages_number;
					std::cout << "Стоимость упаковки: ";
					std::cin >> cur_drug.packege_price;
					std::cout << "Дата поступления: ";
					std::cin >> cur_drug.receipt_date;
					std::cout << "Срок хранения: ";
					std::cin >> cur_drug.shelf_life;

					if (Drugs_unordered_set.empty() || Drugs_unordered_set.find(cur_drug) == Drugs_unordered_set.end())
					{
						Drugs_unordered_set.insert(cur_drug);
						Drugs_map.insert(std::pair<int, Drug>(Drugs_map.size(), cur_drug));
						fout.write(reinterpret_cast <char*>(&cur_drug), sizeof(Drug));
						ok = 1;
					}
					else
					{
						std::cout << "\nТакая запись о лекарстве уже существует.\nПовторите ввод данных о лекарстве\n" << std::endl;
					}
				} while (!ok);
			}
		fin.close();
		fout.close();
	}
	remove((filename + ".bin").c_str());
	rename((filename + "_temporarity.bin").c_str(), (filename + ".bin").c_str());
	remove((filename + "_temporarity.bin").c_str());
}