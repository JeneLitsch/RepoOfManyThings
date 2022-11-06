#include "type.hxx"
#include <iostream>

int main(int argc, char const *argv[]){
	ltn::c::Type typeA = ltn::c::Array{ltn::c::Int{}};
	ltn::c::Type typeB = ltn::c::Array{ltn::c::Int{}};

	// std::cout << (typeA.to<ltn::c::Array>().contains == ltn::c::Int{}) << "\n";

	return 0;
}
