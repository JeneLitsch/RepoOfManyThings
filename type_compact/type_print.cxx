#include "type.hxx"


namespace {
	const TypeCode * print_impl(std::ostream & out, const TypeCode * type);


	const TypeCode * print_impl_primary(const std::string_view name, std::ostream & out, const TypeCode * type) {
		out << name;
		return type + 1;
	}



	const TypeCode * print_impl_unary(const std::string_view name, std::ostream & out, const TypeCode * type) {
		out << name;
		out << "<";
		const auto next = print_impl(out, type+1);
		out << ">";
		return next;
	}



	const TypeCode * print_impl_binary(const std::string_view name, std::ostream & out, const TypeCode * type) {
		out << name;
		out << "<";
		const auto next0 = print_impl(out, type+1);
		out << ", ";
		const auto next1 = print_impl(out, next0);
		out << ">";
		return next1;
	}



	const TypeCode * print_impl_other(std::ostream & out, const TypeCode * type) {
		std::uint16_t size0 = static_cast<std::uint16_t>(*(++type));
		std::uint16_t size1 = static_cast<std::uint16_t>(*(++type));
		std::uint16_t size = size0 + (size1 << 8);
		for(std::uint16_t i = 0; i < size; ++i) {
			out << static_cast<char>(*++type);
		}
		return type;
	}



	const TypeCode * print_impl(std::ostream & out, const TypeCode * type) {
		switch (*type) {
			case TypeCode::INVALID:     return print_impl_primary("<invalid>", out, type);
			case TypeCode::OTHER:       return print_impl_other(out, type);
			case TypeCode::ANY:         return print_impl_primary("any", out, type);
			case TypeCode::BOOL:        return print_impl_primary("bool", out, type);
			case TypeCode::CHAR:        return print_impl_primary("char", out, type);
			case TypeCode::INT:         return print_impl_primary("int", out, type);
			case TypeCode::FLOAT:       return print_impl_primary("float", out, type);
			case TypeCode::STRING:      return print_impl_primary("string", out, type);
			case TypeCode::ARRAY:       return print_impl_unary("array", out, type);
			case TypeCode::EMPTY_ARRAY: return print_impl_primary("array<>", out, type);
			case TypeCode::MAP:         return print_impl_binary("map", out, type);
			case TypeCode::OPTIONAL:    return print_impl_unary("optional", out, type);
		}
		return type;
	}
}



std::ostream & operator<<(std::ostream & out, const Type & type) {
	print_impl(out, type.get_code().data());
	return out;
}



std::string to_string(const Type & type) {
	std::ostringstream oss;
	oss << type;
	return oss.str();
}