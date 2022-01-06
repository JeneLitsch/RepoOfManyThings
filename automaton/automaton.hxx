#pragma once
#include "automaton/state.hxx"
#include "automaton/table_state.hxx"
#include "automaton/probabilistic_state.hxx"
#include "automaton/fsm_core.hxx"
#include "automaton/fsm.hxx"

namespace stx {
	template <typename ID, typename Input>
	using fsm = basic_fsm<state, ID, Input>;
	
	template <typename ID, typename RNG>
	using pa = basic_fsm<probabilistic_state, ID, RNG>;
}