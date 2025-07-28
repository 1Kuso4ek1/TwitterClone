#include "Filters/JwtFilter.hpp"

#include "Utils/JwtUtils.hpp"

namespace Filters
{

void JwtFilter::doFilter(
    const HttpRequestPtr& req,
    FilterCallback&& fcb,
    FilterChainCallback&& fccb
)
{
    static auto accessSecret = app().getCustomConfig()["jwt"]["access_secret"].asString();

    const auto authHeader = req->getHeader("Authorization");
    const auto sessionJwt = req->getSession()->get<std::string>("jwtAccess");

    auto token =
        authHeader.empty() || authHeader.size() < 7
        ? sessionJwt : authHeader.substr(7);

    if(authHeader.empty() && sessionJwt.empty())
    {
        if(tryRefresh(req))
            token = req->getSession()->get<std::string>("jwtAccess");
        else
        {
            fcb(HttpResponse::newRedirectionResponse("/login"));
            return;
        }
    }

    try
    {
        const auto [userId, username] = Utils::verify(token, accessSecret);

        req->setParameter("user_id", userId);
        req->setParameter("username", username);

        fccb();
    }
    catch(...)
    {
        fcb(HttpResponse::newHttpResponse(k401Unauthorized, CT_NONE));
    }
}

bool JwtFilter::tryRefresh(const HttpRequestPtr& req)
{
    static auto refreshSecret = app().getCustomConfig()["jwt"]["refresh_secret"].asString();

    const auto refreshToken = req->getCookie("refreshToken");

    if(refreshToken.empty())
        return false;

    try
    {
        const auto [userId, username] = Utils::verify(refreshToken, refreshSecret);

        req->getSession()->insert(
            "jwtAccess",
            Utils::makeAccessToken(std::stoi(userId), username)
        );

        return true;
    }
    catch(...)
    {
        return false;
    }
}

}
