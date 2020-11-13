#pragma once
#include <iostream>
#include <string>
#include "exeption.h"

class Vector;

class Matrix {
	int m_raw, m_col;
	double **m_mat;
public:
	//êîíñòğóêòîğû
	Matrix();
	explicit Matrix(int p_raw, int p_col);
	explicit Matrix(int p_raw, int p_cole, double default_value);
	Matrix(const Matrix &p_mat);
	//äåñòğóêòîğ
	~Matrix();
	//îïåğàòîğû
	const Matrix & operator=(const Matrix &p_mat);	//Matrix=Matrix
	const Matrix & operator+=(const Matrix &p_mat);//Matrix+=Matrix
	const Matrix & operator-=(const Matrix &p_mat);	//Matrix-=Matrix
	//ÑËÎÆÅÍÈÅ
	const Matrix& operator++();	//Matrix+E
	const Matrix & Matrix::operator+() const;
	const Matrix operator++(int);
	//ÂÛ×ÈÒÀÍÈÅ
	const Matrix& operator--();	//Matrix-E
	const Matrix& operator-();
	const Matrix operator--(int);
	//ÓÌÍÎÆÅÍÈÅ
	const Matrix operator*(const Matrix &p_mat) const;//Matrix*Matrix
	const Matrix operator*(const double &q) const;//Matrix*Num
	friend const Vector operator*(const Matrix &p_mat,const Vector &p_vec);	//Matrix*Vector
	//ÏĞÎ×ÅÅ
	const double operator()(const int raw_id,const int col_id) const;
	friend std::ostream& operator<<(std::ostream &p_out, const Matrix &p_mat);
	friend std::istream& operator>>(std::istream &p_in,Matrix &o_mat);
	const Matrix transp() const;
	const int raws() const;
	const int columns() const;
};

const Matrix operator*(const double &q, const Matrix &p_mat);//Num*Matrix
const Matrix operator+(Matrix p_mat1, const Matrix &p_mat2); 	//Matrix+Matrix
const Matrix operator-(Matrix p_mat1, const Matrix &p_mat2); 	//Matrix-Matrix

