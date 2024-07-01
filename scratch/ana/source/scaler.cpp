#include "scaler.hh"

void ScalerB2F::update(const std::array<int, 32> &values) {
	for (int i = 0; i < 32; i++) {
		this->values[i] = values[i];
	}
}
