#pragma once
#include "Status.h"
#include <string>

class IDigital_output {
public:
	virtual ~IDigital_output() = 0;
	virtual Status read() const = 0;
	virtual bool write(Status status) = 0;
	virtual std::string get_name() const = 0;
	virtual int get_pin() const = 0;
};