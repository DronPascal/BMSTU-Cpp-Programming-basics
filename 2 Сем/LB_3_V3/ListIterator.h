#pragma once
#include "List.h"

template <class _DataType>
class List;
template <class _DataType>
class Cell;

template <class _DataType, class Cell>
class ListIterator
{
private:
	Cell *ptr;
public:
	friend List<_DataType>;
	ListIterator() : ptr(nullptr) {}	//КОНСТРУКТОРЫ

	ListIterator(List<_DataType> *_List, size_t ind)
	{
		if (ind < 0 || ind > _List->size || _List->head == nullptr)
			throw std::out_of_range("Не существует элемента с таким индексом.");
		ptr = _List->head;
		for (size_t i = 0; i < ind; i++)
			++(*this);
	}

	explicit ListIterator(Cell *_Cell): ptr(_Cell)
	{}

	ListIterator(const ListIterator &Iter)
	{
		ptr = Iter.ptr;
	}

	ListIterator & operator++()
	{
		ptr = (ptr->next);
		return (*this);
	}

	ListIterator operator++(int)
	{
		ListIterator temp(*this);
		ptr = (ptr->next);
		return temp;
	}

	ListIterator & operator--()
	{
		ptr = (ptr->prev);
		return (*this);
	}

	ListIterator operator--(int)
	{
		ListIterator temp(*this);
		ptr = (ptr->prev);
		return temp;
	}

	_DataType & operator*() const
	{
		return (ptr->inf);
	}

	const ListIterator& operator=(ListIterator Iter)
	{
		if (*this != Iter)
			ptr = Iter.ptr;
		return (*this);
	}

	const ListIterator& operator=(Cell &cell)
	{
		if (*this->ptr != *cell)
			ptr = *cell;
		return (*this);
	}

	bool operator==(ListIterator Iter) const
	{
		return ((ptr == Iter.ptr));
	}

	bool operator!=(ListIterator Iter) const
	{
		return (!(*this == Iter));
	}

	_DataType* operator->()
	{
		return (&((*ptr).inf));
	}

	explicit operator _DataType()
	{
		return *ptr;
	}
};