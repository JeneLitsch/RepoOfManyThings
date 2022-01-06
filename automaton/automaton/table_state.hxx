#pragma once
#include "state.hxx"
#include <unordered_map>

namespace stx {
	template<typename ID, typename Input>
	class table_state : public state<ID, Input> {
	public:
		using table_type = std::unordered_map<Input, ID>;
		
		table_state(const ID & id_key, const table_type & table)
			: state<ID, Input>(id_key), table(table) {}
		
		virtual ID operator()(const Input & input) const override {
			return this->table.at(input);
		}	
	
	private:
		table_type table;
	};
}
