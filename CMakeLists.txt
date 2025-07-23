cmake_minimum_required(VERSION 3.10)
project(backend)

# For drogon
set(BUILD_EXAMPLES OFF)
set(BUILD_TESTING OFF)

# For JWT
set(JWT_BUILD_EXAMPLES OFF)

set(OUTPUT_DIR ${CMAKE_BINARY_DIR})

set(
    BACKEND_HEADERS

    include/Controllers/FollowsController.hpp
    include/Controllers/LikesController.hpp
    include/Controllers/LoginController.hpp
    include/Controllers/PostsController.hpp
    include/Controllers/StaticController.hpp
    include/Controllers/UsersController.hpp

    include/Filters/JwtFilter.hpp

    include/Models/Follows.hpp
    include/Models/Likes.hpp
    include/Models/Posts.hpp
    include/Models/Users.hpp

    include/Utils/JwtUtils.hpp
    include/Utils/Singleton.hpp
)

set(
    BACKEND_SOURCES

    src/Controllers/FollowsController.cpp
    src/Controllers/LikesController.cpp
    src/Controllers/LoginController.cpp
    src/Controllers/PostsController.cpp
    src/Controllers/StaticController.cpp
    src/Controllers/UsersController.cpp

    src/Filters/JwtFilter.cpp

    src/Models/Follows.cpp
    src/Models/Likes.cpp
    src/Models/Posts.cpp
    src/Models/Users.cpp

    src/main.cpp
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(libraries/drogon)
add_subdirectory(libraries/jwt-cpp)

add_executable(
    backend

    ${BACKEND_HEADERS}
    ${BACKEND_SOURCES}
)


target_link_libraries(backend drogon jwt-cpp)

target_include_directories(backend PUBLIC ${CMAKE_SOURCE_DIR}/include/)

