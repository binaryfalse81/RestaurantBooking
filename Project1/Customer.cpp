#pragma once
#include <string>

using std::string;

class Customer {
public:
    Customer(string name, string phoneNumber) : 
        name{ name }, 
        phoneNumber{ phoneNumber } {
    }

    Customer(string name, string phoneNumber, string email) :
        name{ name }, 
        phoneNumber{ phoneNumber }, 
        email{ email } {
    }

    virtual string getEmail() {
        return email;
    }

private:
    string name;
    string phoneNumber;
    string email;
};
