#ifndef ACCOUNT_CONTROLLER_H
#define ACCOUNT_CONTROLLER_H

#include "BankAccount.h"
#include <drogon/HttpController.h>
#include <thread>

const static double STD_INIT_AMOUNT = 1000.0;

class AccountController : public drogon::HttpController<AccountController> {
	private:
		BankAccount account;

	public:
		AccountController();

		METHOD_LIST_BEGIN
		ADD_METHOD_TO(AccountController::deposit, "/deposit", drogon::Post);
		ADD_METHOD_TO(AccountController::withdraw, "/withdraw", drogon::Post);
		ADD_METHOD_TO(AccountController::getBalance, "/balance", drogon::Get);
		METHOD_LIST_END


		void deposit(const drogon::HttpRequestPtr &req, std::function<void(const drogon::HttpResponsePtr &)> &&callback);
		void withdraw(const drogon::HttpRequestPtr &req, std::function<void(const drogon::HttpResponsePtr &)> &&callback);
		void getBalance(const drogon::HttpRequestPtr& req, std::function<void(const drogon::HttpResponsePtr&)> &&callback);
};


#endif

