project(UbexCpp)
cmake_minimum_required(VERSION 2.8)
aux_source_directory(. SRC_LIST)
file(GLOB HEADER_FILES "include/*.hpp")
file(GLOB HEADER_FILES ${HEADER_FILES} "tests/test_utils/*.hpp" "tests/value_test/*.cpp")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -std=c++14 -Wall -Wextra -Werror -Wno-unused-variable -g")
add_subdirectory(include)
include_directories(include)
add_subdirectory(tests)
add_subdirectory(src)
add_executable(${PROJECT_NAME} ${SRC_LIST})
target_link_libraries(${PROJECT_NAME} UbexCpp_test_lib)
target_link_libraries(${PROJECT_NAME} UbexCpp_lib)
target_link_libraries(${PROJECT_NAME} cppunit)
