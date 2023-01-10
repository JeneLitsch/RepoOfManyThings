#include "parse.hxx"
#include <sstream>

template<typename T>
void print(std::optional<T> t) {
	if(t) {
		auto [t_value, t_stream] = * t;
		std::cout << std::string{t_value.begin(), t_value.end()};
	} 
	else std::cout << "<null>";
}

template<typename T>
void println(std::optional<T> t) {
	print(t);
	std::cout << "\n";
}


void parse_number(const auto & parser, auto str) {
	Stream iss {str};
	if(auto floating = parser(iss)) {
		auto [result, rest] = *floating;
		auto [whole, fraction] = result;
		std::cout << "Found: ";
		std::string str;
		for(const auto & c : whole) {
			str.push_back(c);
		}
		if(!fraction.empty()) {
			str.push_back('.');
			for(const auto & c : fraction) {
				str.push_back(c);
			}
		}
		std::cout << str << "\n";
	}
	else {
		std::cout << "Not Found\n";
	}
}


int main() {
	const auto digits_p = one_or_more(digit());
	const auto floating_p = digits_p + conditional(
		char_p('.'), digits_p, constant<std::vector<char>>({})
	);
	parse_number(floating_p, "12345.67890");
	parse_number(floating_p, "42");
	parse_number(floating_p, "abc");


	println(one_or_more(alpha_numeric())(Stream{"abc123_xyz"}));

	// Stream iss {"abc1234"};
	// auto single_a = char_p('a');
	// auto single_b = char_p('b');
	// auto single_c = char_p('c');
	// auto single_a_or_b = single_a | single_b;
	// println(single_a(iss));
	// println(single_a(iss));
	// println(single_a_or_b(iss));
	// println(single_a_or_b(iss));
	// println(single_a(iss));
	// println(single_a(iss));
	// println(single_b(iss));
	// println(single_c(iss));
	// auto single_digit = digit();
	// println(single_digit(iss));
	// println(single_digit(iss));
	// println(single_digit(iss));
	// println(single_digit(iss));
	return 0;
}
