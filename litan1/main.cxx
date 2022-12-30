#include "litan.hxx"

int main(int argc, char const *argv[]) {
	StackValue a = Int{42};
	StackValue b = a;
	StackValue c = String{heap.alloc<HeapString>()};
	c = String{heap.alloc<HeapString>()};
	b = Float{42};
	gc(stack, heap);
}
