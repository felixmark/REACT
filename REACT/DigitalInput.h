#pragma once
#include "IDigitalInput.h"
#include <wiringPi.h>
#include <string>
#include <iostream>
#include <memory>

class Digital_input : public IDigital_input {
private:
	std::string name_;
	int pin_;

public:
	Digital_input();
	Digital_input(std::string name, int pin, int pullUpDown);
	virtual Status read() const override;
	virtual std::string get_name() const override;
	virtual int get_pin() const override;
	friend std::ostream& operator<< (std::ostream& os, const Digital_input& di);
};