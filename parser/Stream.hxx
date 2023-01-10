#pragma once
#include <string_view>
#include <optional>

class Stream {
public:
	Stream(std::string_view buffer_view) : buffer_view{buffer_view} {}
	
	
	
	std::optional<char> peek() const {
		if(buffer_view.empty()) return std::nullopt;
		else return buffer_view.front();
	}



	std::optional<char> get() {
		auto c = peek();
		if(c) buffer_view.remove_prefix(1);
		return c;
	}
private:
	std::string_view buffer_view;
};