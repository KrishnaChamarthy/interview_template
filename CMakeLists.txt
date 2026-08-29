cmake_minimum_required(VERSION 3.16)
project(StripeInterviewPrep CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

# Fetch JSON library
FetchContent_Declare(
    nlohmann_json
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG v3.11.3
)

# Fetch HTTP library
FetchContent_Declare(
    cpr
    GIT_REPOSITORY https://github.com/libcpr/cpr.git
    GIT_TAG 1.10.5
)

FetchContent_MakeAvailable(nlohmann_json cpr)

add_executable(app src/main.cpp)
target_link_libraries(app PRIVATE nlohmann_json::nlohmann_json cpr::cpr)
