#include "BankAccount.h"


BankAccount::BankAccount() : balance(0.0) {}
BankAccount::BankAccount(double initBalance) : balance(initBalance) {}

void BankAccount::deposit(double amount) {
	std::lock_guard<std::mutex> lock{mtx};
	balance += amount;

	cv.notify_all();
}

void BankAccount::withdraw(double amount) {
		std::unique_lock<std::mutex> lock{mtx};
		//cv.wait(lock, [this, amount]() {return balance >= amount; });
		if (balance > 0) balance -= amount;
		else balance = 0;
		//cv.notify_all();
}

double BankAccount::getBalance() const  {
	std::lock_guard<std::mutex> lock{mtx};

	return balance;
}

std::mutex& BankAccount::getMutex() const {
	return mtx;

}

void BankAccount::waitForFunds(double amount) {
	std::unique_lock<std::mutex> lock{mtx};
	cv.wait(lock, [this, amount]() { return balance >= amount; });
}


