#pragma once
#include <initializer_list>
#include <iostream>
#include "ListIterator.h"

template <class _DataType, class Cell>
class ListIterator;

template <class _DataType>	
class List
{
private:
	class Cell
	{
		Cell *next, *prev;
		_DataType inf;
		friend ListIterator<_DataType, Cell>;
		friend List<_DataType>;
	public:
		Cell() :next(nullptr), prev(nullptr), inf() {};
		Cell(const _DataType &val) :next(nullptr), prev(nullptr), inf(val) {};
	};

	Cell *head, *tail;
	size_t size;

public:
	typedef ListIterator<_DataType, Cell> iterator;
	friend ListIterator<_DataType, Cell>;

	List() :head(nullptr), tail(nullptr), size(0) {};	//1.1.	Конструктор по умолчанию

	explicit List(size_t SIZE, _DataType val = _DataType())	//1.2.	Конструктор, принимающий количество элементов 
	{													//1.3.	Конструктор, принимающий количество элементов и значение, инициализирующее элементы списка.
		if (SIZE)
		{
			head = tail = new Cell();
			for (size_t i = 1; i < SIZE; i++)
			{
				Cell *tmp = new Cell(val);
				tmp->prev = tail;
				tail->next = tmp;
				tail = tmp;
			}
			size = SIZE;
		}
	}

	template<class _IterType>
	List(_IterType beg, _IterType end)	//1.4.	Конструктор, принимающий интервал [beg, end), заданный итераторами.
	{
		if (beg != end)
		{
			head = tail = new Cell(beg.ptr->inf);
			size = 1;
			++beg;
		}
		for (; beg != end; beg++)
		{
			Cell *tmp = new Cell(beg.ptr->inf);
			tmp->prev = tail;
			tail->next = tmp;
			tail = tmp;
			size++;
		}
	}

	List(std::initializer_list<_DataType> p_List) : size(p_List.size())	//1.5.Конструктор, принимающий список инициализации std::initializer_list
	{
		head = tail = new Cell(*(p_List.begin()));
		size = p_List.size();
		for (auto cur = p_List.begin() + 1; cur < p_List.end(); ++cur)
		{
			Cell *tmp = new Cell(*cur);
			tmp->prev = tail;
			tail->next = tmp;
			tail = tmp;
		}
	}

	List(List &_List) : List(_List.size)	//1.6.	Копирующий конструктор
	{
		auto cur = Begin();
		for (auto _cur = _List.Begin(); _cur != _List.End(); ++_cur)
		{
			*cur = *_cur;
			++cur;
		}
	}

	List(List &&_List)	//1.7.	Конструктор переноса
		: size(_List.size), head(_List.head), tail(_List.tail)
	{
		_List.size = 0;
		_List.head = nullptr;
		_List.tail = nullptr;
	}

	~List()
	{
		Clear();
	}
	//ФУНКЦИИ ОПЕРАЦИИ

	const List& operator=(List &_List)	//2.1.	Операция присваивания с копированием
	{
		Resize(_List.size);
		auto cur = Begin();
		for (auto _cur = _List.Begin(); _cur != _List.End(); ++_cur)
		{
			cur.ptr->inf = _cur.ptr->inf;
			++cur;
		}
	}

	const List& operator=(List &&_List)	//2.2.	Операция присваивания с переносом
	{
		if (*this == _List) return *this;
		head = _List.head;
		tail = _List.tail;
		size = _List.size;
		_List.head = nullptr;
		_List.tail = nullptr;
		_List.size = 0;
		return *this;
	}

	//МЕТОДЫ

	iterator Begin()
	{
		iterator tmp(head);
		return tmp;
	}

	iterator End()
	{
		iterator tmp(tail->next);
		return tmp;
	}

	void PushBack(const _DataType &elem)
	{
		if (tail)
		{
			Cell *tmp = new Cell(elem);
			tmp->prev = tail;
			tail->next = tmp;
			tail = tmp;
		}
		else head = tail = new Cell(elem);
		size++;
	}

	void PopBack()
	{
		if (tail)
		{
			tail = tail->prev;
			delete tail->next;
			tail->next = nullptr;
			size--;
		}
	}

	void PushFront(_DataType &elem)
	{
		if (head)
		{
			Cell* tmp = new Cell(elem);
			tmp->next = head;
			head->prev = tmp;
			head = tmp;
		}
		else  head = tail = new Cell(elem);
		size++;
	}

	void PopFront()
	{
		if (head)
		{
			head = head->next;
			delete head->prev;
			head->prev = nullptr;
			size--;
		}
	}

	iterator Insert(iterator &pos, _DataType &elem)
	{
		if (pos == Begin()) PushFront(elem);
		else if (pos == End()) PushBack(elem);
		else
		{
			Cell* tmp = new Cell(elem);
			tmp->prev = (pos.ptr)->prev;
			((pos.ptr)->prev)->next = tmp;
			tmp->next = pos.ptr;
			(pos.ptr)->prev = tmp;
		}
		size++;
		return --pos;
	}

	template<class _IterType>
	_IterType Insert(_IterType &pos, _IterType &beg, _IterType &end)
	{
		List<_DataType> buf(beg, end);
		if (typeid(*pos) != typeid(_DataType))
		{
			tail = buf.tail;
			head = buf.head;
		}
		else if (pos == Begin())
		{
			head->prev = buf.tail;
			buf.tail->next = head;
			head = buf.head;
		}
		else if (pos == End())
		{
			tail->next = buf.head;
			buf.head->prev = tail;
			tail = buf.tail;
		}
		else
		{
			pos.ptr->prev->next = buf.head;
			buf.head->prev = pos.ptr->prev;
			pos.ptr->prev = buf.tail;
			buf.tail->next = pos.ptr;
		}
		size += buf.size;
		pos.ptr = buf.head;
		buf.head = nullptr;
		return pos;
	}

	template<class _IterType>
	_IterType Erase(_IterType &pos)
	{
		if (pos == Begin())
		{

			PopFront();
			pos = Begin();
		}
		else if (pos == End())
		{
			PopBack();
			pos = End();
		}
		else
		{
			_IterType a(pos);
			((pos.ptr)->prev)->next = (pos.ptr)->next;
			((pos.ptr)->next)->prev = (pos.ptr)->prev;
			++pos;
			delete a.ptr;
			size--;
		}
		return pos;
	}

	void Resize(size_t num)
	{
		if (num > size)
			for (size_t i = size; i < num; i++)
			{
				Cell* tmp = new Cell();
				tmp->prev = tail;
				tail->next = tmp;
				tail = tmp;
			}
		else if (num < size)
		{
			if (num == 0) Clear();
			else
			{
				for (size_t i = size; i > num; i--)
				{
					delete tail->next;
					tail = tail->prev;
				}
				tail->next = nullptr;
			}
		}
		else
			return;
		size = num;
	}

	bool Empty()
	{
		return (size == 0);
	}

	size_t Size()
	{
		return size;
	}

	void Clear()
	{
		Cell* cpyhead = head;
		while (cpyhead)
		{
			head = head->next;
			delete cpyhead;
			cpyhead = head;
		}
		head = tail = nullptr;
		size = 0;
	}
};




