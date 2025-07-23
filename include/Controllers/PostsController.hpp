#pragma once
#include <drogon/HttpController.h>

using namespace drogon;

using Callback = std::function<void(const HttpResponsePtr&)>;

namespace Controllers
{

class PostsController final : public HttpController<PostsController>
{
public:
    static void createPost(const HttpRequestPtr& req, Callback&& callback);
    static void deletePost(const HttpRequestPtr& req, Callback&& callback, int postId);

    static void getFeed(const HttpRequestPtr& req, Callback&& callback, int limit, int offset);
    static void getUserPosts(const HttpRequestPtr& req, Callback&& callback, int userId, int limit, int offset);

public:
    METHOD_LIST_BEGIN

        ADD_METHOD_TO(createPost, "/api/posts", Post, "Filters::JwtFilter");
        ADD_METHOD_TO(deletePost, "/api/posts/{post_id}", Delete, "Filters::JwtFilter");

        ADD_METHOD_TO(getFeed, "/api/posts?limit={limit}&offset={offset}", Get, "Filters::JwtFilter");
        ADD_METHOD_TO(getUserPosts, "/api/posts/user/{user_id}?limit={limit}&offset={offset}", Get);

    METHOD_LIST_END
};

}
