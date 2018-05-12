#pragma once
#include <vector>
#include <stdexcept>
#include <algorithm>

class Pin_manager {
private:
	std::vector<int> reserved_pins_;
public:
	Pin_manager();
	int reserve(int pin);
	void release(int pin);
};