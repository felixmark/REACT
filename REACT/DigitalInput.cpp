#include "DigitalInput.h"

Digital_input::Digital_input() : Digital_input("?", 0, PUD_DOWN) { /* Call other constructor */ }

Digital_input::Digital_input(std::string name, int pin, int pullUpDown) : name_{ name }, pin_{ pin } {
	// Setup Pin
	pinMode(pin_, INPUT);
	pullUpDnControl(pin_, pullUpDown);
}

Status Digital_input::read() const {
	return int_to_status(digitalRead(pin_));
}

std::string Digital_input::get_name() const {
	return name_;
}

int Digital_input::get_pin() const {
	return pin_;
}

std::ostream & operator<<(std::ostream & os, const Digital_input & di) {
	return os << di.name_ << " (Pin " << di.pin_ << "): " << (di.read() == Status::ON ? "ON" : "OFF");
}
