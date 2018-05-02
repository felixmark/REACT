#include "PinManager.h"

void Pin_manager::reserve(int pin) {
	std::vector<int>::iterator position = std::find(reserved_pins_.begin(), reserved_pins_.end(), pin);
	if (position != reserved_pins_.end())
		throw std::runtime_error("Pin was already reserved!");
	else reserved_pins_.push_back(pin);
}

void Pin_manager::release(int pin) {
	std::vector<int>::iterator position = std::find(reserved_pins_.begin(), reserved_pins_.end(), pin);
	if (position != reserved_pins_.end())
		reserved_pins_.erase(position);
	else  throw std::runtime_error("Pin was not reserved!");
}
