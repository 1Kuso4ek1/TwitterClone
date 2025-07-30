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

void StaticController::wasmApp(const HttpRequestPtr& req, Callback&& callback)
{
    callback(HttpResponse::newFileResponse("../static/html/TwitterClone.html"));
}

void StaticController::wasmFile(const HttpRequestPtr& req, Callback&& callback)
{
    callback(HttpResponse::newFileResponse("../static/wasm/TwitterClone.wasm", "", CT_APPLICATION_WASM));
}

void StaticController::wasmAppJs(const HttpRequestPtr& req, Callback&& callback)
{
    callback(HttpResponse::newFileResponse("../static/js/TwitterClone.js"));
}

void StaticController::qtLoader(const HttpRequestPtr& req, Callback&& callback)
{
    callback(HttpResponse::newFileResponse("../static/js/qtloader.js"));
}

void StaticController::qtLogo(const HttpRequestPtr& req, Callback&& callback)
{
    callback(HttpResponse::newFileResponse("../static/img/qtlogo.svg"));
}

}
