#include <memory>
#include <iostream>

class PolyStorage {
	struct Base {
		virtual std::unique_ptr<Base> clone() const = 0;
		virtual ~Base() = default;
		virtual const void *get_object() const = 0;
		virtual void *get_object() = 0;
	};
	
	template<typename T>
	struct Store : Base {
		template<typename U>
		Store(U && u) 
			: object{std::forward<U>(u)} {}

		virtual std::unique_ptr<Base> clone() const override {
			return std::make_unique<Store<T>>(object);
		}

		virtual const void *get_object() const override {
			return reinterpret_cast<const void*>(&object);
		}

		virtual void *get_object() override {
			return reinterpret_cast<void*>(&object);
		}
		T object;
	};
public:
	template<typename T>
	PolyStorage(T && t) 
		: object{std::make_unique<Store<T>>(std::forward<T>(t))} {}

	PolyStorage(const PolyStorage & other) {
		this->object = other.object->clone();
	}

	PolyStorage & operator=(const PolyStorage & other) {
		PolyStorage tmp = other;
		swap(*this, tmp);
		return *this;
	}

	PolyStorage(PolyStorage && other) {
		this->object = std::move(other.object);
	}

	PolyStorage & operator=(PolyStorage && other) {
		PolyStorage tmp = std::move(other);
		swap(*this, tmp);
		return *this;
	}

	friend void swap(PolyStorage & l, PolyStorage & r) {
		std::swap(l.object, r.object);
	}

	const void * get_object() const {
		return object->get_object();
	}

	void * get_object() {
		return object->get_object();
	}
private:
	std::unique_ptr<Base> object;
};


template<typename ... Concepts>
class poly final : public Concepts... {
public:
	template<typename T>
	poly(T && t) 
		: Concepts{t}...
		, storage{std::forward<T>(t)} {}
	poly(const poly & other) = default;
	poly & operator=(const poly & other) = default;
	poly(poly && other) = default;
	poly & operator=(poly && other) = default;
	~poly() = default;
protected:
	virtual const void *get_object() const {
		return storage.get_object();
	}

	virtual void *get_object() {
		return storage.get_object();
	}
private:
	PolyStorage storage;
};



class Model {
public:
protected:
	virtual void *get_object() = 0;
	virtual ~Model() = default;
};



class Foo : public virtual Model {
public:
	template<typename T>
	Foo(const T & t) {
		fx = [] (void * obj) {
			return reinterpret_cast<T*>(obj)->foo();
		};
	}

	void foo() {
		return this->fx(this->get_object());
	}
private:
	void(*fx)(void *);
};


struct A {
	void foo() {
		std::cout << "A\n";
	}
};


struct B {
	void foo() {
		std::cout << "B\n";
	}
};



int main() {
	poly<Foo> x = A{}; 
	x.foo(); 
	x = B{};
	x.foo(); 
	return 0;
}
