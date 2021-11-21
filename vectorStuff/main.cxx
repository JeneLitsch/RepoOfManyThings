#include "VectorStuff.hxx"
#include <iostream>
int main() {
	std::vector<std::vector<int>> vec1 = {{1,2,3}};
	std::vector<std::vector<int>> vec2 = {{4,5,6}};
	std::cout << vec1 + vec2 << "\n";
	std::vector<float> vec3 = {1.4,3,5,7};
	std::cout << "avg(" << vec3 << ") = " << avg(vec3.begin(), vec3.end()) << "\n";
	return 0;
}
