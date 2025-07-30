#pragma once
#include <drogon/HttpController.h>

using namespace drogon;

using Callback = std::function<void(const HttpResponsePtr&)>;

namespace Controllers
{

class StaticController final : public HttpController<StaticController>
{
public:
    static void swaggerPage(const HttpRequestPtr& req, Callback&& callback);
    static void swaggerFile(const HttpRequestPtr& req, Callback&& callback);

    static void index(const HttpRequestPtr& req, Callback&& callback);
    static void login(const HttpRequestPtr& req, Callback&& callback);

    static void wasmApp(const HttpRequestPtr& req, Callback&& callback);
    static void wasmFile(const HttpRequestPtr& req, Callback&& callback);
    static void wasmAppJs(const HttpRequestPtr& req, Callback&& callback);
    static void qtLoader(const HttpRequestPtr& req, Callback&& callback);
    static void qtLogo(const HttpRequestPtr& req, Callback&& callback);

public:
    METHOD_LIST_BEGIN

        ADD_METHOD_TO(swaggerPage, "/swagger", Get);
        ADD_METHOD_TO(swaggerFile, "/swagger/swagger.json", Get);

        ADD_METHOD_TO(index, "/", Get);
        ADD_METHOD_TO(login, "/login", Get);

        ADD_METHOD_TO(wasmApp, "/wasm", Get);
        ADD_METHOD_TO(wasmFile, "/TwitterClone.wasm", Get);
        ADD_METHOD_TO(wasmAppJs, "/TwitterClone.js", Get);
        ADD_METHOD_TO(qtLoader, "/qtloader.js", Get);

        ADD_METHOD_TO(qtLogo, "/qtlogo.svg", Get);

    METHOD_LIST_END
};

}
