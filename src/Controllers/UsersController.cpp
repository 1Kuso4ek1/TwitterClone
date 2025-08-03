#include "Controllers/UsersController.hpp"

#include "Models/Users.hpp"

namespace Models = drogon_model::TwitterClone;

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

void UsersController::updateMe(const HttpRequestPtr& req, Callback&& callback)
{
    static auto mapper = orm::Mapper<Models::Users>(app().getDbClient());

    const auto selfId = std::stoi(req->getParameter("user_id"));
    const auto json = *req->getJsonObject();

    if(!json.isMember("display_name") || !json.isMember("username")
        || json["username"].asString().find(' ') != std::string::npos)
    {
        callback(HttpResponse::newHttpResponse(k400BadRequest, CT_NONE));
        return;
    }

    try
    {
        auto user = mapper.findByPrimaryKey(selfId);
        user.setDisplayName(json["display_name"].asString());
        user.setUsername(json["username"].asString());

        mapper.update(user);

        callback(HttpResponse::newHttpJsonResponse(user.toJson()));
    }
    catch(...)
    {
        callback(HttpResponse::newHttpResponse(k409Conflict, CT_NONE));
    }
}

void UsersController::uploadAvatar(const HttpRequestPtr& req, Callback&& callback)
{
    static auto mapper = orm::Mapper<Models::Users>(app().getDbClient());

    const auto selfId = std::stoi(req->getParameter("user_id"));

    MultiPartParser parser;
    if(parser.parse(req) != 0)
    {
        callback(HttpResponse::newHttpResponse(k400BadRequest, CT_NONE));
        return;
    }

    const auto& files = parser.getFiles();
    if(files.size() != 1)
    {
        callback(HttpResponse::newHttpResponse(k400BadRequest, CT_NONE));
        return;
    }

    const auto& file = files[0];
    const auto filename = std::format("{}.{}", file.getMd5(), file.getFileExtension());
    const auto url = std::format(
        "{}://{}/img/{}",
        req->getConnectionPtr().lock()->isSSLConnection() ? "https" : "http",
        req->getHeader("host"),
        filename
    );

    try
    {
        auto user = mapper.findByPrimaryKey(selfId);
        user.setAvatarUrl(url);

        mapper.update(user);

        file.saveAs(std::format("{}/img/{}", app().getDocumentRoot(), filename));

        callback(HttpResponse::newHttpJsonResponse(user.toJson()));
    }
    catch(...)
    {
        LOG_ERROR << "Failed to get user by selfId: " << selfId;

        callback(HttpResponse::newHttpResponse(k404NotFound, CT_NONE));
    }
}

}
