#include "Controllers/LoginController.hpp"

#include "Utils/JwtUtils.hpp"

#include "Models/Users.hpp"

#include <drogon/HttpClient.h>
#include <drogon/orm/Mapper.h>

using namespace std::chrono_literals;

namespace Models = drogon_model::whoitter;

namespace Controllers
{

void LoginController::login(const HttpRequestPtr& req, Callback&& callback)
{
    const auto& config = app().getCustomConfig();
    const auto state = utils::secureRandomString(12);
    const auto link = std::format(
        oauth2Template,
        config["oauth2"]["client_id"].asString(),
        config["oauth2"]["redirect_uri"].asString(),
        state
    );

    req->getSession()->insert("oauth2_state", state);

    callback(HttpResponse::newRedirectionResponse(link));
}

void LoginController::logout(const HttpRequestPtr& req, Callback&& callback)
{
    const auto response = HttpResponse::newHttpResponse();

    // Removing cookie
    Utils::saveRefreshToCookie("", response, 0);

    // Clearing session
    req->getSession()->erase("jwtAccess");

    callback(response);
}

void LoginController::oauth(
    const HttpRequestPtr& req, Callback&& callback,
    const std::string& code, const std::string& state
)
{
    if(const auto sessionState = req->getSession()->get<std::string>("oauth2_state");
        state != sessionState || sessionState.empty())
    {
        LOG_ERROR << "Invalid OAuth2 state: " << state << ", expected: " << sessionState;

        callback(HttpResponse::newHttpResponse(k400BadRequest, CT_NONE));
        return;
    }

    req->getSession()->erase("oauth2_state");

    const auto& config = app().getCustomConfig();

    static const auto client = HttpClient::newHttpClient("https://oauth2.googleapis.com/");

    const auto request = HttpRequest::newHttpFormPostRequest();
    request->setPath("/token");
    request->setParameter("code", code);
    request->setParameter("client_id", config["oauth2"]["client_id"].asString());
    request->setParameter("client_secret", config["oauth2"]["client_secret"].asString());
    request->setParameter("redirect_uri", config["oauth2"]["redirect_uri"].asString());
    request->setParameter("grant_type", "authorization_code");

    client->sendRequest(request,
        [callback = std::move(callback), req](const ReqResult reqRes, const HttpResponsePtr& resp)
        {
            if(resp->getJsonObject())
            {
                const auto& json = *resp->getJsonObject();
                const auto access = json["access_token"].asString();

                requestUser(req, access, static_cast<Callback>(callback));
            }
            else
            {
                LOG_ERROR << "Failed to get access token";

                callback(HttpResponse::newHttpResponse(k400BadRequest, CT_NONE));
            }
        });
}

void LoginController::requestUser(const HttpRequestPtr& req, const std::string& accessToken, Callback&& callback)
{
    static const auto client = HttpClient::newHttpClient("https://www.googleapis.com/");

    const auto request = HttpRequest::newHttpRequest();
    request->setPath("/oauth2/v2/userinfo");
    request->addHeader("Authorization", std::format("Bearer {}", accessToken));

    client->sendRequest(request,
        [callback = std::move(callback), req](const ReqResult reqRes, const HttpResponsePtr& resp)
        {
            if(const auto json = resp->getJsonObject(); json)
            {
                const auto response = HttpResponse::newRedirectionResponse("/api/users/me");
                processUser(req, response, *json);

                callback(response);
            }
            else
            {
                LOG_ERROR << "Failed to get user info";

                callback(HttpResponse::newHttpResponse(k400BadRequest, CT_NONE));
            }
        });
}

void LoginController::processUser(const HttpRequestPtr& req, const HttpResponsePtr& resp, const Json::Value& user)
{
    static auto mapper = orm::Mapper<Models::Users>(app().getDbClient());

    Models::Users userModel;

    try // Try to find the user by OAuth ID
    {
        // Throws if not fount
        userModel = mapper.findOne({ "oauth_id", user["id"].asString() });
    }
    catch(...) // The user is probably not in the database
    {
        userModel.setOauthId(user["id"].asString());
        userModel.setUsername(std::format("user_{}", utils::secureRandomString(8)));
        userModel.setDisplayName(user["name"].asString());
        userModel.setAvatarUrl(user["picture"].asString());

        mapper.insert(userModel);
    }

    const auto access = Utils::makeAccessToken(*userModel.getId(), *userModel.getUsername());
    const auto refresh = Utils::makeRefreshToken(*userModel.getId(), *userModel.getUsername());

    req->getSession()->insert("jwtAccess", access);

    Utils::saveRefreshToCookie(refresh, resp);
}

}
