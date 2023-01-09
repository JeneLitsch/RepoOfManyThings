#include "type.hxx"
#include <tuple>

namespace {
	using It = std::vector<TypeCode>::const_iterator;
	std::uint64_t other_head_size(It begin) {
		std::uint64_t size0 = static_cast<std::uint64_t>(*++begin);
		std::uint64_t size1 = static_cast<std::uint64_t>(*++begin);
		return 1 + 2 + (size0 + (size1 << 8));
	}



	std::uint64_t head_size(const Type & type) {
		if(is_other(type)) {
			return other_head_size(std::begin(type.get_code()));
		}
		if(is_fx_ptr(type)) {
			return 2;
		}
		return 1;
	}


	
	const std::optional<It> end_of_type(const std::optional<It> & code, const It & end);



	const std::optional<It> end_of_fx_ptr(const std::optional<It> & code, const It & end) {
		auto it = code;
		auto size = static_cast<std::uint8_t>(*++*it);
		++*it;
		for(int i = 0; i < size; ++i) {
			it = end_of_type(it,end);
		}
		return it;
	}



	using It = std::vector<TypeCode>::const_iterator;
	const std::optional<It> end_of_type(const std::optional<It> & code, const It & end) {
		
		if(!code) return code;
		if(*code >= end) return std::nullopt;

		switch (**code) {
		case TypeCode::INVALID:     return *code + 1;
		case TypeCode::OTHER:       return end_of_type(*code + other_head_size(*code), end);
		case TypeCode::ANY:         return *code + 1;
		case TypeCode::BOOL:        return *code + 1;
		case TypeCode::CHAR:        return *code + 1;
		case TypeCode::INT:         return *code + 1;
		case TypeCode::FLOAT:       return *code + 1;
		case TypeCode::STRING:      return *code + 1;
		case TypeCode::ARRAY:       return end_of_type(*code + 1, end);
		case TypeCode::EMPTY_ARRAY: return *code + 1;
		case TypeCode::MAP:         return end_of_type(end_of_type(*code + 1, end), end);
		case TypeCode::OPTIONAL:    return end_of_type(*code + 1, end);
		case TypeCode::FX_PTR:      return end_of_fx_ptr(*code + 1, end);
		}
		return *code + 1;
	}



	auto range(const Type & type) {
		const auto & code = type.get_code();
		auto begin = std::begin(code); 
		auto end = std::end(code);
		return std::make_tuple(begin, end);
	}



	auto subrange_0(const Type & type) {
		auto [begin, end] = range(type);
		auto sub_begin = std::optional{begin};
		std::advance(*sub_begin, head_size(type));
		auto sub_end = end_of_type(sub_begin, end);
		return std::make_tuple(sub_begin, sub_end);
	}



	auto next_subrange(auto sub_end, auto full_end) {
		auto new_sub_begin = sub_end;
		auto new_sub_end = end_of_type(new_sub_begin, full_end);
		return std::make_tuple(new_sub_begin, new_sub_end);
	}
}




std::vector<Type> subtypes(const Type & type) {
	std::vector<Type> types;
	auto [begin, end] = range(type);
	auto [sub_begin, sub_end] = subrange_0(type);
	for(std::uint64_t i = 0; true; ++i) {
		if(!sub_begin) return types;
		if(!sub_end) return types;
		types.push_back(Type{{*sub_begin, *sub_end}});
		std::tie(sub_begin, sub_end) = next_subrange(sub_end, end);
	}
}



std::optional<Type> element_type(const Type & type) {
	const auto compatible = is_array(type) || is_optional(type);
	if(!compatible) return std::nullopt;
	const auto types = subtypes(type);
	if(types.size() <= 0) return std::nullopt;
	return types[0];
}



std::optional<Type> key_type(const Type & type) {
	const auto compatible = is_map(type);
	if(!compatible) return std::nullopt;
	const auto types = subtypes(type);
	if(types.size() <= 0) return std::nullopt;
	return types[0];
}



std::optional<Type> value_type(const Type & type) {
	const auto compatible = is_map(type);
	if(!compatible) return std::nullopt;
	const auto types = subtypes(type);
	if(types.size() <= 1) return std::nullopt;
	return types[1];
}
