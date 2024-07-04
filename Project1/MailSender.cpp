#pragma once
#include "Schedule.cpp"

class MailSender {
public:
	virtual void sendMail(Schedule *schedule) {
		//send mail
	}
};
