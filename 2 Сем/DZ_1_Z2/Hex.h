#pragma once
#include "string.h"
#include "Array.h"
#include <stdexcept>

class Hex : public Array
{
public:
	Hex() : Array() {};
	Hex(const int& p_size, const unsigned char& def_val = '0') : Array(p_size, def_val) {};
	Hex(const char str[]) : Array(str) 
	{
		for (int i = 0; i < size; i++)
			if (!((str[i] >= '0' && str[i] <= '9') || (str[i] >= 'a' && str[i] <= 'f') || (str[i] >= 'A' && str[i] <= 'F')))
				throw invalid_argument("В записи шестнадцатеричного числа использована недопустимая цифра");
	};

	void operator=(const Array& cur)
	{
		this->Array::operator=(cur);
	}

	const int inDec(const unsigned char& cur)
	{
		if ((cur >= '0') && (cur <= '9')) return cur - 48;
		else if ((cur >= 'a') && (cur <= 'f')) return cur - 87;
		else if ((cur >= 'A') && (cur <= 'F')) return cur - 55;
	}
	const unsigned char inHex(const int& cur)
	{
		if (cur <= 9) return cur + 48;
		else return cur + 55;
	}
	//ОПЕРАТОР
	const Array operator+(Array & cur) override
	{
		Array res(size >= cur.GetSize() ? size : cur.GetSize());
		int b_size, s_size;
		if (size > cur.GetSize())
		{
			b_size = size;
			s_size = cur.GetSize();
		}
		else
		{
			b_size = cur.GetSize();
			s_size = size;
		}
		int i = 0;
		unsigned char rem = '0';
		while (i != s_size)
		{
			res.GetArr()[i] = inHex((inDec(arr[i]) + inDec(cur.GetArr()[i]) + inDec(rem)) % 16);
			rem = inHex((inDec(arr[i]) + inDec(cur.GetArr()[i])) / 15);
			i++;
		}
		if (size > cur.GetSize())
			while (i != b_size)
			{
				res.GetArr()[i] = inHex((inDec(arr[i]) + inDec(rem)) % 16);
				rem = inHex((inDec(arr[i])) / 15);
				i++;
			}
		else if (size < cur.GetSize())
			while (i != b_size)
			{
				res.GetArr()[i] = inHex((inDec(cur.GetArr()[i]) + inDec(rem)) % 16);
				rem = inHex((inDec(cur.GetArr()[i])) / 15);
				i++;
			}
		if (rem != '0')
		{
			if (b_size + 1 > max_size) throw std::length_error("Результат сложения превысил допустимый размер");
			Array res2(b_size + 1);
			for (int i = 0; i < b_size; i++)
				res2.GetArr()[i] = res.GetArr()[i];
			res2.GetArr()[b_size] = rem;
			res2.GetArr()[b_size + 1] = '\0';
			return res2;
		}
		res.GetArr()[b_size] = '\0';
		return res;
	}
	//ФУНКЦИЯ
	const Array summ(Array& cur1, Array& cur2) override
	{
		Array res(cur1.GetSize() >= cur2.GetSize() ? cur1.GetSize() : cur2.GetSize());
		int b_size, s_size;
		if (cur1.GetSize() > cur2.GetSize())
		{
			b_size = cur1.GetSize();
			s_size = cur2.GetSize();
		}
		else
		{
			b_size = cur2.GetSize();
			s_size = cur1.GetSize();
		}
		int i = 0;
		unsigned char rem = '0';
		while (i != s_size)
		{
			res.GetArr()[i] = inHex((inDec(cur1.GetArr()[i]) + inDec(cur2.GetArr()[i]) + inDec(rem)) % 16);
			rem = inHex((inDec(cur1.GetArr()[i]) + inDec(cur2.GetArr()[i])) / 15);
			i++;
		}
		if (cur1.GetSize() > cur2.GetSize())
			while (i != b_size)
			{
				res.GetArr()[i] = inHex((inDec(cur1.GetArr()[i]) + inDec(rem)) % 16);
				rem = inHex((inDec(cur1.GetArr()[i])) / 15);
				i++;
			}
		else if (cur1.GetSize() < cur2.GetSize())
			while (i != b_size)
			{
				res.GetArr()[i] = inHex((inDec(cur2.GetArr()[i]) + inDec(rem)) % 16);
				rem = inHex((inDec(cur2.GetArr()[i])) / 15);
				i++;
			}
		if (rem != '0')
		{
			if (b_size + 1 > max_size) throw std::length_error("Результат сложения превысил допустимый размер");
			Array res2(b_size + 1);
			for (int i = 0; i < b_size; i++)
				res2.GetArr()[i] = res.GetArr()[i];
			res2.GetArr()[b_size] = rem;
			res2.GetArr()[b_size + 1] = '\0';
			return res2;
		}
		res.GetArr()[b_size] = '\0';
		return res;
	}
};