#include <iostream>
#include <array>
#include <vector>
#include <span>
#include <memory>
#include <unistd.h>
#include <stdio.h>
#include <malloc.h>
#include <sys/mman.h>

namespace xyz {
	std::size_t page_size() {
		return sysconf(_SC_PAGE_SIZE);
	}

	std::size_t fit(std::size_t size) {
		return ((page_size() + size - 1) / size) * size;
	}

	std::ptrdiff_t distance(const void * a, const void * b) {
		return reinterpret_cast<std::intptr_t>(b) 
			- reinterpret_cast<std::intptr_t>(a);
	}

	void * align(const void * ptr) {
		const auto page = reinterpret_cast<std::uintptr_t>(ptr) / page_size();
		return (void*)(page*page_size());
	}

	void make_executable(const void * data, std::size_t size) {
		const auto addr = align(data);
		const auto offset = distance(addr, data);
		const auto len = fit(size + offset);
		int error = mprotect(addr, len, PROT_EXEC|PROT_READ|PROT_WRITE);

		if(error != 0) throw std::runtime_error {
			"Cannot mark memory as executable"
		};
	}

	void make_executable(const std::span<const char> & array) {
		return make_executable(std::data(array), std::size(array));
	}

	template<typename Ret, typename ...Args>
	Ret invoke(const auto & array, Args ... args) {
		const auto data = std::data(array);
		auto fx = reinterpret_cast<Ret(*)(Args...)>(data);
		return fx(args...);
	}
}

const char add [] =
	"\x8d\x04\x37" // lea eax, [rdi+rsi]
	"\xc3";        // ret

int main(int argc, char const *argv[]) {
	xyz::make_executable(add);
	std::cout << xyz::invoke<int, int, int>(add, 1, 2) << std::endl; // 3
	return 0;
}

