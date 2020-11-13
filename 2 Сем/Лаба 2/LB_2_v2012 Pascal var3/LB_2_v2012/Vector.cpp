#include "Vector.h"
#include "Matrix.h"
#include <iostream>
#include <string>
#include "exeption.h"


Vector::Vector() :	//
	v_size(0),
	v_arr(nullptr)
{};

Vector::Vector(int p_v_size) :	//
	v_size(p_v_size),
	v_arr(new double[p_v_size])
{
	for (int i = 0; i < v_size; i++)
		v_arr[i] = 0;
}

Vector::Vector(int p_v_size, double default_value) :	//
	v_size(p_v_size),
	v_arr(new double[p_v_size])
{
	for (int i = 0; i < v_size; i++)
		v_arr[i] = default_value;
};

Vector::Vector(const Vector &p_vec) :	//
	v_size(p_vec.v_size),
	v_arr(new double[p_vec.v_size])
{
	for (int i = 0; i < v_size; i++)
		v_arr[i] = p_vec.v_arr[i];
};

Vector::~Vector()
{
	delete[] v_arr;
}

const Vector  & Vector:: operator=(const Vector &p_vec)
{
	if (this == &p_vec)
		return *this;
	if (v_size != p_vec.v_size)
	{
		delete v_arr;
		v_size = p_vec.v_size;
		v_arr = new double[p_vec.v_size];
	}
	for (int i = 0; i < v_size; ++i)
		v_arr[i] = p_vec.v_arr[i];
	return (*this);
}

const Vector & Vector:: operator+=(const Vector &p_vec)
{
	//cls_IncompatibleDimException ?
	if (v_size != p_vec.v_size)
		throw cls_IncompatibleDimException(v_size, p_vec.v_size);
	for (int i = 0; i < v_size; ++i)
		v_arr[i] += p_vec.v_arr[i];
	return (*this);
}


const Vector & Vector::operator-=(const Vector &p_vec)
{
	//cls_IncompatibleDimException ?
	if (v_size != p_vec.v_size)
		throw cls_IncompatibleDimException(v_size, p_vec.v_size);
	for (int i = 0; i < v_size; ++i)
		v_arr[i] -= p_vec.v_arr[i];
	return (*this);
}

const Vector& Vector::operator+() const
{
	return (*this);
}

const Vector& Vector::operator++()	//++Vector
{
	for (int i = 0; i < this->v_size; ++i)
		++(this->v_arr[i]);
	return (*this);
}

const Vector Vector::operator++(int)
{
	Vector temp(*this);
	for (int i = 0; i < v_size; ++i)
		v_arr[i]++;
	return temp;
}




const Vector & Vector::operator-()
{
	for (int i = 0; i < v_size; i++)
		v_arr[i] *= -1;
	return (*this);
}

const Vector& Vector::operator--()	//--Vector
{
	for (int i = 0; i < this->v_size; ++i)
		--(this->v_arr[i]);
	return (*this);
}

const Vector Vector::operator--(int)
{
	Vector temp(*this);
	for (int i = 0; i < v_size; ++i)
		v_arr[i]--;
	return temp;
}

const Vector Vector::operator*(const double &q) const	//Vector*Num
{
	Vector res(*this);
	for (int i = 0; i < v_size; ++i)
		res.v_arr[i] =res.v_arr[i]*q;
	return res;
}

const double & Vector::operator[](int p_ind) const
{
	if (p_ind < 0 || p_ind>v_size)
		throw cls_OutOfRangeException(p_ind, v_size);
	return v_arr[p_ind];
}

double & Vector::operator[] (int p_ind) {
	if (p_ind < 0 || p_ind>v_size)
		throw cls_OutOfRangeException(p_ind, v_size);
	return v_arr[p_ind];

}

const double Vector::operator*(const Vector &p_vec) const	//Vector*Vector
{
	double res = 0;
	//cls_IncompatibleDimException ?
	if (v_size != p_vec.v_size)
		throw cls_IncompatibleDimException(v_size, p_vec.v_size);
	for (int i = 0; i < v_size; ++i)
		res += v_arr[i] * p_vec.v_arr[i];
	return res;
}



std::ostream& operator<<(std::ostream &p_out, const Vector &p_vec)
{
	p_out << " " << std::endl;
	p_out << "----" << std::endl;
	for (int i = 0; i < p_vec.v_size; ++i)
		p_out << p_vec.v_arr[i]<< "|" << std::endl;
	p_out << "----" << std::endl;
	return p_out;
}

std::istream& operator>>(std::istream &p_in, Vector &o_vec)
{
	p_in >> o_vec.v_size;
	o_vec.v_arr = new double[o_vec.v_size];
	for (int i = 0; i < o_vec.v_size; ++i)
		p_in >> o_vec.v_arr[i];
	return p_in;
}

double Vector::length() const
{
	double res = 0;
	for (int i = 0; i < v_size; ++i)
		res += this->v_arr[i] * this->v_arr[i];
	return sqrt(res);
}

int Vector::size() const
{
	return v_size;
}

const Vector operator-(Vector p_vec1, const Vector &p_vec2)
{
	//Vector res(*this);
	//cls_IncompatibleDimException ?
	return p_vec1 -= p_vec2;
}

const Vector operator+(Vector p_vec1, const Vector &p_vec2)
{
	//Vector res(*this);
	//cls_IncompatibleDimException ?
	return p_vec1 += p_vec2;
}

const Vector operator*(const double &q, const Vector &p_vec)	//Num*Vector
{
	return (p_vec * q);
}