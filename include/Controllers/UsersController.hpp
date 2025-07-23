#pragma once
#include <drogon/HttpController.h>

using namespace drogon;

using Callback = std::function<void(const HttpResponsePtr&)>;

namespace Controllers
{

class UsersController final : public HttpController<UsersController>
{
public:
    static void getUserById(const HttpRequestPtr& req, Callback&& callback, int id);
    static void getMe(const HttpRequestPtr& req, Callback&& callback);

public:
    METHOD_LIST_BEGIN

        ADD_METHOD_TO(getUserById, "/api/users/{id}", Get);
        ADD_METHOD_TO(getMe, "/api/users/me", Get, "Filters::JwtFilter");

    METHOD_LIST_END
};

}
