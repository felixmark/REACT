#pragma once
#include "IDigitalOutput.h"
#include <wiringPi.h>
#include <string>
#include <iostream>
#include <memory>

class Digital_output : public IDigital_output {
private:
	std::string name_;
	int pin_;

public:
	Digital_output();
	Digital_output(std::string name, int pin, Status status);
	virtual Status read() const override;
	virtual bool write(Status status) override;
	virtual std::string get_name() const override;
	virtual int get_pin() const override;
	friend std::ostream& operator<< (std::ostream& os, const Digital_output& dout);
};