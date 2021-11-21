#include "IIFE.hxx"
#include <iostream>
int main() {
	std::cout << iife([] {
		return "Hello World";
	}) << "\n";
	return 0;
}
