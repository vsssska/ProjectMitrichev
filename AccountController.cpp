#include "AccountController.h"

AccountController::AccountController() : account(STD_INIT_AMOUNT) {}

void AccountController::deposit(const drogon::HttpRequestPtr& req, std::function<void (const drogon::HttpResponsePtr &)> &&callback) {// double amount) {
	auto amount = std::stod(req->getParameter("amount"));//.asDouble();
	
	std::thread([this, amount, callback] () {
		account.deposit(amount);
		auto resp = drogon::HttpResponse::newHttpResponse();
		resp->setBody("Deposit successful, you've deposited " + std::to_string(amount));
	       callback(resp);	
	}).detach();
}


