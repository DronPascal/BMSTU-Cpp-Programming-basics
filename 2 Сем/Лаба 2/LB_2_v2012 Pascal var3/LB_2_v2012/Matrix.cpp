#include "Matrix.h"
#include "Vector.h"
#include <iostream>
#include <string>
#include "exeption.h"
Matrix::Matrix() :	//
	m_raw(0),
	m_col(0),
	m_mat(nullptr)
{};

Matrix::Matrix(int p_raw, int p_col) :	//
	m_raw(p_raw),
	m_col(p_col),
	m_mat(new double*[p_raw])
{
	for (int i = 0; i < p_raw; i++)
		m_mat[i] = new double[p_col];
	for (int i = 0; i < p_raw; i++)
		for (int j=0; j < p_col; j++)
			m_mat[i][j] = 0;
};

Matrix::Matrix(int p_raw, int p_col, double default_value) :	//
	m_raw(p_raw),
	m_col(p_col),
	m_mat(new double*[p_raw])
{
	for (int i = 0; i < p_raw; i++)
		m_mat[i] = new double[p_col];
	for (int i = 0; i < p_raw; i++)
		for (int j=0; j < p_col; j++)
			m_mat[i][j] = default_value;
};

Matrix::Matrix(const Matrix &p_mat) :	//
	m_raw(p_mat.m_raw),
	m_col(p_mat.m_col),
	m_mat(new double*[p_mat.m_raw])
{
	for (int i = 0; i < p_mat.m_raw; i++)
		m_mat[i] = new double[p_mat.m_col];
	for (int i = 0; i < p_mat.m_raw; i++)
		for (int j=0; j < p_mat.m_col; j++)
			m_mat[i][j] = p_mat.m_mat[i][j];
};

Matrix::~Matrix()
{
	for (int i = 0; i < m_raw; i++)
		delete[] m_mat[i];
	delete[] m_mat;
}

const Matrix & Matrix::operator=(const Matrix &p_mat)	//Matrix=Matrix
{
	if (this == &p_mat)
		return *this;
	if ((m_raw != p_mat.m_raw) || (m_col != p_mat.m_col))
	{
		delete m_mat;
		m_raw = p_mat.m_raw;
		m_col = p_mat.m_col;
		m_mat = new double*[m_raw];
		for (int i = 0; i < m_raw; i++)
			m_mat[i] = new double[m_col];
	}
	for (int i = 0; i < m_raw; i++)
		for (int j = 0; j < m_col; j++)
			m_mat[i][j] = p_mat.m_mat[i][j];
	return (*this);
}

const Matrix & Matrix::operator+=(const Matrix &p_mat)	//Matrix+=Matrix
{
	//cls_IncompatibleDimException ?
	if (p_mat.m_raw != m_raw)
		throw cls_IncompatibleDimException(m_raw, p_mat.m_raw);
	if (p_mat.m_col != m_col)
		throw cls_IncompatibleDimException(m_col, p_mat.m_col);
	for (int i = 0; i < m_raw; i++)
		for (int j = 0; j < m_col; j++)
			m_mat[i][j] += p_mat.m_mat[i][j];
	return (*this);
}

const Matrix & Matrix::operator-=(const Matrix &p_mat)	//Matrix-=Matrix
{
	//cls_IncompatibleDimException ?
	if (p_mat.m_raw != m_raw)
		throw cls_IncompatibleDimException(m_raw, p_mat.m_raw);
	if (p_mat.m_col != m_col)
		throw cls_IncompatibleDimException(m_col, p_mat.m_col);
	for (int i = 0; i < m_raw; i++)
		for (int j = 0; j < m_col; j++)
			m_mat[i][j] -= p_mat.m_mat[i][j];
	return (*this);
}



const Matrix& Matrix::operator++()	//++Matrix
{
	//cls_IncompatibleDimException ?
	if (this->m_col != this->m_raw)
		throw cls_IncompatibleDimException(this->m_raw, this->m_col);	
	for (int i = 0; i <	this->m_raw; i++)
		++(this->m_mat[i][i]);
	return (*this);
}

const Matrix & Matrix::operator+() const
{
	return (*this);
}

const Matrix Matrix::operator++(int)
{
	Matrix temp(*this);
	//cls_IncompatibleDimException ?
	if (m_col != m_raw)
		throw cls_IncompatibleDimException(m_raw, m_col);
	for (int i = 0; i < m_raw; i++)
		m_mat[i][i]++;
	return temp;
}

const Matrix& Matrix::operator-()
{
	for (int i = 0; i < m_raw; i++)
		for (int j = 0; j < m_col; j++)
			m_mat[i][j] *= -1;
	return (*this);
}

const Matrix& Matrix::operator--()	//--Matrix
{
	//cls_IncompatibleDimException ?
	if (this->m_col != this->m_raw)
		throw cls_IncompatibleDimException((this->m_raw), (this->m_col));	
	for (int i = 0; i < (this->m_raw); i++)
		--(this->m_mat[i][i]);
	return (*this);
}

const Matrix Matrix::operator--(int)
{
	Matrix temp(*this);
	//cls_IncompatibleDimException ?
	if (m_col != m_raw)
		throw cls_IncompatibleDimException(m_raw, m_col);
	for (int i = 0; i < m_raw; i++)
		m_mat[i][i]--;
	return temp;
}

const Matrix Matrix::operator*(const Matrix &p_mat) const	//Matrix*Matrix
{
	Matrix res(m_raw, p_mat.m_col);
	//cls_IncompatibleDimException ?
	if (m_col != p_mat.m_raw)
		throw cls_IncompatibleDimException(m_col, p_mat.m_raw);
	for (int i = 0; i < m_raw; i++)
		for (int j = 0; j < p_mat.m_col; j++)
			for (int k = 0; k<m_col; k++)
				res.m_mat[i][j] += m_mat[i][k] * p_mat.m_mat[k][j];
	return (res);
}

const Matrix Matrix::operator*(const double &q) const	//Matrix*Num
{
	Matrix res(*this);
	for (int i = 0; i < m_raw; i++)
		for (int j = 0; j < m_col; j++)
			res.m_mat[i][j] = res.m_mat[i][j]*q;
	return (res);
}

const Vector operator*(const Matrix &p_mat, const Vector &p_vec) //Matrix*Vector
{
	//cls_IncompatibleDimException ?
	if (p_vec.v_size != p_mat.m_col)
		throw cls_IncompatibleDimException(p_vec.v_size, p_mat.m_raw);
	if(p_mat.m_col == 1)
		throw cls_IncompatibleDimException(p_mat.m_col, 0);
	Vector res(p_vec);
	float sum = 0;
	for (int i = 0; i < p_mat.m_raw; i++)
	{

		for (int j = 0; j < p_mat.m_col; j++)
			sum += p_mat.m_mat[i][j] * p_vec.v_arr[j];
		res.v_arr[i] = sum;
		sum = 0;
	}
	return res;
}



const double Matrix::operator()(int raw_id, int col_id) const
{
	//cls_ArrOutOfRange ? 
	if (raw_id < 0 || raw_id > m_col || col_id < 0 || col_id>m_col)
		throw cls_OutOfRangeException(raw_id, m_raw, col_id, m_col);
	return m_mat[raw_id][col_id];
}

std::ostream& operator<<(std::ostream &p_out, const Matrix &p_mat)
{
	p_out << "\n";
	if (p_mat.m_raw==1)
	{
		p_out << "(  ";
		for (int j=0; j < p_mat.m_col; j++)
			p_out << p_mat.m_mat[0][j] << "  ";
		p_out << ")" << std::endl;
	}
	else 
	{
		p_out << "/  ";
		for (int j=0; j < p_mat.m_col; j++)
			p_out << p_mat.m_mat[0][j] << "  ";
		p_out << "\\" << std::endl;
		for (int i = 1; i < p_mat.m_raw-1; i++)
		{
			p_out << "|  ";
			for (int j=0; j < p_mat.m_col; j++)
				p_out << p_mat.m_mat[i][j] << "  ";
			p_out << "|" << std::endl;
		}
		p_out << "\\  ";
		for (int j=0; j < p_mat.m_col; j++)
				p_out << p_mat.m_mat[p_mat.m_raw-1][j] << "  ";
		p_out << "/" << std::endl;
	}
	return p_out;
}

std::istream& operator>>(std::istream &p_in,Matrix &o_mat)
{
	std::cout << "Введите количество строк" << std::endl;
	p_in >> o_mat.m_raw;
	std::cout << "Введите количество столбцов" << std::endl;
	p_in >> o_mat.m_col;
	o_mat.m_mat = new double*[o_mat.m_raw];
		for (int i = 0; i < o_mat.m_raw; i++)
			o_mat.m_mat[i] = new double[o_mat.m_col];
	for (int i = 0; i < o_mat.m_raw; i++)
		for (int j=0; j < o_mat.m_col; j++)
			p_in >> o_mat.m_mat[i][j];
	return p_in;
}

const Matrix Matrix::transp() const
{
	Matrix res(m_col, m_raw);
	for (int i = 0; i < m_col; i++)
		for (int j = 0; j < m_raw; j++)
			res.m_mat[i][j] = m_mat[j][i];
	return res;
}

const int Matrix::raws() const
{
	return m_raw;
}

const int Matrix::columns() const
{
	return m_col;
}

const Matrix operator*(const double &q, const Matrix &p_mat)//Num*Matrix
{
	return (p_mat*q);
}

const Matrix operator+(Matrix p_mat1, const Matrix &p_mat2) 	//Matrix+Matrix
{
	return (p_mat1 += p_mat2);
}

const Matrix operator-(Matrix p_mat1, const Matrix &p_mat2) 	//Matrix-Matrix
{
	return (p_mat1 -= p_mat2);
}
