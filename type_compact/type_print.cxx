#include "type.hxx"


namespace {

	
	using It = std::vector<TypeCode>::const_iterator;
	It print_impl(std::ostream & out, It type);


	It print_impl_primary(const std::string_view name, std::ostream & out, It type) {
		out << name;
		return type + 1;
	}



	It print_impl_unary(const std::string_view name, std::ostream & out, It type) {
		out << name;
		out << "<";
		const auto next = print_impl(out, type+1);
		out << ">";
		return next;
	}



	It print_impl_binary(const std::string_view name, std::ostream & out, It type) {
		out << name;
		out << "<";
		const auto next0 = print_impl(out, type+1);
		out << ", ";
		const auto next1 = print_impl(out, next0);
		out << ">";
		return next1;
	}



	It print_impl_other(std::ostream & out, It type) {
		std::uint64_t size0 = static_cast<std::uint64_t>(*++type);
		std::uint64_t size1 = static_cast<std::uint64_t>(*++type);
		std::uint8_t size = size0 + (size1 << 8);
		for(int i = 0; i < size; ++i) {
			++type;
			out << static_cast<char>(*type);
		}
		return type;
	}



	It print_impl_fx_ptr(std::ostream & out, It type) {
		++type;
		const auto arity = static_cast<std::uint8_t>(*type);

		std::ostringstream return_type;
		++type;
		type = print_impl(return_type, type);
		
		out << "(";
		for(auto i = 0; i < arity; ++i) {
			if(i) out << ", ";
			type = print_impl(out, type);
		}
		out << ")->";
		out << return_type.str();

		return type;
	}



	It print_impl(std::ostream & out, It type) {
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
			case TypeCode::FX_PTR:      return print_impl_fx_ptr(out, type);
		}
		return type;
	}
}



std::ostream & operator<<(std::ostream & out, const Type & type) {
	print_impl(out, std::begin(type.get_code()));
	return out;
}



