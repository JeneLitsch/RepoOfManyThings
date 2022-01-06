#pragma once
#include <memory>
#include <vector>

namespace stx {
	template<
		template <typename ID, typename Input> typename BaseState,
		typename ID,
		typename Input>
	class basic_fsm : public fsm_core<BaseState, ID, Input> {
	public:
		void add(std::unique_ptr<BaseState<ID, Input>> && s) {
			this->fsm_core<BaseState, ID, Input>::add(*s);
			states.push_back(std::move(s));
		}
	private:
		std::vector<std::unique_ptr<const BaseState<ID, Input>>> states;
	};
	


	// All hail the fsm :D
}