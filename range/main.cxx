#include "Range.hxx"
#include <iostream>

int main() {
	for(auto i : Range{1, 5}) {
		std::cout << i << "\n";
	}
	return 0;
}
