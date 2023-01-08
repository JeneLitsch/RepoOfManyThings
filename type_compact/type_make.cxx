#include "type.hxx"

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
	code.push_back(TypeCode::OTHER);
	std::uint16_t size = static_cast<std::uint16_t>(std::size(name));
	std::uint8_t size0 = (size >> 0) & 0xff;
	std::uint8_t size1 = (size >> 8) & 0xff;
	code.push_back(static_cast<TypeCode>(size0));
	code.push_back(static_cast<TypeCode>(size1));
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
	code.push_back(TypeCode::ARRAY);
	for(const auto x : elem_type.get_code()) {
		code.push_back(x);
	}
	return Type{code};
}



Type map(const Type & key_type, const Type & value_type) {
	std::vector<TypeCode> code;
	code.push_back(TypeCode::MAP);
	for(const auto x : key_type.get_code()) {
		code.push_back(x);
	}
	for(const auto x : value_type.get_code()) {
		code.push_back(x);
	}
	return Type{code};
}



Type optional(const Type & wrapped_type) {
	std::vector<TypeCode> code;
	code.push_back(TypeCode::OPTIONAL);
	for(const auto x : wrapped_type.get_code()) {
		code.push_back(x);
	}
	return Type{code};
}