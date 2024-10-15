#pragma once
#include <drogon/HttpController.h>

class FrontendController : public drogon::HttpController<FrontendController> {
	public:
		METHOD_LIST_BEGIN
		ADD_METHOD_TO(FrontendController::bankPage, "/index", drogon::Get);
		METHOD_LIST_END

		void bankPage(const drogon::HttpRequestPtr& req, std::function<void(const drogon::HttpResponsePtr&)> &&callback) {
			auto resp = drogon::HttpResponse::newFileResponse("/app/frontend/index.html");
			resp->setContentTypeCode(drogon::CT_TEXT_HTML);
			callback(resp);
		}

};

