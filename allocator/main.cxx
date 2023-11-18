#include <string>
#include <iostream>
#include <vector>
#include <array>
#include <numeric>
#include <memory>
#include <memory_resource>
#include <chrono>

template<std::uint64_t BLOCK_SIZE, std::uint64_t BLOCK_COUNT>
class page {
public:
	page(const page &) = delete;
	page() {
		std::iota(available, available + BLOCK_COUNT, 0);
	}

	void * allocate(std::size_t size) {
		if(size <= BLOCK_SIZE) {
			if(available_count == 0) return nullptr;
			--available_count;
			const auto index = available[available_count]; 
			return buffer + index * BLOCK_SIZE;
		}
		else {
			return nullptr;
		}
	}

	bool deallocate(void * ptr, std::size_t size) {
		const char * begin = buffer;
		const char * end = begin + (BLOCK_COUNT * BLOCK_SIZE);
		if(ptr >= begin && ptr < end) {
			auto index = (reinterpret_cast<const char*>(ptr) - begin) / BLOCK_SIZE;
			available[available_count] = index;
			++available_count;
			return true;
		}
		else {
			return false;
		}
	}

private:
	char buffer [BLOCK_COUNT * BLOCK_SIZE];
	std::uint64_t available [BLOCK_COUNT];
	std::uint64_t available_count = BLOCK_COUNT;
};

template<std::uint64_t BLOCK_SIZE, std::uint64_t BLOCK_COUNT>
class pool {
public:
	void * allocate(std::size_t size) {
		if(size <= BLOCK_SIZE) {
			for(const auto & page : this->pages) {
				if(auto ptr = page->allocate(size)) {
					return ptr;
				} 
			}
			this->pages.push_back(std::make_unique<page<BLOCK_SIZE, BLOCK_COUNT>>());
			return pages.back()->allocate(size);
		}
		return nullptr;
	}

	bool deallocate(void * ptr, std::size_t size) {
		if(size <= BLOCK_SIZE) {
			for(const auto & page : this->pages) {
				if(page->deallocate(ptr, size)) return true;
			}
			return false;
		}
		else {
			return false;
		}
	}
private:
	std::vector<std::unique_ptr<page<BLOCK_SIZE, BLOCK_COUNT>>> pages;
};

template<typename T>
class test_allocator {
public:
	using value_type = T;
	
	test_allocator() = default;
	test_allocator(const test_allocator&) = delete;

	T * allocate(std::size_t size) {
		// std::cout << "Allocate: " << (size * sizeof(T)) << "bytes\n";
		// if(auto ptr = page_4.allocate(size))   return reinterpret_cast<T*>(ptr);
		// if(auto ptr = page_8.allocate(size))   return reinterpret_cast<T*>(ptr);
		// if(auto ptr = page_16.allocate(size))  return reinterpret_cast<T*>(ptr);
		// if(auto ptr = page_32.allocate(size))  return reinterpret_cast<T*>(ptr);
		// if(auto ptr = page_64.allocate(size))  return reinterpret_cast<T*>(ptr);
		// if(auto ptr = page_128.allocate(size)) return reinterpret_cast<T*>(ptr);
		return new T[size];
	}

	void deallocate(T * ptr, std::size_t size) {
		// std::cout << "Deallocate: " << (size * sizeof(T)) << "bytes\n";
		// if(page_4.deallocate(ptr, size)) return;
		// if(page_8.deallocate(ptr, size)) return;
		// if(page_16.deallocate(ptr, size)) return;
		// if(page_32.deallocate(ptr, size)) return;
		// if(page_64.deallocate(ptr, size)) return;
		// if(page_128.deallocate(ptr, size)) return;
		delete[] ptr;
	}

	pool<4, 256>   page_4;
	pool<8, 256>   page_8;
	pool<16, 256>  page_16;
	pool<32, 256>  page_32;
	pool<64, 256>  page_64;
	pool<128, 256> page_128;
};


using string = std::basic_string<char, std::char_traits<char>, test_allocator<char>>;


template<typename Allocator>
void run() {
	std::chrono::high_resolution_clock clock{};
	auto t1 = clock.now();
	// string str = "AAAAAAAAAAA";
	for(std::size_t i = 0; i < 100000; ++i) {
		std::vector<int, Allocator> arr{};
		for(std::size_t n = 0; n < (i % 16); ++n) {
			arr.push_back(1);
		}
	}
	auto t2 = clock.now();
	std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(t2-t1).count() << "ms\n"; 
}


template<typename Allocator>
void run_pmr() {
	Allocator allocator;
	std::chrono::high_resolution_clock clock{};
	auto t1 = clock.now();
	// string str = "AAAAAAAAAAA";
	for(std::size_t i = 0; i < 100000; ++i) {
		std::pmr::vector<int> arr{&allocator};
		for(std::size_t n = 0; n < (i % 16); ++n) {
			arr.push_back(1);
		}
	}
	auto t2 = clock.now();
	std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(t2-t1).count() << "ms\n"; 
}


int main() {
	run<std::allocator<int>>();
	run<test_allocator<int>>();
	run_pmr<std::pmr::unsynchronized_pool_resource>();
	run_pmr<std::pmr::synchronized_pool_resource>();
	run_pmr<std::pmr::monotonic_buffer_resource>();
	return 0;
}
