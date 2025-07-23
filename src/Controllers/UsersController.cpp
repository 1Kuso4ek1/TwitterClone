#include "Controllers/UsersController.hpp"

#include "Models/Users.hpp"

namespace Models = drogon_model::whoitter;

namespace Controllers
{

void UsersController::getUserById(const HttpRequestPtr& req, Callback&& callback, const int id)
{
    static auto mapper = orm::Mapper<Models::Users>(app().getDbClient());

    try
    {
        const auto user = mapper.findByPrimaryKey(id);

        callback(HttpResponse::newHttpJsonResponse(user.toJson()));
    }
    catch(...)
    {
        LOG_ERROR << "Failed to get user by id: " << id;

        callback(HttpResponse::newHttpResponse(k404NotFound, CT_NONE));
    }
}

void UsersController::getMe(const HttpRequestPtr& req, Callback&& callback)
{
    static auto mapper = orm::Mapper<Models::Users>(app().getDbClient());

    const auto selfId = std::stoi(req->getParameter("user_id"));

    try
    {
        const auto user = mapper.findByPrimaryKey(selfId);

        callback(HttpResponse::newHttpJsonResponse(user.toJson()));
    }
    catch(...)
    {
        LOG_ERROR << "Failed to get user by selfId: " << selfId;

        callback(HttpResponse::newHttpResponse(k404NotFound, CT_NONE));
    }
}

}
