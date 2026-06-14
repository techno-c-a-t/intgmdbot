cmake_minimum_required(VERSION 3.20)
project(intgmdbot LANGUAGES CXX)

# Require C++20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Compiler flags for strictness and warnings
if(MSVC)
    add_compile_options(/W4 /WX)
else()
    add_compile_options(-Wall -Wextra -Wpedantic -Werror)
endif()

# Find OpenSSL (required for HTTPS in cpp-httplib)
find_package(OpenSSL REQUIRED)

# Find nlohmann_json (header-only JSON library)
find_package(nlohmann_json REQUIRED)

# Fetch cpp-httplib dependency
include(FetchContent)
set(CPPHTTPLIB_OPENSSL_SUPPORT ON CACHE BOOL "" FORCE)

FetchContent_Declare(
    httplib
    GIT_REPOSITORY https://github.com/yhirose/cpp-httplib
    GIT_TAG        v0.16.0
)
FetchContent_MakeAvailable(httplib)

# Create the executable with files organized in src/
add_executable(intgmdbot 
    src/main.cpp
    src/config.cpp
    src/logger.cpp
    src/telegram_client.cpp
    src/markdown_ast.cpp
    src/markdown_parser.cpp
    src/message_accumulator.cpp
)

# Tell compiler where to find headers
target_include_directories(intgmdbot PRIVATE include)

# Compile definitions
target_compile_definitions(intgmdbot PRIVATE CPPHTTPLIB_OPENSSL_SUPPORT)

# Link OpenSSL and libraries
find_package(Threads REQUIRED)

target_link_libraries(intgmdbot
    PRIVATE
        OpenSSL::SSL
        OpenSSL::Crypto
        nlohmann_json::nlohmann_json
        httplib
        Threads::Threads
)
