#pragma once
#include "Algorithm.h"
struct Student
{
	//friend void ForEach<class _Iter,class _Func>(_Iter beg, _Iter end, _Func p_op);
	std::string Name, Group;
	double		AvrMark;
	friend std::ostream& operator<<(std::ostream &p_out, const Student &stud)
	{
		p_out << "\n	Name: " << stud.Name << "	Group: " << stud.Group << "	Average mark: " << stud.AvrMark;
		return p_out;
	}
	friend std::istream& operator>>(std::istream &p_in, Student &stud)
	{
		std::cout << "¬ведите им€: ";
		p_in >> stud.Name;
		std::cout << "¬ведите группу: ";
		p_in >> stud.Group;
		std::cout << "¬ведите средний балл: ";
		p_in >> stud.AvrMark;
		std::cout << std::endl;
		return p_in;
	}
	const Student & operator=(const Student &stud)
	{
		if (*this == stud)
			return (*this);
		Name = stud.Name;
		Group = stud.Group;
		AvrMark = stud.AvrMark;
		return (*this);
	}
	bool operator>(const Student &stud) const
	{
		return (AvrMark > stud.AvrMark);
	}
	bool operator<(const Student &stud) const
	{
		return (AvrMark < stud.AvrMark);
	}
	bool operator == (const Student &stud) const
	{
		if (Name == stud.Name &&
			Group == stud.Group &&
			AvrMark == stud.AvrMark)
			return true;
		else return false;
	}
	void ChangeAvrg()
	{
		this->AvrMark = 5;
	}
};

