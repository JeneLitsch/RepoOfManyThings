#include "automaton.hxx"
#include <string>
#include <iostream>

// class S : public stx::state<std::string, int> {
// public:
// 	S(const std::string & id_key) : state(id_key) {}
// 	virtual std::string operator()(const int & i) const override {
// 		if(i == 0) return "a";
// 		if(i == 1) return "b";
// 		return "a";
// 	}
// };

int main(){
	{	
		std::cout << "Normal FSM\n";
		using S = stx::table_state<std::string, int>;
		stx::fsm<std::string, int> stateMachine;
		stateMachine.add(std::make_unique<S>("a", S::table_type{
			{0, "a"},
			{1, "b"},
		}));
		stateMachine.add(std::make_unique<S>("b", S::table_type{
			{0, "a"},
			{1, "b"},
		}));

		stateMachine.enter("a");
		std::cout << stateMachine.current_state().id() << "\n";
		stateMachine.update(0);
		std::cout << stateMachine.current_state().id() << "\n";
		stateMachine.update(1);
		std::cout << stateMachine.current_state().id() << "\n";
	}
	{
		std::cout << "Probabilistic FSM\n";
		std::mt19937_64 rng;
		rng.seed(std::random_device()());
		stx::pa<std::string, std::mt19937_64> stateMachine;
		using S = decltype(stateMachine)::state_type;
		stateMachine.add(std::make_unique<S>("a", S::table_type{
			{"a", 100},
			{"b", 1},
		}));
		stateMachine.add(std::make_unique<S>("b", S::table_type{
			{"a", 1},
			{"b", 100},
		}));
		stateMachine.enter("a");
		for(std::size_t i = 0; i < 100; i++) {
			stateMachine.update(rng);
			std::cout << stateMachine.current_state().id();
		}
		std::cout << "\n";
	}
	return 0;
}
