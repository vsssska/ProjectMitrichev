#include "BankAccount.h"


BankAccount::BankAccount() : balance(0.0) {}
BankAccount::BankAccount(double initBalance) : balance(initBalance) {}

void BankAccount::deposit(double amount) {
	std::lock_guard<std::mutex> lock{mtx};
	balance += amount;

	cv.notify_all();
}



