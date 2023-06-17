#include <iostream>
#include <string>
#include <string_view>
#include <tuple>
#include <functional>
#include <memory>

template<typename Result>
class Parser {
public:
	using Tup = std::tuple<Result, std::string_view>;
	using Ret = std::optional<Tup>;
	using Fx = std::function<Ret(std::string_view input)>;
	Parser(Fx fx) : fx{fx} {}
	Ret operator()(std::string_view in) const {
		return fx(in);
	}
private:
	Fx fx;
};



auto string(std::string str) {
	using P = Parser<std::string>;
	typename P::Fx fx = [str] (std::string_view in) -> typename P::Ret {
		if(in.starts_with(str)) {
			in.remove_prefix(std::size(str));
			return P::Tup{str, in};
		}
		else {
			return std::nullopt;
		}
	};
	return P(std::move(fx));
}



auto word() {
	using P = Parser<std::string>;
	typename P::Fx fx = [] (std::string_view in) -> typename P::Ret {
		std::string str;
		if(!in.empty() && !std::isspace(in.front())) {
			str.push_back(in.front());
			in.remove_prefix(1);
			while(!in.empty() && !std::isspace(in.front())) {
				str.push_back(in.front());
				in.remove_prefix(1);
			}
			return P::Tup{{str}, in};
		}
		else {
			return std::nullopt;
		}
	};
	return P(std::move(fx));
}



auto whitespace() {
	using P = Parser<std::tuple<>>;
	typename P::Fx fx = [] (std::string_view in) -> typename P::Ret {
		if(!in.empty() && std::isspace(in.front())) {
			in.remove_prefix(1);
			while(!in.empty() && std::isspace(in.front())) {
				in.remove_prefix(1);
			}
			return P::Tup{{}, in};
		}
		else {
			return std::nullopt;
		}
	};
	return P(std::move(fx));
}



template<typename R1, typename R2>
auto chain(Parser<R1> l, Parser<R2> r) {
	using R3 = decltype(std::tuple_cat(
		std::tuple{std::declval<R1>()},
		std::tuple{std::declval<R2>()}
	)); 
	using P = Parser<R3>;
	typename P::Fx fx = [l, r] (std::string_view in) -> typename P::Ret {
		if(auto l_result = l(in)) {
			auto [l_value, l_rest] = std::move(*l_result);
			if(auto r_result = r(l_rest)) {
				auto [r_value, r_rest] = std::move(*r_result);
				return typename P::Tup{std::tuple_cat(
					std::tuple{std::move(l_value)},
					std::tuple{std::move(r_value)}
				), r_rest};
			}
		}
		return std::nullopt;
	};
	return P{std::move(fx)};
}



template<typename R1, typename R2>
auto operator >>=(Parser<R1> l, Parser<R2> r) {
	return chain(std::move(l),std::move(r));
}



template<typename R, std::size_t N>
auto either(std::array<Parser<R>, N> parsers) {
	using P = Parser<R>;
	typename P::Fx fx = [parsers] (std::string_view in) -> typename P::Ret {
		for(const auto & p : parsers) {
			if(auto r = p(in)) return r;
		}
		return std::nullopt;
	};
	return P{std::move(fx)};
}




template<typename R>
auto link(const Parser<R> * parser) {
	using P = Parser<R>;
	typename P::Fx fx = [parser] (std::string_view in) -> typename P::Ret {
		return (*parser)(in);
	};
	return P{std::move(fx)};
}



template<typename R>
auto constant(R value) {
	using P = Parser<R>;
	typename P::Fx fx = [value] (std::string_view in) -> typename P::Ret {
		return typename P::Tup{value, in};
	};
	return P{std::move(fx)};
}



template<typename R>
auto discard(Parser<R> parser) {
	using P = Parser<std::tuple<>>;
	typename P::Fx fx = [parser] (std::string_view in) -> typename P::Ret {
		if(auto r = parser(in)) {
			auto [r_val, r_rest] = std::move(*r);
			return typename P::Tup{{}, r_rest};
		}
		else {
			return std::nullopt;
		}
	};
	return P{std::move(fx)};
}



template<typename R>
auto transform(Parser<R> parser, auto fx) {
	using PIn = Parser<R>;
	using T = decltype(fx(std::declval<R>()));
	using POut = Parser<T>;
	auto fx_parse = [parser, fx] (std::string_view in) -> typename POut::Ret {
		if(auto r = parser(in)) {
			auto [r_val, r_rest] = std::move(*r);
			return typename POut::Tup{fx(std::move(r_val)), r_rest};
		}
		else {
			return std::nullopt;
		}
	};
	return POut{std::move(fx_parse)};
}



template<typename R>
auto operator |=(Parser<R> p, auto fx) {
	return transform(std::move(p), std::move(fx));
}



template<typename R>
auto optional(Parser<R> parser) {
	using P = Parser<std::optional<R>>;
	typename P::Fx fx = [parser] (std::string_view in) -> typename P::Ret {
		if(auto r = parser(in)) {
			auto [val, rest] = std::move(*r);
			return typename P::Tup{{std::move(val)}, rest};
		}
		else {
			return typename P::Tup{{std::nullopt}, in};
		}
	};
	return P{std::move(fx)};
}


struct Node {
	enum class Op { ADD, SUB, MLT, DIV };
	Node(float value) : value { value } {}
	Node(Op op, std::unique_ptr<Node> l, std::unique_ptr<Node> r)
		: op{op}
		, l { std::move(l) }
		, r { std::move(r) } {}
	Op op;
	std::unique_ptr<Node> l;
	std::unique_ptr<Node> r;
	float value;
};


void print(Node & node) {
	if(node.l && node.r) {
		std::cout << "(" << static_cast<int>(node.op) << " ";
		print(*node.l);
		std::cout << " ";
		print(*node.r);
		std::cout << ")";
	}
	else {
		std::cout << node.value;
	}
}


auto to_float = [] (const auto & str) {
	return std::stof(str);
};

auto to_leaf_node = [] (const auto & value) {
	return std::make_unique<Node>(value);
};

auto to_branch_node = [] (auto t) {
	auto [l, op, r] = std::move(t);
	return std::make_unique<Node>(op, std::move(l), std::move(r));
};

extern Parser<std::unique_ptr<Node>> node_p;
auto float_p = word() |= to_float;
auto leaf_p = float_p |= to_leaf_node;
auto op_p = either(std::array{
	discard(string("+")) >>= constant(Node::Op::ADD),  
	discard(string("-")) >>= constant(Node::Op::SUB),  
	discard(string("*")) >>= constant(Node::Op::MLT),  
	discard(string("/")) >>= constant(Node::Op::DIV),  
});
auto sep_p = discard(optional(whitespace()));
auto binary_p = link(&leaf_p) >>= sep_p >>= op_p >>= sep_p >>= link(&node_p);
auto branch_p = binary_p |= to_branch_node;
extern Parser<std::unique_ptr<Node>> node_p = either(std::array{
	leaf_p,
	branch_p
});

void math_parse() {
	auto node = node_p("4.2 + 1");
	print(*std::get<0>(*node));
}


int main(int argc, char const *argv[]) {
	math_parse();
	// auto p1 = string("abc");
	// auto p2 = string("xyz");
	// auto p3 = string("abc") >> string("xyz") >> p2;
	// auto p4 = p1 >> white_space() >> p2;
	// auto p5 = either(std::array{p1, p2});
	// auto p6 = string("abc") >> string("xyz") >> link(&p2);


	// std::cout << !!p1("abc") << "\n";
	// std::cout << !!p2("xyz") << "\n";
	// std::cout << !!p3("abcxyz") << !!p6("abcxyz") << "\n";
	// std::cout << !!p3("abcxyzxyz") << !!p6("abcxyzxyz") << "\n";

	// std::cout << !!p4("abcxyzxyz") << "\n";
	// std::cout << !!p4("abc xyzxyz") << "\n";
	// std::cout << !!p4("abc  xyzxyz") << "\n";
	// std::cout << !!p4("abc \n xyzxyz") << "\n";
	// std::cout << !!p5("abc") << "\n";
	// std::cout << !!p5("xyz") << "\n";
	// std::cout << !!p5("def") << "\n";

	// auto p_abc = string("abc");
	// auto p_xyz = string("xyz");
	// auto to_float = [] (const std::tuple<int> & t) {
	// 	return static_cast<float>(std::get<0>(t));
	// };
	// auto parser = either(std::array{
	// 	discard(link(&p_abc))
	// 	>>= discard(optional(whitespace()))
	// 	>>= discard(link(&p_abc))
	// 	>>= constant(42),
		
	// 	discard(link(&p_xyz))
	// 	>>= discard(optional(whitespace()))
	// 	>>= discard(link(&p_xyz))
	// 	>>= constant(1337)
	// }) |= to_float;

	// auto [val1, rest1] = *parser("abcabc");
	// auto [val2, rest2] = *parser("xyzxyz");
	// auto [val3, rest3] = *parser("abc abc");
	// auto [val4, rest4] = *parser("xyz xyz");
	
	// std::cout << val1 << "\n";
	// std::cout << val2 << "\n";
	// std::cout << val3 << "\n";
	// std::cout << val4 << "\n";



	// if(r1) {
	// 	auto [val1, rest1] = *r1;
	// 	auto [val1_1, val1_2, val1_3] = val1;
	// 	std::cout << val1_1 << val1_2 << "\n";
	// }
	// if(r2) {
	// 	auto [val2, rest2] = *r1;
	// 	auto [val2_1, val2_2] = val2;
	// 	std::cout << val2_1 << val2_2 << "\n";
	// }
	return 0;
}
