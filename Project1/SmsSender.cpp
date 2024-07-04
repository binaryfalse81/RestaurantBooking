#pragma once
#include "Schedule.cpp"

class SmsSender {
public:
	virtual void send(Schedule *schedule) {
		//send message to PhoneNumber
	}
};
