#include <string_view>
#include <string>
#include <variant>
#include <vector>
#include <sstream>
#include <iostream>
#include <memory>

class FormatArg {
public:
	template<typename T>
	FormatArg(const T & t) {
		this->arg_ptr = reinterpret_cast<const void *>(&t);
		this->fx_ptr = [](std::ostream & oss, const void * ptr) {
			oss << *reinterpret_cast<const T *>(ptr);
		};
	}

	friend std::ostream & operator<<(std::ostream & oss, const FormatArg & arg) {
		arg.fx_ptr(oss, arg.arg_ptr);
		return oss;
	} 

private:
	const void * arg_ptr;
	void(*fx_ptr)(std::ostream &, const void *);
};



bool match(std::string_view & formatting, const std::string_view & str) {
	if(formatting.starts_with(str)) {
		formatting.remove_prefix(std::size(str));
		return true;
	}
	return false;
}



char fetch(std::string_view & formatting) {
	auto chr = formatting.front();
	formatting.remove_prefix(1);
	return chr;
}



void format_field(std::string_view & formatting, std::ostream & oss, auto & it, auto end) {
	if(match(formatting, "}}")) throw std::runtime_error{
		"{ must be escaped as {{"
	};

	if(!match(formatting, "}")) throw std::runtime_error{
		"Expected closing } after format field"
	};

	if(it != end) oss << *it++;
	else oss << "{}";
}



std::string format(std::string_view formatting, const std::vector<FormatArg> & args) {
	
	std::ostringstream oss;
	auto it = std::begin(args); 
	auto end = std::end(args); 
	
	while(!formatting.empty()) {
		if(match(formatting, "{{")) {
			oss << "{";
		}
		else if(match(formatting, "{")) {
			format_field(formatting, oss, it, end);
		}
		else if(match(formatting, "}}")) {
			oss << "}";
		}
		else if(match(formatting, "}")) {
			throw std::runtime_error{"} must be escaped as }}"};
		}
		else {
			oss << fetch(formatting);
		}
	}
	return oss.str();
}



void run_format(std::string_view formatting) {
	std::cout << "format: " << formatting << " -> ";
	try {
		std::cout << format(formatting, {"World", "123", 3.14, 42}) << "\n";
	}
	catch(const std::runtime_error & e) {
		std::cout << "Cannot format: " << e.what() << "\n";
	}
}



int main() {
	run_format("Hello {} {{}} {} {} {}         ");
	run_format("Hello {} {{}} {} {} {} {} {} {}");
	run_format("Hello {} {{}}                  ");
	run_format("Hello {} {}} {}                ");
	run_format("Hello {} {{} {}                ");
	return 0;
}
