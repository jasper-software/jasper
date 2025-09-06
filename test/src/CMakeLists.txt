cmake_minimum_required(VERSION 3.20)

project(test_app LANGUAGES C)

find_package(JasPer CONFIG REQUIRED)

add_executable(app app.c)
target_link_libraries(app JasPer::libjasper)

install(CODE "message(STATUS \"Dummy install target executed.\")")
