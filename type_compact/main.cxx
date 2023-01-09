#include "type.hxx"
#include <iostream>

int main() {
	Type t_bool = boolean(); 
	Type t_char = character(); 
	Type t_int = integer(); 
	Type t_float = floating();
	Type t_array_int = array(integer());
	Type t_array_bool = array(boolean());
	Type t_map_int_bool = map(integer(), boolean());
	Type t_other = other("TTT");
	Type t_fx_ptr = fx_ptr(integer(), {array(floating()), string()});

	std::cout << t_bool << "\n";
	std::cout << t_char << "\n";
	std::cout << t_int << "\n";
	std::cout << t_float << "\n";
	std::cout << t_array_int << "\n";
	std::cout << t_map_int_bool << "\n";
	std::cout << t_other << "\n";
	std::cout << t_fx_ptr << "\n";

	std::cout << is_any(t_bool) << "\n";
	std::cout << is_boolean(t_bool) << "\n";

	std::cout << (t_bool == boolean()) << "\n";
	std::cout << (t_array_int == t_array_bool) << "\n";

	std::cout << element_type(t_array_int).value() << "\n";
	std::cout << key_type(t_map_int_bool).value() << "\n";
	std::cout << value_type(t_map_int_bool).value() << "\n";

	std::cout << subtypes(t_bool).size() << "\n";
	std::cout << subtypes(t_array_bool).size() << "\n";
	std::cout << subtypes(t_map_int_bool).size() << "\n";
	std::cout << subtypes(t_fx_ptr).size() << "\n";
	std::cout << subtypes(t_other).size() << "\n";
	// std::cout << subtype(t_bool, 0).value() << "\n";
	return 0;
}
