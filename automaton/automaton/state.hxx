#pragma once

namespace stx {
	template<typename ID>
	class state_base {
	public:
		state_base(const ID & id_key) : id_key(id_key) {}
		const ID & id() const {
			return this->id_key;
		} 
	private:
		ID id_key;
	};

	template<typename ID, typename Input>
	class state : public state_base<ID> {
	public:
		state(const ID & id_key) : state_base<ID>(id_key) {}
		virtual ID operator()(const Input & input) const = 0;
	};
}
