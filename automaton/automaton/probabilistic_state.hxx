#pragma once
#include "state.hxx"
#include <vector>
#include <random>

namespace stx {
	template<typename ID, typename RNG>
	class probabilistic_state : public state_base<ID> {
	public:
		using transition_type = std::tuple<ID, int>;
		using table_type = std::vector<transition_type>;
		
		probabilistic_state(const ID & id_key, const table_type & table)
			: state_base<ID>(id_key), table(table) {}
		
		virtual ID operator()(RNG & rng) const {
			std::vector<int> weights;
			for(const auto & [id, weight] : table) {
				weights.push_back(weight);
			}
			const auto begin = weights.begin();
			const auto end = weights.end();
			const auto i = std::discrete_distribution<int>{begin, end}(rng);
			const auto & [id, weight] = table[i];
			return id;
		}
	
	private:
		table_type table;
	};
}
