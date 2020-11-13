#pragma once

#include <string>

class BadFileFormatException : public std::exception {
	std::string error;
public:
	BadFileFormatException() {
		error.assign("���������������� ������ �����.");
	}
	explicit BadFileFormatException(const std::string& file_format) {
		error.assign("������ \"" + file_format + "\" �� ��������������");
	}
	const char* what() const noexcept override {
		return error.c_str();
	}
};

class NoFileFoundException : public std::exception {
	std::string error;
public:
	NoFileFoundException() {
		error.assign("�� ���������� ������� ����.");
	}
	explicit NoFileFoundException(const std::string& file_) {
		error.assign("���� \"" + file_ + "\" �� ������� �������");
	}
	const char* what() const noexcept override {
		return error.c_str();
	}
};