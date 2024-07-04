#pragma once
#include <ctime>
#include "Customer.cpp"

using std::time_t;

class Schedule {
public:
	Schedule(tm dateTime, int numberOfPeople, Customer customer) :
		dateTime{ dateTime }, 
		numberOfPeople{ numberOfPeople }, 
		customer{ customer } {
	}

	tm getDateTime() {
		return dateTime;
	}

	int getNumberOfPeople() {
		return numberOfPeople;
	}

	Customer getCustomer() {
		return customer;
	}

private:
	tm dateTime;
	int numberOfPeople;
	Customer customer;
};
