#pragma once
#include <iostream>
#include <string>
#include "exeption.h"
class Matrix;

class Vector {
	int v_size;
	double *v_arr;

public:
	//конструкторы
	Vector();
	explicit Vector(int p_v_size);
	Vector(int p_v_size, double default_value);
	Vector(const Vector &p_vec);
	//деструктор
	~Vector();
	//операторы
	const Vector & operator=(const Vector &p_vec);
	const Vector & operator+=(const Vector &p_vec);
	const Vector & operator-=(const Vector &p_vec);
		//СЛОЖЕНИЕ
	
	const Vector& operator+() const;
	const Vector& operator++();
	const Vector operator++(int);
		//ВЫЧИТАНИЕ
	const Vector & operator-();
	const Vector& operator--();
	const Vector operator--(int);
		//УМНОЖЕНИЕ
	const Vector operator*(const double &q) const;//Vector*Num
	const double operator*(const Vector &p_vec) const;//Vector*Vector;
	friend const Vector operator*(const Matrix &p_mat, const Vector &p_vec);	//Matrix*Vector

		//ПРОЧЕЕ
	const double& operator[](int p_ind) const ;
	double& operator[](int p_ind);
	explicit operator double()
	{
		return length();
	}
	friend std::ostream& operator<<(std::ostream &p_out, const Vector &p_vec);
	friend std::istream& operator>>(std::istream &p_in, Vector &o_vec);
	double length() const;
	int size() const;
};

const Vector operator-(Vector p_vec1, const Vector &p_vec2);
const Vector operator+(Vector p_vec1, const Vector &p_vec2);
const Vector operator*(const double &q, const Vector &p_vec);	//Num*Vector



