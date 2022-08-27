#pragma once
#include <variant>
#include "stdxx/heaped.hxx"

namespace ltn::c {
	class Type;

	struct Error {
		friend bool operator==(const Error &, const Error &) = default; 
	};
	
	struct Dynamic {
		friend bool operator==(const Dynamic &, const Dynamic &) = default; 
	};
	
	struct Null {
		friend bool operator==(const Null &, const Null &) = default; 
	};
	
	struct Bool {
		friend bool operator==(const Bool &, const Bool &) = default; 
	};
	
	struct Char {
		friend bool operator==(const Char &, const Char &) = default; 
	};
	
	struct Int {
		friend bool operator==(const Int &, const Int &) = default; 
	};

	struct Float {
		friend bool operator==(const Float &, const Float &) = default; 
	};
	
	struct String {
		friend bool operator==(const String &, const String &) = default; 
	};
	
	struct Array {
		Array(Type type);
		friend bool operator==(const Array & l, const Array & r);
		stx::heaped<Type> contains;
	};

	class Type {
	public:
		Type(const auto & x) : core {x} {}

		friend bool operator==(const Type & l, const Type & r) {
			return l.core == r.core;
		}


		
		friend bool operator==(const auto & l, const Type & r) {
			return Type{l} == r;
		}


		
		friend bool operator==(const Type & l, const auto & r) {
			return l == Type{r};
		}



		template<typename T>
		bool is() const {
			return std::get_if<T>(&this->core);
		}


		
		template<typename T>
		const T & to() const {
			return std::get<T>(this->core);
		}

		template<typename T>
		T & to() {
			return std::get<T>(this->core);
		}



	private:
		std::variant<
			Error,
			Dynamic,
			Bool,
			Char,
			Int,
			Float,
			String,
			Array
		> core;
	};	

	Array::Array(Type type) : contains{type} {}

	bool operator==(const Array & l, const Array & r) {
		return *l.contains == *r.contains;
	}
}