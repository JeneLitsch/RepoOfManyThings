#include <iostream>
#include <optional>

struct error_default {
	template<typename T>
	operator T() const {
		throw std::runtime_error{"Hello World"};
	}
};

int main() {
	std::optional<int> a = 42;
	std::cout << a.value_or(error_default{});
	std::optional<int> b = std::nullopt;
	std::cout << b.value_or(error_default{});
	return 0;
}
