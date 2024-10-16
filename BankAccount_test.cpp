#include <iostream>
#include "BankAccount.h"

void testDeposit() {
    BankAccount account;
    account.deposit(100.0);
    if (account.getBalance() != 100.0) {
        std::cerr << "Test Deposit Failed: Expected 100.0, got " << account.getBalance() << std::endl;
    } else {
        std::cout << "Test Deposit Passed." << std::endl;
    }
}

void testWithdraw() {
    BankAccount account(100.0);
    account.withdraw(50.0);
    if (account.getBalance() != 50.0) {
        std::cerr << "Test Withdraw Failed: Expected 50.0, got " << account.getBalance() << std::endl;
    } else {
        std::cout << "Test Withdraw Passed." << std::endl;
    }
}

void testWithdrawInsufficientFunds() {
    BankAccount account(50.0);
    account.withdraw(100.0); // This should block until enough funds are deposited.
    account.deposit(100.0); // Now there are sufficient funds.
    account.withdraw(100.0);
    if (account.getBalance() != 0.0) {
        std::cerr << "Test Withdraw Insufficient Funds Failed: Expected 0.0, got " << account.getBalance() << std::endl;
    } else {
        std::cout << "Test Withdraw Insufficient Funds Passed." << std::endl;
    }
}

int main() {
    testDeposit();
    testWithdraw();
    testWithdrawInsufficientFunds();

    return 0;
}
