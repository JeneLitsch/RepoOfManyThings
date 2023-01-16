#include <memory>
#include <iostream>

class Visitor {
	struct Base {
		virtual void operator()(int i) const = 0;
		virtual void operator()(float f) const = 0;
		virtual ~Base() = default;
	};

	template<typename Callable>
	struct Impl : Base {
		Impl(const Callable & callable) : callable{callable} {}
		Callable callable;

		virtual void operator()(int i) const override {
			callable(i);
		};
		virtual void operator()(float f) const override {
			callable(f);
		};
	};
public:
	template<typename Callable>
	Visitor(const Callable & callable)
		: object { std::make_unique<Impl<Callable>>(callable) } {} 

	template<typename T>
	void operator()(const T & t) const {
		object->operator()(t);
	}
private:
	std::unique_ptr<Base> object;
};



class Erased {
	struct Base {
		virtual void visit(const Visitor & visitor) const = 0;
		virtual ~Base() = default;
	};

	template<typename T>
	struct Impl : Base {
		Impl(const T & t) : t{t} {}
		virtual void visit(const Visitor & visitor) const override {
			visitor(t);
		}
		T t;
	};
public:
	template<typename T>
	Erased(const T & t)
		: object { std::make_unique<Impl<T>>(t) } {} 
	
	auto visit(const Visitor & visitor) {
		object->visit(visitor);
	};
private:
	std::unique_ptr<Base> object;
};



int main() {
	Erased a = 4; 
	Erased b = 5.f;
	Visitor print = [](const auto & value) {
		std::cout << value << "\n";
	};
	a.visit(print);
	b.visit(print);

	return 0;
}
