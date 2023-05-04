cmake_minimum_required(VERSION 3.16)
project(ex5_shirlevy)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(ex5_shirlevy
        tests.cpp
#        main.cpp
        Movie.cpp
        Movie.h
#        print_main.cpp
        RecommenderSystem.h
        RecommenderSystemLoader.h
        RSUser.cpp
        RSUser.h
        RSUsersLoader.h
        RecommenderSystem.cpp
        RecommenderSystemLoader.cpp
        RSUsersLoader.cpp)
