#include "BankAccount.h"


BankAccount::BankAccount() : balance(0.0) {}
BankAccount::BankAccount(double initBalance) : balance(initBalance) {}

void BankAccount::deposit(double amount) {
	std::lock_guard<std::mutex> lock{mtx};
	balance += amount;

	//std::cout << "Deposited: " << amount << " | Current balance: " << balance << std::endl;
	cv.notify_all();
}

void BankAccount::withdraw(double amount) {
		std::unique_lock<std::mutex> lock{mtx};
		cv.wait(lock, [this, amount]() {return balance >= amount; });

		balance -= amount;
		//std::cout << "Withdrew: " << amount << " | Current balance: " << balance << std::endl;
}

double BankAccount::getBalance() const  {
	std::lock_guard<std::mutex> lock{mtx};

	return balance;
}

