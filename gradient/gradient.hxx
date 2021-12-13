#include <map>
#include <iostream>
#include <concepts>
#include <algorithm>
#include <cmath>
#include <numeric>

namespace stx {

	auto dec(auto it) {
		return --it;
	}

	auto size(const auto begin, const auto end) {
		return std::distance(begin, end);
	}

	bool empty(const auto begin, const auto end) {
		return stx::size(begin, end) == 0;
	}
 
	template<typename Key, typename Value>
	Value gradient(
		const typename std::map<Key, Value>::const_iterator & begin,
		const typename std::map<Key, Value>::const_iterator & end,
		const Key & t,
		const Value & defaultValue = {}) {

		if(empty(begin, end)) {
			return defaultValue;
		}

		constexpr auto comp = [](const auto t, const auto & pair) {
			return pair.first > t;
		};

		const auto pair = std::upper_bound(begin, end, t, comp);

		const auto next = (pair == end)   ? dec(pair) : pair;
		const auto prev = (pair != begin) ? dec(pair) : pair; 

		const auto [posNext, valueNext] = *next;
		const auto [posPrev, valuePrev] = *prev;

		const auto distance = posNext - posPrev;
		
		if(distance == 0) {
			return valuePrev;
		}

		const auto tLerp = std::clamp(
			static_cast<Value>((t - posPrev)/distance),
			static_cast<Value>(0.0),
			static_cast<Value>(1.0));

		return std::lerp(
			static_cast<Value>(valuePrev), 
			static_cast<Value>(valueNext),
			static_cast<Value>(tLerp));	
	}
}
