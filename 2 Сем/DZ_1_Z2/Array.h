#pragma once
#include <stdexcept>
using namespace std;
class Array
{
protected:
	unsigned char* arr;
	int size;
	static const int max_size = 100;
public:
	Array()
	{
		arr = nullptr;
		size = 0;
	}
	explicit Array(const int& p_size, const unsigned char& def_val = '0')
	{
		//Исключение по max_size
		if ((p_size < 0) || (p_size) > max_size) throw length_error("Недопустимый размер массива");
		size = p_size;
		arr = new unsigned char[size + 1];
		for (int i = 0; i < size; i++)
			arr[i] = def_val;
		arr[size] = '\0';
	}
	Array(const char str[])
	{
		//искл по size
		if (strlen(str) > max_size) throw length_error("Недопустимый размер массива");
		size = strlen(str);
		arr = new unsigned char[size + 1];
		for (int i = 0; i < size; i++)
		{
			
			arr[i] = str[size - 1 - i];	//Младшая цифра имеет меньший индекс
		}
		arr[size] = '\0';
	}

	void operator=(const Array &cur)
	{
		if (size == cur.size)
			for (int i = 0; i <= size; i++)
				arr[i] = cur.arr[i];
		else
		{
			delete[] arr;
			size = cur.size;
			arr = new unsigned char[size + 1];
			for (int i = 0; i <= size; i++)
				arr[i] = cur.arr[i];
		}
	}

	const unsigned char& operator[](const int& ind)
	{
		//Исключение по size
		if ((ind < 0) || (ind >= size))
			throw out_of_range("Попытка обратиться к несуществующему элементу массива");
		return arr[ind];
	}

	virtual const Array operator+(Array & cur)
	{
		Array res(size > cur.size ? size : cur.size);
		int b_size, s_size;
		if (size > cur.size)
		{
			b_size = size;
			s_size = cur.size;
			for (int i = s_size; i < b_size; i++)
				res.arr[i] = arr[i];
		}
		else
		{
			b_size = cur.size;
			s_size = size;
			for (int i = s_size; i < b_size; i++)
				res.arr[i] = cur.arr[i];
		}
		for (int i = 0; i < s_size; i++)
			res.arr[i] = arr[i] + cur.arr[i];
		return res;
	}

	virtual const Array summ(Array& cur1, Array& cur2)
	{
		Array res(cur1.size > cur2.size ? cur1.size : cur2.size);
		int b_size, s_size;
		if (cur1.size > cur2.size)
		{
			b_size = cur1.size;
			s_size = cur2.size;
			for (int i = s_size; i < b_size; i++)
				res.arr[i] = cur1.arr[i];
		}
		else
		{
			b_size = cur2.size;
			s_size = cur1.size;
			for (int i = s_size; i < b_size; i++)
				res.arr[i] = cur2.arr[i];
		}
		for (int i = 0; i < s_size; i++)
			res.arr[i] = cur1.arr[i] + cur2.arr[i];
		return res;
	}

	void print()
	{
		for (int i = size - 1; i >= 0; i--)
			printf("%c", arr[i]);
	}

	int GetSize()
	{
		return size;
	}

	unsigned char* GetArr()
	{
		return arr;
	}
};