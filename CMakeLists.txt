project(UbexCpp)
cmake_minimum_required(VERSION 2.8)
aux_source_directory(. SRC_LIST)
aux_source_directory(include/ SRC_LIST)
add_executable(${PROJECT_NAME} ${SRC_LIST})

#set(SRC_LIST ${SRC_LIST})
MESSAGE( SRC_LIST  " : ${SRC_LIST} " )

target_link_libraries(${PROJECT_NAME} cppunit)

ADD_DEFINITIONS(
    -std=c++1y
    -Wall
    -g
)

