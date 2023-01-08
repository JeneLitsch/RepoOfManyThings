#pragma once
#include <cstdint>

enum class TypeCode : std::uint8_t {
	INVALID = 0,
	OTHER,
	
	ANY,
	
	BOOL,
	CHAR,
	INT,
	FLOAT,

	STRING,
	ARRAY,
	EMPTY_ARRAY,
	MAP,
	OPTIONAL,
	FX_PTR,
};