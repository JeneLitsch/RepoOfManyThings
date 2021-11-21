#pragma once
#include <memory>

// from is nulled in all cases
template<class To, class From>
std::unique_ptr<To> unique_cast(std::unique_ptr<From> from) {
	if(auto ptr_to = dynamic_cast<To*>(from.get())) {
		from.release();
		return std::unique_ptr<To>(ptr_to);
	}
	return nullptr;
}

// from is only nulled if successful
template<class To, class From>
std::unique_ptr<To> unique_cast_if(std::unique_ptr<From> & from) {
	if(auto ptr_to = dynamic_cast<To*>(from.get())) {
		from.release();
		return std::unique_ptr<To>(ptr_to);
	}
	return nullptr;
}