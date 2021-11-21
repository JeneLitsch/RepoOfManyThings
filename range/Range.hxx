#pragma once
#include <iterator>
#include <cstddef>
#include <concepts>
template<class ValueType> requires std::integral<ValueType>
class Range {
public:
	class Iterator {
	public:
		Iterator(ValueType value) : value(value) {}
		
		using iterator_category = std::input_iterator_tag;
		using difference_type = std::ptrdiff_t;
		using value_type = ValueType;
		using pointer = value_type*;
		using reference = value_type&; 

		const value_type & operator*() const {
			return this->value;
		}
		
		const value_type * operator->() const {
			return &this->value;
		}
		
		Iterator operator++() { 
			this->value++; return *this;
		}
		
		Iterator operator++(int) {
			Iterator temp = *this;
			this->value++;
			return temp;
		}

		friend bool operator==(const Iterator & l, const Iterator & r) {
			return l.value == r.value;
		}

		friend bool operator!=(const Iterator & l, const Iterator & r) {
			return l.value != r.value;
		}
	
	private:
		value_type value;
	};

	Iterator begin() const {
		return Iterator{first};
	}

	Iterator end() const {
		return Iterator{last};
	}

	Range(ValueType first, ValueType last) : first(first), last(last) {}
private:
	ValueType first;
	ValueType last;
};