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
    static void updateMe(const HttpRequestPtr& req, Callback&& callback);
    static void uploadAvatar(const HttpRequestPtr& req, Callback&& callback);

public:
    METHOD_LIST_BEGIN

        ADD_METHOD_TO(getUserById, "/api/users/{id}", Get, "Filters::JwtFilter");
        ADD_METHOD_TO(getMe, "/api/users/me", Get, "Filters::JwtFilter");
        ADD_METHOD_TO(updateMe, "/api/users/me", Post, "Filters::JwtFilter");
        ADD_METHOD_TO(uploadAvatar, "/api/users/me/avatar", Post, "Filters::JwtFilter");

    METHOD_LIST_END
};

}
