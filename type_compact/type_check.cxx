#include "type.hxx"

namespace {
	TypeCode start_code_of(const Type & type) {
		return type.get_code().front();
	}
}


bool is_any(const Type & type) {
	return start_code_of(type) == TypeCode::ANY;
}



bool is_other(const Type & type) {
	return start_code_of(type) == TypeCode::OTHER;
}



bool is_boolean(const Type & type) {
	return start_code_of(type) == TypeCode::BOOL;
}



bool is_character(const Type & type) {
	return start_code_of(type) == TypeCode::CHAR;
}



bool is_integer(const Type & type) {
	return start_code_of(type) == TypeCode::INT;
}



bool is_floating(const Type & type) {
	return start_code_of(type) == TypeCode::FLOAT;
}



bool is_string(const Type & type) {
	return start_code_of(type) == TypeCode::STRING;
}



bool is_array(const Type & type) {
	return start_code_of(type) == TypeCode::ARRAY 
		|| start_code_of(type) == TypeCode::EMPTY_ARRAY;
}



bool is_empty_array(const Type & type) {
	return start_code_of(type) == TypeCode::EMPTY_ARRAY;
}



bool is_map(const Type & type) {
	return start_code_of(type) == TypeCode::MAP;
}



bool is_optional(const Type & type) {
	return start_code_of(type) == TypeCode::OPTIONAL;
}



bool operator==(const Type & l, const Type & r) {
	return (l <=> r) == 0;
}



std::strong_ordering operator<=>(const Type & l, const Type & r) {
	return l.get_code() <=> r.get_code();
}