cmake_minimum_required(VERSION 3.15)
project(longnumber)

set(CMAKE_CXX_STANDARD 17)

# 禁用 SSL 验证（解决证书问题）
set(CMAKE_TLS_VERIFY OFF)

# 自动下载 Google Test
include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
    TLS_VERIFY OFF  # 下载时也禁用 SSL
)
FetchContent_MakeAvailable(googletest)

# 直接构建测试可执行文件
add_executable(run_tests
    src/LongNumber.cpp
    test/test_long_number.cpp
)

# 添加头文件路径
target_include_directories(run_tests PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# 链接 Google Test（Windows下不需要pthread！）
target_link_libraries(run_tests PRIVATE
    gtest_main
)

# Windows下禁用pthread
if(WIN32)
    target_compile_definitions(run_tests PRIVATE GTEST_HAS_PTHREAD=0)
endif()

add_test(NAME run_tests COMMAND run_tests)