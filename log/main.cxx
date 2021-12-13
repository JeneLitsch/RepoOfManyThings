#include "log.hxx"


int main() {
	debug::log.registerOutput(std::cout);
	debug::log << debug::fatal << "Hello World";
	debug::log << debug::error << "Hello World";
	debug::log << debug::warning << "Hello World";
	debug::log << debug::info << "Hello World";
}