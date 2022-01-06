#pragma once
#include <unordered_map>
#include <stdexcept>

namespace stx {
	template<
		template <typename ID, typename Input> typename BaseState,
		typename ID,
		typename Input>
	class fsm_core {
	public:
		using id_type = ID;
		using input_type = Input;
		using state_type = BaseState<ID, Input>;
		fsm_core() = default;
		fsm_core(const fsm_core &) = delete;
		fsm_core & operator=(const fsm_core &) = delete;
		fsm_core(fsm_core &&) = delete;
		fsm_core & operator=(fsm_core &&) = delete;

		void enter(const id_type & id) {
			this->change_state(id);		
		}

		void update(const input_type & input) {
			this->change_state(this->current_state()(input));
		}

		void update(input_type & input) {
			this->change_state(this->current_state()(input));
		}

		const state_type & current_state() const {
			if(!current) {
				throw std::logic_error{"No current state is set"};
			} 
			return *this->current;
		}

	protected:
		void change_state(const id_type & id){
			if(!this->statetable.contains(id)) {
				throw std::out_of_range{"Does not contain selected state"};
			}
			this->current = statetable.at(id);
		}

		void add(const state_type & s) {
			statetable.insert({s.id(), &s});
		}
	private:
		std::unordered_map<id_type, const state_type *> statetable;
		const state_type * current;
	};
}
