#pragma once
#include <set>
#include <functional>
#include <iostream>
#include <optional>
#include <tuple>
#include <variant>

template<typename Result>
class Parser {
public:
	std::optional<Result> operator()(std::istream & in) const {
		return this->starts_with(in.peek()) ? reader(in) : std::nullopt;
	}

	Parser(
		const std::set<char> & start,
		const std::function<std::optional<Result>(std::istream &)> & reader)
		: start{start}
		, reader{reader} {}

	const std::set<char> & get_start() const {
		return start;
	}

	bool starts_with(char c) const {
		return start.contains(c);
	}

	
private:
	std::set<char> start;
	std::function<std::optional<Result>(std::istream &)> reader;
};



template<typename T>
inline std::set<T> merge_sets(const std::set<T> & l, const std::set<T> & r) {
	std::set<char> merged;
	std::set_union(
		l.begin(), l.end(),
		r.begin(), r.end(),
		std::inserter(merged, merged.begin())
	);
	return merged;
}



template<typename Result>
inline Parser<Result> operator|(
	const Parser<Result> & l,
	const Parser<Result> & r) {

	return Parser<Result>{
		merge_sets(l.get_start(), r.get_start()),
		[l, r] (std::istream & in) {
			return l.starts_with(in.peek()) ? l(in) : r(in);
		}
	};
}



template<typename Result1, typename Result2>
inline auto operator+(const Parser<Result1> & l, const Parser<Result2> & r) {
	using Tuple = decltype(std::tuple_cat(
		std::tuple{*l(std::declval<std::istream&>())},
		std::tuple{*r(std::declval<std::istream&>())}
	));
	return Parser<Tuple>{
		merge_sets(l.get_start(), r.get_start()),
		[l, r] (std::istream & in) -> std::optional<Tuple> {
			auto a = l(in);
			if(!a) return std::nullopt;
			auto b = r(in);
			if(!b) return std::nullopt;
			return std::tuple_cat(std::tuple{*a}, std::tuple{*b});
		}
	};
}




template<typename Result>
inline Parser<std::vector<Result>> one_or_more(const Parser<Result> & parser) {
	return Parser<std::vector<Result>>{
		parser.get_start(),
		[parser] (std::istream & in) -> std::optional<std::vector<Result>> {
			std::vector<Result> result;
			while(parser.starts_with(in.peek())) {
				auto r = parser(in);
				if(!r) return std::nullopt;
				result.push_back(*r);
			} 
			return result;
		}
	};
}



template<typename ResultCondition, typename ResultIf, typename ResultElse>
inline auto conditional(
	const Parser<ResultCondition> & condition,
	const Parser<ResultIf> & if_branch,
	const Parser<ResultElse> & else_branch) {

	return Parser<std::variant<ResultIf, ResultElse>> {
		merge_sets(condition.get_start(), else_branch.get_start()),
		[condition, if_branch, else_branch] (std::istream & in)
		-> std::optional<std::variant<ResultIf, ResultElse>> {
			if(condition.starts_with(in.peek())) {
				auto c = condition(in);
				if(!c) return std::nullopt;
				auto value = if_branch(in);
				if(!value) return std::nullopt;
				return *value;
			}
			else {
				auto value = else_branch(in);
				if(!value) return std::nullopt;
				return *value;
			}
		}
	};
}


template<typename ResultCondition, typename Result>
inline auto optional(
	const Parser<ResultCondition> & condition,
	const Parser<Result> & if_branch) {

	return Parser<std::optional<Result>> {
		condition.get_start(),
		[condition, if_branch] (std::istream & in)
		-> std::optional<std::optional<Result>> {
			if(condition.starts_with(in.peek())) {
				auto c = condition(in);
				if(!c) return std::nullopt;
				auto value = if_branch(in);
				if(!value) return std::nullopt;
				return *value;
			}
			else return std::optional<std::optional<Result>>();
		}
	};
}


inline Parser<char> char_p(char c) {
	return Parser<char>{{c}, [] (std::istream &in) { return in.get(); }};
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