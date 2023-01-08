#pragma once
#include <vector>
#include <string>
#include <sstream>
#include <optional>
#include "TypeCode.hxx"


class Type final {
public:
	Type(std::vector<TypeCode> code) : code {code} {}
	const std::vector<TypeCode> & get_code() const {
		return code;
	}
private:
	std::vector<TypeCode> code;
};

Type any();
Type other(std::string_view name);
Type boolean();
Type character();
Type integer();
Type floating();
Type string();
Type array();
Type array(const Type & elem_type);
Type map(const Type & key_type, const Type & value_type);
Type optional(const Type & wrapped_type);

bool is_any(const Type & type);
bool is_other(const Type & type);
bool is_boolean(const Type & type);
bool is_character(const Type & type);
bool is_integer(const Type & type);
bool is_floating(const Type & type);
bool is_string(const Type & type);
bool is_array(const Type & type);
bool is_empty_array(const Type & type);
bool is_map(const Type & type);
bool is_optional(const Type & type);

std::vector<Type> subtypes(const Type & type);
std::optional<Type> element_type(const Type & type);
std::optional<Type> key_type(const Type & type);
std::optional<Type> value_type(const Type & type);

bool operator==(const Type & l, const Type & r);
std::strong_ordering operator<=>(const Type & l, const Type & r);

std::ostream & operator<<(std::ostream & out, const Type & type);
std::string to_string(const Type & type);