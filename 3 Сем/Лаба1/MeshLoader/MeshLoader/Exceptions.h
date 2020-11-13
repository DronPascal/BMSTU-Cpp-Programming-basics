#pragma once

#include <string>

class BadFileFormatException : public std::exception {
	std::string error;
public:
	BadFileFormatException() {
		error.assign("Неподдерживаемый формат файла.");
	}
	explicit BadFileFormatException(const std::string& file_format) {
		error.assign("Формат \"" + file_format + "\" не поддерживается");
	}
	const char* what() const noexcept override {
		return error.c_str();
	}
};

class NoFileFoundException : public std::exception {
	std::string error;
public:
	NoFileFoundException() {
		error.assign("Не получилось открыть файл.");
	}
	explicit NoFileFoundException(const std::string& file_) {
		error.assign("Файл \"" + file_ + "\" не удалось открыть");
	}
	const char* what() const noexcept override {
		return error.c_str();
	}
};