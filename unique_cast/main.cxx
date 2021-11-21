#include "unique_cast.hxx"
#include <iostream>
struct Base {
	virtual ~Base() {
		std::cout << "~Base\n";
	}
};

struct A : public Base {
	virtual ~A() {
		std::cout << "~A\n";
	}
};

struct B : public Base {
	virtual ~B() {
		std::cout << "~B\n";
	}
};

void test() {
	std::cout << "Test unique_cast\n";
	std::unique_ptr<Base> ptr = std::make_unique<A>();
	if(auto b = unique_cast<B>(std::move(ptr))) {
		std::cout << "Is of type B!\n";
	}
	if(auto a = unique_cast<A>(std::move(ptr))) {
		std::cout << "Is of type A!\n";
	}
	std::cout << "END\n\n";
}

void testIf() {
	std::cout << "Test unique_cast_if\n";
	std::unique_ptr<Base> ptr = std::make_unique<A>();
	if(auto b = unique_cast_if<B>(ptr)) {
		std::cout << "Is of type B!\n";
	}
	if(auto a = unique_cast_if<A>(ptr)) {
		std::cout << "Is of type A!\n";
	}
	std::cout << "END\n\n";
}

int main() {
	test();
	testIf();
	return 0;
}

