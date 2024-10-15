#ifndef BANK_ACCOUNT_H
#define BANK_ACCOUNT_H

#include <mutex>
#include <condition_variable>

class BankAccount {
	private:
		double balance;
		mutable std::mutex mtx;
		std::condition_variable cv;

	public:
		BankAccount();
		BankAccount(double initBalance);

		void deposit(double amount);
		void withdraw(double amount);
		void waitForFunds(double amount);

		double getBalance() const;
		
};

#endif

