# Проект 1
## Вариант 9
### Описание задачи

В данном проекте требуется разработать программу, которая позволяет нескольким клиентам (потокам управления) управлять общим счетом. Клиенты могут выполнять следующие операции:
- Помещать средства на счет (запрос вида "положить amount рублей").
- Снимать средства со счета (запрос вида "снять amount рублей").

Программа должна удовлетворять следующим требованиям:

1. **Консистентность счета**: Текущий баланс счета определяется как сумма всех средств, внесенных клиентами, минус сумма всех снятых средств.
   
2. **Отрицательный баланс запрещен**: Баланс счета никогда не должен быть отрицательным. При попытке снять больше средств, чем имеется на счете, клиенту нужно дождаться достаточного пополнения баланса.

3. **Безопасность работы с потоками**: Программа должна поддерживать работу с несколькими потоками одновременно, при этом избегая состояния гонки (race conditions). Это требует синхронизации потоков при доступе к общему счету.

4. **Ожидание при нехватке средств**: Если клиент запрашивает снятие средств, но на счету недостаточно денег, программа должна приостановить выполнение этого потока до тех пор, пока на счет не будет положена достаточная сумма другим клиентом.

5. **Логирование операций**: Программа должна выводить перечень всех операций, которые выполняются (внесение и снятие средств). Лог должен содержать информацию о:
   - типе операции (внесение или снятие),
   - сумме операции,
   - текущем балансе после выполнения операции,
   - идентификаторе клиента (потока), выполняющего операцию.



Документация кода 

# main.cpp

```cpp
#include <drogon/drogon.h>
#include "AccountController.h"
#include "FrontendController.h"

/**
 * @brief Точка входа в приложение.
 *
 * Этот файл содержит основной метод для запуска веб-сервера,
 * используя фреймворк Drogon. Он настраивает слушатель, 
 * устанавливает количество потоков и запускает приложение.
 *
 * @return 0, если приложение завершилось успешно.
 */
int main() {
    // Добавление слушателя на IP-адрес 0.0.0.0 и порт 8848
	drogon::app().addListener("0.0.0.0", 8848);
    
    // Установка количества потоков для обработки запросов
	drogon::app().setThreadNum(4);
    
    // Запуск приложения
	drogon::app().run();

	return 0;
}


# BankAccount.h

#ifndef BANK_ACCOUNT_H
#define BANK_ACCOUNT_H

#include <mutex>
#include <condition_variable>

/**
 * @class BankAccount
 * @brief Класс, представляющий банковский счет с поддержкой многопоточности.
 *
 * Этот класс управляет балансом банковского счета и обеспечивает 
 * безопасный доступ к нему из нескольких потоков с использованием 
 * мьютексов и переменных условия.
 */
class BankAccount {
	private:
		double balance;               ///< Текущий баланс счета
		mutable std::mutex mtx;      ///< Мьютекс для защиты доступа к балансу
		std::condition_variable cv;   ///< Переменная условия для синхронизации потоков

	public:
		/**
		 * @brief Конструктор по умолчанию, инициализирующий баланс нулем.
		 */
		BankAccount();

		/**
		 * @brief Конструктор, инициализирующий баланс заданной суммой.
		 *
		 * @param initBalance Начальный баланс счета.
		 */
		BankAccount(double initBalance);

		/**
		 * @brief Метод для пополнения счета.
		 *
		 * @param amount Сумма для пополнения.
		 */
		void deposit(double amount);

		/**
		 * @brief Метод для снятия средств со счета.
		 *
		 * @param amount Сумма для снятия.
		 */
		void withdraw(double amount);

		/**
		 * @brief Метод ожидания наличия достаточных средств на счете.
		 *
		 * @param amount Сумма, которую необходимо снять.
		 */
		void waitForFunds(double amount);

		/**
		 * @brief Метод для получения текущего баланса счета.
		 *
		 * @return Текущий баланс.
		 */
		double getBalance() const;
};

#endif

# BankAccount.cpp

#include "BankAccount.h"

// Конструктор по умолчанию
BankAccount::BankAccount() : balance(0.0) {}

// Конструктор с инициализацией баланса
BankAccount::BankAccount(double initBalance) : balance(initBalance) {}

/**
 * @brief Метод для пополнения счета.
 *
 * Этот метод добавляет указанную сумму к текущему балансу,
 * блокируя мьютекс для безопасного доступа.
 *
 * @param amount Сумма для пополнения.
 */
void BankAccount::deposit(double amount) {
	std::lock_guard<std::mutex> lock{mtx};
	balance += amount;

	// Уведомление всех ожидающих потоков о том, что баланс изменился.
	cv.notify_all();
}

/**
 * @brief Метод для снятия средств со счета.
 *
 * Этот метод уменьшает баланс на указанную сумму, ожидая 
 * наличия достаточных средств перед снятием.
 *
 * @param amount Сумма для снятия.
 */
void BankAccount::withdraw(double amount) {
	std::unique_lock<std::mutex> lock{mtx};
	cv.wait(lock, [this, amount]() { return balance >= amount; });

	balance -= amount;
}

/**
 * @brief Метод для получения текущего баланса.
 *
 * @return Текущий баланс.
 */
double BankAccount::getBalance() const {
	std::lock_guard<std::mutex> lock{mtx};
	return balance;
}

/**
 * @brief Метод ожидания наличия достаточных средств на счете.
 *
 * Этот метод блокирует поток до тех пор, пока баланс не станет
 * достаточным для выполнения операции.
 *
 * @param amount Сумма, которую необходимо снять.
 */
void BankAccount::waitForFunds(double amount) {
	std::unique_lock<std::mutex> lock{mtx};
	cv.wait(lock, [this, amount]() { return balance >= amount; });
}


# AccountController.h


#ifndef ACCOUNT_CONTROLLER_H
#define ACCOUNT_CONTROLLER_H

#include "BankAccount.h"
#include <drogon/HttpController.h>
#include <thread>
#include <sstream>

const static double STD_INIT_AMOUNT = 1000.0; ///< Стандартная начальная сумма на счете

/**
 * @class AccountController
 * @brief Контроллер для обработки HTTP-запросов, связанных с банковским счетом.
 *
 * Этот контроллер управляет действиями пополнения, снятия и получения
 * текущего баланса счета через HTTP-запросы.
 */
class AccountController : public drogon::HttpController<AccountController> {
	private:
		BankAccount account; ///< Объект банковского счета

	public:
		/**
		 * @brief Конструктор контроллера, инициализирующий счет стандартной суммой.
		 */
		AccountController();

		METHOD_LIST_BEGIN
		/**
		 * @brief Эндпоинт для пополнения счета.
		 *
		 * Обрабатывает POST-запросы на /deposit.
		 */
		ADD_METHOD_TO(AccountController::deposit, "/deposit", drogon::Post);
		
		/**
		 * @brief Эндпоинт для снятия средств со счета.
		 *
		 * Обрабатывает POST-запросы на /withdraw.
		 */
		ADD_METHOD_TO(AccountController::withdraw, "/withdraw", drogon::Post);
		
		/**
		 * @brief Эндпоинт для получения текущего баланса.
		 *
		 * Обрабатывает GET-запросы на /balance.
		 */
		ADD_METHOD_TO(AccountController::getBalance, "/balance", drogon::Get);
		METHOD_LIST_END

		/**
		 * @brief Метод для пополнения счета.
		 *
		 * @param req Входящий HTTP-запрос.
		 * @param callback Функция обратного вызова для отправки ответа.
		 */
		void deposit(const drogon::HttpRequestPtr &req, std::function<void(const drogon::HttpResponsePtr &)> &&callback);

		/**
		 * @brief Метод для снятия средств со счета.
		 *
		 * @param req Входящий HTTP-запрос.
		 * @param callback Функция обратного вызова для отправки ответа.
		 */
		void withdraw(const drogon::HttpRequestPtr &req, std::function<void(const drogon::HttpResponsePtr &)> &&callback);

		/**
		 * @brief Метод для получения текущего баланса.
		 *
		 * @param req Входящий HTTP-запрос.
		 * @param callback Функция обратного вызова для отправки ответа.
		 */
		void getBalance(const drogon::HttpRequestPtr& req, std::function<void(const drogon::HttpResponsePtr&)> &&callback);
};

#endif


# AccountController.cpp


#include "AccountController.h"

// Конструктор контроллера
AccountController::AccountController() : account(STD_INIT_AMOUNT) {}

/**
 * @brief Метод для пополнения счета.
 *
 * Этот метод обрабатывает запрос на пополнение счета и выполняет 
 * его в отдельном потоке, чтобы избежать блокировок.
 *
 * @param req Входящий HTTP-запрос.
 * @param callback Функция обратного вызова для отправки ответа.
 */
void AccountController::deposit(const drogon::HttpRequestPtr& req, std::function<void(const drogon::HttpResponsePtr &)> &&callback) {
	auto amount = std::stod(req->getParameter("amount"));
	
	std::thread([this, amount, callback] () {
		account.deposit(amount);
		auto resp = drogon::HttpResponse::newHttpResponse();
		
		std::stringstream ss;
		ss << std::this_thread::get_id();
		
		resp->setBody("Пополнение успешно, вы пополнили " + std::to_string(amount) + " (ID потока: " + ss.str() + ")");
		callback(resp);
	}).detach();
}

/**
 * @brief Метод для снятия средств со счета.
 *
 * Этот метод обрабатывает запрос на снятие средств и выполняет его 
 * в отдельном потоке. Если средств недостаточно, ожидает их поступления.
 *
 * @param req Входящий HTTP-запрос.
 * @param callback Функция обратного вызова для отправки ответа.
 */
void AccountController::withdraw(const drogon::HttpRequestPtr& req, std::function<void(const drogon::HttpResponsePtr &)> &&callback) {
	auto amount = std::stod(req->getParameter("amount"));

	std::thread([this, amount, callback] () {
		std::stringstream ss;
		ss << std::this_thread::get_id();

		if (account.getBalance() < amount) {
			auto resp = drogon::HttpResponse::newHttpResponse();
			resp->setBody("Недостаточно средств для снятия " + std::to_string(amount) + " (ID потока: " + ss.str() + ")");
			callback(resp);
			account.waitForFunds(amount);
		}
		account.withdraw(amount);

		auto resp = drogon::HttpResponse::newHttpResponse();
		resp->setBody("Снятие успешно, вы сняли " + std::to_string(amount) + " (ID потока: " + ss.str() + ")");
		callback(resp);
	}).detach();
}

/**
 * @brief Метод для получения текущего баланса.
 *
 * Этот метод обрабатывает запрос на получение баланса и отправляет 
 * его в ответе.
 *
 * @param req Входящий HTTP-запрос.
 * @param callback Функция обратного вызова для отправки ответа.
 */
void AccountController::getBalance(const drogon::HttpRequestPtr& req, std::function<void(const drogon::HttpResponsePtr &)> &&callback) {
	auto resp = drogon::HttpResponse::newHttpResponse();
	resp->setBody("Текущий баланс: " + std::to_string(account.getBalance()));
	callback(resp);
}

# FrontendController.h


#pragma once
#include <drogon/HttpController.h>

/**
 * @class FrontendController
 * @brief Контроллер для обработки HTTP-запросов на фронтенде банковского приложения.
 *
 * Этот контроллер отвечает за обслуживание основного интерфейса банка и обработку
 * HTTP-запросов, связанных с фронтендом банка.
 */
class FrontendController : public drogon::HttpController<FrontendController> {
	public:
		METHOD_LIST_BEGIN
		/**
		 * @brief Эндпоинт для обслуживания банковской страницы.
		 * 
		 * Этот метод обрабатывает GET-запросы к пути "/index", возвращая
		 * основной HTML-файл для интерфейса банковского счета.
		 */
		ADD_METHOD_TO(FrontendController::bankPage, "/index", drogon::Get);
		METHOD_LIST_END

		/**
		 * @brief Обслуживает HTML-файл банковской страницы.
		 *
		 * Этот метод создает HTTP-ответ, который возвращает HTML-файл
		 * банковской страницы, расположенный по адресу "/app/frontend/index.html".
		 * Он устанавливает тип контента ответа на HTML перед отправкой его клиенту.
		 *
		 * @param req Входящий HTTP-запрос.
		 * @param callback Функция обратного вызова для отправки ответа.
		 */
		void bankPage(const drogon::HttpRequestPtr& req, std::function<void(const drogon::HttpResponsePtr&)> &&callback) {
			auto resp = drogon::HttpResponse::newFileResponse("/app/frontend/index.html");
			resp->setContentTypeCode(drogon::CT_TEXT_HTML);
			callback(resp);
		}
};

