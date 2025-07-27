#include "Controllers/LikesController.hpp"

#include "Models/Likes.hpp"
#include "Models/Users.hpp"

namespace Models = drogon_model::TwitterClone;

namespace Controllers
{

void LikesController::likePost(const HttpRequestPtr& req, Callback&& callback, const int postId)
{
    static auto mapper = orm::Mapper<Models::Likes>(app().getDbClient());

    try
    {
        const auto selfId = std::stoi(req->getParameter("user_id"));

        Models::Likes like;
        like.setUserId(selfId);
        like.setPostId(postId);

        mapper.insert(like);

        callback(HttpResponse::newHttpResponse(k201Created, CT_NONE));
    }
    catch(...)
    {
        LOG_ERROR << "Failed to like post: " << postId;

        callback(HttpResponse::newHttpResponse(k500InternalServerError, CT_NONE));
    }
}

void LikesController::unlikePost(const HttpRequestPtr& req, Callback&& callback, const int postId)
{
    static auto mapper = orm::Mapper<Models::Likes>(app().getDbClient());

    try
    {
        const auto selfId = std::stoi(req->getParameter("user_id"));

        mapper.deleteBy(
            orm::Criteria{ "user_id", selfId }
            && orm::Criteria{ "post_id", postId }
        );

        callback(HttpResponse::newHttpResponse(k204NoContent, CT_NONE));
    }
    catch(...)
    {
        LOG_ERROR << "Failed to unlike post: " << postId;

        callback(HttpResponse::newHttpResponse(k500InternalServerError, CT_NONE));
    }
}

void LikesController::getLikes(const HttpRequestPtr& req, Callback&& callback, const int postId)
{
    static auto mapper = orm::Mapper<Models::Likes>(app().getDbClient());

    try
    {
        const auto likes = mapper.findBy({ "post_id", postId });

        Json::Value json;
        for(const auto& i : likes)
            json.append(i.getUser(app().getDbClient()).toJson());

        callback(HttpResponse::newHttpJsonResponse(json));
    }
    catch(...)
    {
        LOG_ERROR << "Failed to get likes for post: " << postId;

        callback(HttpResponse::newHttpResponse(k500InternalServerError, CT_NONE));
    }
}

}
