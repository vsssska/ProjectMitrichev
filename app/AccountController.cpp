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


void AccountController::withdraw(const drogon::HttpRequestPtr& req, std::function<void (const drogon::HttpResponsePtr &)> &&callback) { //double amount) {
	auto amount = std::stod(req->getParameter("amount"));//.asDouble();

	std::thread([this, amount, callback] () {
		account.withdraw(amount);
		auto resp = drogon::HttpResponse::newHttpResponse();
                resp->setBody("Withdraw successful, you've withdrawn  " + std::to_string(amount));
                callback(resp);
	}).detach();
}

void AccountController::getBalance(const drogon::HttpRequestPtr& req, std::function<void (const drogon::HttpResponsePtr &)> &&callback) {
	auto resp = drogon::HttpResponse::newHttpResponse();
	resp->setBody("Current Balance: " + std::to_string(account.getBalance()));
	callback(resp);
}

