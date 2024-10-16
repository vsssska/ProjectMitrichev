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


int main() {
    testDeposit();
    testWithdraw();

    return 0;
}
