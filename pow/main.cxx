#include <concepts>
#include <iostream>
#include <cmath>

template<std::integral T>
constexpr T pow(T b, T e) {
	if(e < 0) throw std::invalid_argument{ "Negative power" };
	T res = 1;
	for(T i = sizeof(T) + 1; i > 0; --i) {
		const auto bit = (e >> (i - 1)) & 1;
		res *= res;
		res *= bit * b + !bit;
	}
	return res;
}
	

int main() {
	std::cout << pow(5, 5) << "\n";
	std::cout << pow(5, 0) << "\n";
	std::cout << pow(3, 3) << "\n";
	std::cout << pow(3, -1) << "\n";
}
