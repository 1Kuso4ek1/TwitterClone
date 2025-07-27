#include "Controllers/FollowsController.hpp"

#include "Models/Follows.hpp"
#include "Models/Users.hpp"

namespace Models = drogon_model::TwitterClone;

namespace Controllers
{

void FollowsController::followUser(const HttpRequestPtr& req, Callback&& callback, const int userId)
{
    static auto mapper = orm::Mapper<Models::Follows>(app().getDbClient());

    try
    {
        const auto selfId = std::stoi(req->getParameter("user_id"));

        Models::Follows follow;
        follow.setFollowerId(selfId);
        follow.setFolloweeId(userId);

        mapper.insert(follow);

        callback(HttpResponse::newHttpResponse(k201Created, CT_NONE));
    }
    catch(...)
    {
        LOG_ERROR << "Failed to follow user: " << userId;

        callback(HttpResponse::newHttpResponse(k500InternalServerError, CT_NONE));
    }
}

void FollowsController::unfollowUser(const HttpRequestPtr& req, Callback&& callback, const int userId)
{
    static auto mapper = orm::Mapper<Models::Follows>(app().getDbClient());

    try
    {
        const auto selfId = std::stoi(req->getParameter("user_id"));

        mapper.deleteBy(
            orm::Criteria{ "follower_id", selfId }
            && orm::Criteria{ "followee_id", userId }
        );

        callback(HttpResponse::newHttpResponse(k204NoContent, CT_NONE));
    }
    catch(...)
    {
        LOG_ERROR << "Failed to unfollow user: " << userId;

        callback(HttpResponse::newHttpResponse(k500InternalServerError, CT_NONE));
    }
}

void FollowsController::getFollowers(const HttpRequestPtr& req, Callback&& callback, const int userId)
{
    static auto mapper = orm::Mapper<Models::Follows>(app().getDbClient());

    try
    {
        const auto followers = mapper.findBy({ "followee_id", userId });

        Json::Value json;
        for(const auto& i : followers)
            json.append(i.getFollower(app().getDbClient()).toJson());

        callback(HttpResponse::newHttpJsonResponse(json));
    }
    catch(...)
    {
        LOG_ERROR << "Failed to get followers for user: " << userId;

        callback(HttpResponse::newHttpResponse(k500InternalServerError, CT_NONE));
    }
}

void FollowsController::getFollowing(const HttpRequestPtr& req, Callback&& callback, const int userId)
{
    static auto mapper = orm::Mapper<Models::Follows>(app().getDbClient());

    try
    {
        const auto following = mapper.findBy({ "follower_id", userId });

        Json::Value json;
        for(const auto& i : following)
            json.append(i.getFollowee(app().getDbClient()).toJson());

        callback(HttpResponse::newHttpJsonResponse(json));
    }
    catch(...)
    {
        LOG_ERROR << "Failed to get followees for user: " << userId;

        callback(HttpResponse::newHttpResponse(k500InternalServerError, CT_NONE));
    }
}

}
