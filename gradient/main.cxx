#include "gradient.hxx"

int main() {
	const std::map<float, float> gradient = {
		{0.0, 10.0},
		{0.1, 0.2},
		{0.2, 0.3},
		{0.2, 0.4},
		{0.3, 0.6},
		{0.4, 0.8},
		{0.5, 1.0},
	};

	std::cout
		<< stx::gradient<float, float>(gradient.begin(), gradient.end(), -0.49f) << "\n"
		<< stx::gradient<float, float>(gradient.begin(), gradient.end(), 0.05f) << "\n"
		<< stx::gradient<float, float>(gradient.begin(), gradient.end(), 0.1f) << "\n"
		<< stx::gradient<float, float>(gradient.begin(), gradient.end(), 0.20f) << "\n"
		<< stx::gradient<float, float>(gradient.begin(), gradient.end(), 0.20f) << "\n"
		<< stx::gradient<float, float>(gradient.begin(), gradient.end(), 0.25f) << "\n"
		<< stx::gradient<float, float>(gradient.begin(), gradient.end(), 0.49f) << "\n"
		<< stx::gradient<float, float>(gradient.begin(), gradient.end(), 1.49f) << "\n";
}