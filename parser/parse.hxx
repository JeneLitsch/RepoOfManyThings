#pragma once
#include <set>
#include <functional>
#include <iostream>
#include <optional>
#include <tuple>
#include <variant>
#include "Stream.hxx"

template<typename Result>
class Parser {
public:
	using return_type = std::optional<std::pair<Result, Stream>>;
	
	return_type operator()(Stream in) const {
		return reader(in);
	}

	Parser(
		const std::function<return_type(Stream)> & reader)
		: reader{reader} {}

private:
	std::function<return_type(Stream)> reader;
};



template<typename Result>
inline auto operator|(
	const Parser<Result> & l,
	const Parser<Result> & r) {

	using Ret = typename Parser<Result>::return_type;
	return Parser<Result>{
		[l, r] (Stream in) -> Ret {
			if(auto l_result = l(in)) return l_result;
			if(auto r_result = r(in)) return r_result;
			return std::nullopt;
		}
	};
}



template<typename Result1, typename Result2>
inline auto operator+(const Parser<Result1> & l, const Parser<Result2> & r) {
	using Tuple = decltype(std::tuple_cat(
		std::tuple{std::declval<Result1>()},
		std::tuple{std::declval<Result2>()}
	));
	using Ret = typename Parser<Tuple>::return_type;
	return Parser<Tuple>{
		[l, r] (Stream in) -> Ret {
			auto a = l(in);
			if(!a) return std::nullopt;
			const auto & [a_val, a_in] = *a;
			auto b = r(a_in);
			if(!b) return std::nullopt;
			const auto & [b_val, b_in] = *b;
			return std::make_pair(
				std::tuple_cat(std::tuple{a_val}, std::tuple{b_val}),
				b_in
			);
		}
	};
}




template<typename Result>
inline auto zero_or_more(const Parser<Result> & parser) {
	using Ret = typename Parser<std::vector<Result>>::return_type;
	return Parser<std::vector<Result>>{
		[parser] (Stream in) -> Ret {
			std::vector<Result> result;
			while(auto r = parser(in)) {
				const auto & [r_value, r_stream] = *r;
				in = r_stream;
				result.push_back(r_value);
			} 
			return std::make_pair(result, in);
		}
	};
}



template<typename Result>
inline auto one_or_more(const Parser<Result> & parser) {
	using Ret = typename Parser<std::vector<Result>>::return_type;
	return Parser<std::vector<Result>>{
		[parser] (Stream in) -> Ret {
			auto result1 = parser(in);
			if(!result1) return std::nullopt;
			auto & [first, new_in1] = *result1;
			auto result2 = zero_or_more(parser)(new_in1);
			if(!result2) return std::make_pair(std::vector{first}, new_in1);
			auto & [other, new_in2] = *result2;
			other.insert(other.begin(), first);
			return std::make_pair(other, new_in2);
		}
	};
}



template<typename Result>
inline auto  constant(const Result & value) {
	using Ret = typename Parser<Result>::return_type;
	return Parser<Result>{[value] (Stream in) -> Ret {
		return std::make_pair(value, in);
	}};
}



template<typename Result>
inline auto conditional_impl(
	const auto & condition,
	const auto & if_branch,
	const auto & else_branch) {

	using Ret = typename Parser<Result>::return_type;
	return Parser<Result> {
		[condition, if_branch, else_branch] (Stream in) -> Ret {
			if(auto result = condition(in)) {
				auto [c, in_c] = *result;
				return if_branch(in_c);
			}
			else {
				return else_branch(in);
			}
			return std::nullopt;
		}
	};
}


template<typename ResultCondition, typename ResultIf, typename ResultElse>
inline auto conditional(
	const Parser<ResultCondition> & condition,
	const Parser<ResultIf> & if_branch,
	const Parser<ResultElse> & else_branch) {

	using Variant = std::variant<ResultIf, ResultElse>;
	return conditional_impl<Variant>(condition, if_branch, else_branch);
}



template<typename ResultCondition, typename Result>
inline auto conditional(
	const Parser<ResultCondition> & condition,
	const Parser<Result> & if_branch,
	const Parser<Result> & else_branch) {

	using Ret = typename Parser<Result>::return_type;
	return conditional_impl<Result>(condition, if_branch, else_branch);
}



inline Parser<char> char_p(char c) {
	using Ret = Parser<char>::return_type;
	return Parser<char>{[c] (Stream in) -> Ret { 
		return (in.peek() == c) ? std::make_pair(*in.get(), in) : Ret{};  
	}};
}



inline Parser<char> digit() {
	return char_p('0')
		| char_p('1')
		| char_p('2')
		| char_p('3')
		| char_p('4')
		| char_p('5')
		| char_p('6')
		| char_p('7')
		| char_p('8')
		| char_p('9');
}



inline Parser<char> lower_case() {
		return char_p('a')
		| char_p('b')
		| char_p('c')
		| char_p('d')
		| char_p('e')
		| char_p('f')
		| char_p('g')
		| char_p('h')
		| char_p('i')
		| char_p('j')
		| char_p('k')
		| char_p('l')
		| char_p('m')
		| char_p('n')
		| char_p('o')
		| char_p('p')
		| char_p('q')
		| char_p('r')
		| char_p('s')
		| char_p('t')
		| char_p('v')
		| char_p('w')
		| char_p('x')
		| char_p('y')
		| char_p('z');
}



inline Parser<char> upper_case() {
		return char_p('A')
		| char_p('B')
		| char_p('C')
		| char_p('D')
		| char_p('E')
		| char_p('F')
		| char_p('G')
		| char_p('H')
		| char_p('I')
		| char_p('J')
		| char_p('K')
		| char_p('L')
		| char_p('M')
		| char_p('N')
		| char_p('O')
		| char_p('P')
		| char_p('Q')
		| char_p('R')
		| char_p('S')
		| char_p('T')
		| char_p('V')
		| char_p('W')
		| char_p('X')
		| char_p('Y')
		| char_p('Z');
}



inline Parser<char> alpha() {
	return lower_case() | upper_case();
}



inline Parser<char> alpha_numeric() {
	return alpha() | digit();
}
