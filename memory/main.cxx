#include <cstdint>
#include <memory>
#include <array>
#include <vector>
#include <iostream>
#include <queue>


template<typename T, std::size_t CHUNK_SIZE>
class MemoryPool {
public:
	MemoryPool() = default;
	MemoryPool(const MemoryPool &) = delete;
	MemoryPool & operator=(const MemoryPool &) = delete;

	using Chunk = std::array<T, CHUNK_SIZE>;

	struct Deleter {
		std::size_t index;
		MemoryPool * pool;
		void operator()(T *) {
			pool->destroy(index);
		}
	};

	std::unique_ptr<T, Deleter> make_unique(auto && ... args) {
		const auto id = alloc();
		const auto at_chunk = id / CHUNK_SIZE;
		if(at_chunk >= this->chunks.size()) {
			this->chunks.push_back(std::make_unique<Chunk>());
		}
		T * ptr = this->get(id);
		ptr->~T();
		T * obj = new(ptr) T{args...};
		std::cout << "XXX\n";
		return std::unique_ptr<T, Deleter>(obj, Deleter{
			.index = id,
			.pool = this,
		});
	}
private:
	void destroy(std::size_t i) {
		this->get(i)->~T();
		this->reusable.push(i);
	}

	T * get(std::size_t i) {
		return &(*this->chunks[i / CHUNK_SIZE])[i % CHUNK_SIZE];
	}

	std::size_t alloc() {
		if(!this->reusable.empty()) {
			const auto id = this->reusable.front();
			this->reusable.pop();
			return id;
		}
		else return this->next++;
	}

	std::vector<std::unique_ptr<Chunk>> chunks;
	std::size_t next = 0;
	std::queue<std::size_t> reusable;
};


struct S {
	int x;
};


int main() {
	MemoryPool<S, 4> pool;
	auto s1 = pool.make_unique(1);
	auto s2 = pool.make_unique(2);
	auto s3 = pool.make_unique(3);
	auto s4 = pool.make_unique(4);
	return 0;
}
