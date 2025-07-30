#include <drogon/drogon.h>

using namespace drogon;

int main(const int argc, char* argv[])
{
    auto configPath = "../config/config-template.json";

    if(argc > 1)
        configPath = argv[1];

    app()
        .loadConfigFile(configPath)
        .registerPostHandlingAdvice(
            [](const HttpRequestPtr& req, const HttpResponsePtr& resp) {
                // Needed only for swagger, remove or change it to something safer in production
                resp->addHeader("Access-Control-Allow-Origin", "*");
                // Needed for WASM application to load properly
                resp->addHeader("Cross-Origin-Opener-Policy", "same-origin");
                resp->addHeader("Cross-Origin-Embedder-Policy", "require-corp");
            }
        )
        .run();
}
