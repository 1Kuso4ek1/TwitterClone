#pragma once
#include <drogon/HttpController.h>

using namespace drogon;

using Callback = std::function<void(const HttpResponsePtr&)>;

namespace Controllers
{

class LikesController final : public HttpController<LikesController>
{
public:
    static void likePost(const HttpRequestPtr& req, Callback&& callback, int postId);
    static void unlikePost(const HttpRequestPtr& req, Callback&& callback, int postId);

    static void getLikes(const HttpRequestPtr& req, Callback&& callback, int postId);

public:
    METHOD_LIST_BEGIN

        ADD_METHOD_TO(likePost, "/api/like/{post_id}", Post, "Filters::JwtFilter");
        ADD_METHOD_TO(unlikePost, "/api/like/{post_id}", Delete, "Filters::JwtFilter");

        ADD_METHOD_TO(getLikes, "/api/likes/{post_id}", Get);

    METHOD_LIST_END
};

}
