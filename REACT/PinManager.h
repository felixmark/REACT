#pragma once
#include <vector>
#include <stdexcept>
#include <algorithm>

class Pin_manager {
private:
	static std::vector<int> reserved_pins_;
public:
	void reserve(int pin);
	void release(int pin);
};