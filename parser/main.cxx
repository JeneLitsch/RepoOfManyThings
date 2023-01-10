#include "parse.hxx"
#include <sstream>

template<typename T>
void print(std::optional<T> t) {
	if(t) std::cout << *t;
	else std::cout << "<null>";
}

template<typename T>
void println(std::optional<T> t) {
	print(t);
	std::cout << "\n";
}


int main() {
	std::istringstream iss {"12345#67890"};
	const auto digits_p = one_or_more(digit());
	const auto floating_p = digits_p + conditional(
		char_p('.'), digits_p, char_p('#')
	); 
	if(auto floating = floating_p(iss)) {
		auto [whole, fraction] = *floating;
		std::cout << "Found: ";
		std::string str;
		for(const auto & c : whole) {
			str.push_back(c);
		}
		if(auto fr = std::get_if<std::vector<char>>(&fraction)) {
			str.push_back('.');
			for(const auto & c : *fr) {
				str.push_back(c);
			}
		}
		std::cout << str << "\n";
	}
	else {
		std::cout << "Not Found";
	}
	// auto single_a = char_p('a');
	// auto single_b = char_p('b');
	// auto single_c = char_p('c');
	// auto single_a_or_b = either(single_a, single_b);
	// println(single_a.read(iss));
	// println(single_a.read(iss));
	// println(single_a_or_b.read(iss));
	// println(single_b.read(iss));
	// println(single_c.read(iss));
	// auto single_digit = digit();
	// println(single_digit.read(iss));
	// println(single_digit.read(iss));
	// println(single_digit.read(iss));
	// println(single_digit.read(iss));
	return 0;
}
