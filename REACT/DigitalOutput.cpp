#include "DigitalOutput.h"

Digital_output::Digital_output() : Digital_output("?", 0, Status::OFF) { /* Call other constructor */ }

Digital_output::Digital_output(std::string name, int pin, Status status) : name_{ name }, pin_{ pin } {
	// Setup
	pinMode(pin_, OUTPUT);
	digitalWrite(pin_, status_to_highlow(status));
}

Status Digital_output::read() const {
	return int_to_status(digitalRead(pin_));
}

bool Digital_output::write(Status status) {
	if (status == read()) {
		return false;
	}
	digitalWrite(pin_, status_to_highlow(status));
	return true;
}

std::string Digital_output::get_name() const {
	return name_;
}

int Digital_output::get_pin() const {
	return pin_;
}

std::ostream & operator<<(std::ostream & os, const Digital_output & dout) {
	return os << dout.name_ << " (Pin " << dout.pin_ << "): " << (dout.read() == Status::ON ? "ON" : "OFF");
}
