#pragma once
#include <string>
#include "Status.h"

class IDigital_input {
public:
	virtual ~IDigital_input() = 0;
	virtual Status read() const = 0;
	virtual std::string get_name() const = 0;
	virtual int get_pin() const = 0;
};