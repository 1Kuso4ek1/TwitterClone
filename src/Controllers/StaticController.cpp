#include <Controllers/StaticController.hpp>

namespace Controllers
{

void StaticController::swaggerPage(const HttpRequestPtr& req, Callback&& callback)
{
    callback(HttpResponse::newRedirectionResponse("swagger/index.html"));
}

void StaticController::swaggerFile(const HttpRequestPtr& req, Callback&& callback)
{
    callback(HttpResponse::newFileResponse("../static/swagger.json"));
}

void StaticController::index(const HttpRequestPtr& req, Callback&& callback)
{
    callback(HttpResponse::newFileResponse("../static/html/index.html"));
}

void StaticController::login(const HttpRequestPtr& req, Callback&& callback)
{
    if(req->getSession()->find("jwtAccess"))
    {
        LOG_INFO << "User already logged in, redirecting to /";

        callback(HttpResponse::newRedirectionResponse("/"));
        return;
    }

    callback(HttpResponse::newFileResponse("../static/html/login.html"));
}

}
