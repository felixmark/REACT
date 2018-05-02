#include "Status.h"

bool status_to_bool(Status status) {
	return status == Status::ON ? true : false;
}

int status_to_highlow(Status status) {
	return status == Status::ON ? HIGH : LOW;
}

Status int_to_status(int value) {
	return value == 0 ? Status::OFF : Status::ON;
}
