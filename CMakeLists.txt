cmake_minimum_required(VERSION 2.6)
project(marcel2)

#Compiler flags
set(CMAKE_CXX_FLAGS "-std=c++11 -O3 -Wno-reorder -Wno-deprecated")

message("CXX compiler flags are : ${CMAKE_CXX_FLAGS}")

#Find the ROOT setup package
find_package(ROOT REQUIRED)
include(${ROOT_USE_FILE})

#List include directory
include_directories(${LD_LIBRARY_PATH}
        ${PROJECT_SOURCE_DIR}
        ${ROOT_INCLUDE_DIR}
        ${MYSQL_INCLUDE_DIR})

#Get recursively source and header files
file(GLOB sources ${PROJECT_SOURCE_DIR}/*.C)
file(GLOB sources2 ${PROJECT_SOURCE_DIR}/S*.cpp)

file(GLOB headers ${PROJECT_SOURCE_DIR}/*.h)

add_executable(marcel2 main.cpp ${sources} ${sources2} ${headers})
target_link_libraries(marcel2 ${ROOT_LIBRARIES})