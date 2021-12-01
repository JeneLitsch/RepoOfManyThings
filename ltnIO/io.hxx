#pragma once
#include <istream>
#include <ostream>
#include <iostream>
#include <fstream>

namespace ltn {
	inline std::ofstream fout(const std::string_view path) {
		return std::ofstream(path.data());
	}

	inline void print(std::ostream & stream, auto data) {
		stream << data;
	}

	inline void print(auto data) {
		print(std::cout, data);
	}

	inline void println(std::ostream & stream, auto data) {
		stream << data << "\n";
	}

	inline void println(auto data) {
		println(std::cout, data);
	}


	inline std::ifstream fin(const std::string_view path) {
		return std::ifstream(path.data());
	}

	inline std::string read(std::istream & stream) {
		std::string str;
		stream >> str;
		return str;
	}
	
	inline std::string read() {
		return read(std::cin);
	}
	
	inline std::string readln(std::istream & stream) {
		std::string str;
		std::getline(stream, str);
		return str;
	}
	
	inline std::string readln() {
		return readln(std::cin);
	}

	inline std::int64_t readint(std::istream & stream) {
		std::int64_t value;
		stream >> value;
		return value;
	}

	inline std::int64_t readint() {
		return readint(std::cin);
	}

	inline double readfloat(std::istream & stream) {
		double value;
		stream >> value;
		return value;
	}

	inline double readfloat() {
		return readfloat(std::cin);
	}
	
	inline bool isEof(std::istream & stream) {
		return stream.eof();
	}

	inline bool isEof() {
		return isEof(std::cin);
	}
}