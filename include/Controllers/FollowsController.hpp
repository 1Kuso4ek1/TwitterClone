#pragma once
#include <drogon/HttpController.h>

using namespace drogon;

using Callback = std::function<void(const HttpResponsePtr&)>;

namespace Controllers
{

class FollowsController final : public HttpController<FollowsController>
{
public:
    static void followUser(const HttpRequestPtr& req, Callback&& callback, int userId);
    static void unfollowUser(const HttpRequestPtr& req, Callback&& callback, int userId);

    static void getFollowers(const HttpRequestPtr& req, Callback&& callback, int userId);
    static void getFollowing(const HttpRequestPtr& req, Callback&& callback, int userId);

public:
    METHOD_LIST_BEGIN

        ADD_METHOD_TO(followUser, "/api/follow/{user_id}", Post, "Filters::JwtFilter");
        ADD_METHOD_TO(unfollowUser, "/api/follow/{user_id}", Delete, "Filters::JwtFilter");

        ADD_METHOD_TO(getFollowers, "/api/followers/{user_id}", Get);
        ADD_METHOD_TO(getFollowing, "/api/following/{user_id}", Get);

    METHOD_LIST_END
};

}
