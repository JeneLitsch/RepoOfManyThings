#include "type.hxx"
#include <array>

namespace {
	void append_code(std::vector<TypeCode> & code, const TypeCode & additional) {
		code.push_back(additional);
	}
	
	
	
	void append_code(std::vector<TypeCode> & code, const std::vector<TypeCode> & additional) {
		for(const auto x : additional) {
			append_code(code, x);
		} 
	}


	
	void append_code(std::vector<TypeCode> & code, const Type & additional) {
		return append_code(code, additional.get_code());
	}


	std::array<std::uint8_t, 2> as_2_uint8(std::uint16_t input) {
		return { 
			static_cast<std::uint8_t>((input >> 0) & 0xff),
			static_cast<std::uint8_t>((input >> 8) & 0xff),
		};
	}
}



Type any() {
	return Type{{TypeCode::ANY}};
} 



Type other(std::string_view name) {
	static constexpr auto MAX = std::numeric_limits<std::uint16_t>::max();
	if(name.size() >= MAX) {
		std::ostringstream oss;
		oss << "Typename exceeds " << MAX << " characters";
		throw std::runtime_error { oss.str() };
	}
	std::vector<TypeCode> code;
	append_code(code, TypeCode::OTHER);
	const auto size = static_cast<std::uint16_t>(std::size(name));
	const auto [size0, size1] = as_2_uint8(size);
	append_code(code, static_cast<TypeCode>(size0));
	append_code(code, static_cast<TypeCode>(size1));
	for(const auto chr : name) {
		code.push_back(static_cast<TypeCode>(chr));
	}
	return Type{code};
} 



Type boolean() {
	return Type{{TypeCode::BOOL}};
}



Type character() {
	return Type{{TypeCode::CHAR}};
}



Type integer() {
	return Type{{TypeCode::INT}};
}



Type floating() {
	return Type{{TypeCode::FLOAT}};
}



Type string() {
	return Type{{TypeCode::STRING}};
}



Type array() {
	return Type{{TypeCode::EMPTY_ARRAY}};
}



Type array(const Type & elem_type) {
	std::vector<TypeCode> code;
	append_code(code, TypeCode::ARRAY);
	append_code(code, elem_type);
	return Type{code};
}



Type map(const Type & key_type, const Type & value_type) {
	std::vector<TypeCode> code;
	append_code(code, TypeCode::MAP);
	append_code(code, key_type);
	append_code(code, value_type);
	return Type{code};
}



Type optional(const Type & wrapped_type) {
	std::vector<TypeCode> code;
	append_code(code, TypeCode::OPTIONAL);
	append_code(code, wrapped_type);
	return Type{code};
}



Type fx_ptr(const Type & return_type, const std::vector<Type> & parameters) {
	std::vector<TypeCode> code;
	append_code(code, TypeCode::FX_PTR);
	const auto arity = static_cast<std::uint8_t>(std::size(parameters));
	append_code(code, static_cast<TypeCode>(arity));
	append_code(code, return_type);
	for(const auto & parameter : parameters) {
		append_code(code, parameter);
	}
	return Type{code};
}