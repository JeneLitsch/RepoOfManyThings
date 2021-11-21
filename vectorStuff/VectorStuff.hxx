#pragma once
#include <ostream>
#include <vector>
#include <numeric>

template<class T>
std::ostream & operator<<(std::ostream & out, const std::vector<T> & vec) {
	out << "{";
	for(std::size_t i = 0; i < vec.size(); i++) {
		out << ((i) ? (", ") : ("")) << vec[i];
	}
	out << "}";
	return out;
}

template<class T>
std::vector<T> operator+(const std::vector<T> & l, const std::vector<T> & r) {
	std::vector<T> vec;
	vec.reserve(l.size() + r.size());
	vec.insert(std::end(vec), l.begin(), l.end());
	vec.insert(std::end(vec), r.begin(), r.end());
	return vec;
}

auto avg(auto begin, auto end) {
	auto sum = std::reduce(begin, end);
	auto size = std::distance(begin, end);
	return sum / static_cast<decltype(sum)>(size);
}