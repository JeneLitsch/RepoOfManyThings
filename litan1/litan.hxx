#pragma once
#include <vector>
#include <string>
#include <variant>
#include <iostream>
#include <cstdint>
#include <set>
#include <queue>

struct StackValue;
struct HeapValue;


struct Bool {
	bool value;
};
struct Char {
	std::uint8_t value;
};
struct Int {
	std::int64_t value;
};
struct Float {
	double value;
};
struct String {
	std::uint64_t id;
};
struct Array {
	std::uint64_t id;
};
using ValueCore = std::variant<
	Bool,
	Char,
	Int,
	Float,
	Array,
	String
>;
struct Value {
	ValueCore core;
};




struct HeapString {
	std::string value;
};
struct HeapArray {
	std::vector<HeapValue> value;
};
using HeapObject = std::variant<std::monostate, HeapString, HeapArray>;



class Heap {
public:
	template<typename Type>
	std::uint64_t alloc() {
		const auto id = next();
		std::cout << "Alloc " << id << "\n";
		this->objects[id] = Type{};
		return id;
	}
	

	template<typename Type>
	Type & get(std::uint64_t id) {
		return std::get<Type>(objects[id]);
	}


	void free(std::uint64_t id) {
		if(!std::holds_alternative<std::monostate>(objects[id])) {
			std::cout << "Free " << id << "\n";
			this->reuseable.push(id);
			this->objects[id] = std::monostate{};
		}
	}


	std::size_t size() const {
		return this->objects.size();
	}

private:
	std::uint64_t next() {
		if(this->reuseable.empty()) {
			auto id = objects.size();
			objects.resize(id+1);
			return id;
		}
		else {
			auto id = reuseable.front();
			reuseable.pop();
			return id;
		}
	}
	std::vector<HeapObject> objects;
	std::queue<std::uint64_t> reuseable;
};



struct HeapValue : public Value {
	HeapValue(const Bool & core) :  HeapValue{ValueCore{core}} {}
	HeapValue(const Char & core) :  HeapValue{ValueCore{core}} {}
	HeapValue(const Int & core) :   HeapValue{ValueCore{core}} {}
	HeapValue(const Float & core) : HeapValue{ValueCore{core}} {}

	HeapValue(const ValueCore & core) : Value{core} {}
};



class Stack {
public:
	void add(const Value & value) {
		std::cout << "Added stack value " << &value << "\n";
		this->on_stack.insert(&value);
	}
	void remove(const Value & value) {
		std::cout << "Removed stack value " << &value << "\n";
		this->on_stack.erase(&value);
	}

	auto begin() {
		return std::begin(on_stack);
	}
	
	auto end() {
		return std::end(on_stack);
	}
private:
	std::set<const Value *> on_stack;
};



Stack stack;
Heap heap;



class StackValue : public Value {
public:
	StackValue(const Bool & core) : StackValue{ValueCore{core}} {}
	StackValue(const Char & core) : StackValue{ValueCore{core}} {}
	StackValue(const Int & core) : StackValue{ValueCore{core}} {}
	StackValue(const Float & core) : StackValue{ValueCore{core}} {}
	StackValue(const String & core) : StackValue{ValueCore{core}} {}
	StackValue(const Array & core) : StackValue{ValueCore{core}} {}

	StackValue(const ValueCore & core) : Value{core} {
		stack.add(*this);
	}

	StackValue(const StackValue & other) : Value{other.core} {
		stack.add(*this);
	}

	StackValue & operator=(const StackValue & other) {
		stack.remove(*this);
		this->core = other.core;
		stack.add(*this);
		return *this;
	}

	~StackValue() {
		stack.remove(*this);
	}
};




void mark(const Value & value, Heap & heap, std::set<std::uint64_t> & marked);

void mark_impl(const Array & array, Heap & heap, std::set<std::uint64_t> & marked) {
	if(marked.contains(array.id)) return;
	marked.insert(array.id);
	for(auto & elem : heap.get<HeapArray>(array.id).value) {
		mark(elem, heap, marked);
	}
}

void mark_impl(const String & string, Heap & heap, std::set<std::uint64_t> & marked) {
	if(marked.contains(string.id)) return;
	marked.insert(string.id);
}

void mark_impl(const auto &, Heap & heap, std::set<std::uint64_t> & marked) {

}

void mark(const Value & value, Heap & heap, std::set<std::uint64_t> & marked) {
	std::visit([&] (auto & v) { return mark_impl(v, heap, marked); }, value.core);
}

void mark(Stack stack, Heap & heap, std::set<std::uint64_t> & marked) {
	for(auto & elem : stack) {
		mark(*elem, heap, marked);
	}
}

void sweep(Heap & heap, const std::set<std::uint64_t> & marked) {
	for(std::size_t id = 0; id < heap.size(); ++id) {
		if(!marked.contains(id)) {
			heap.free(id);
		}
	}
}

void gc(Stack & stack, Heap & heap) {
	std::set<std::uint64_t> marked;
	mark(stack, heap, marked);
	sweep(heap, marked);
}