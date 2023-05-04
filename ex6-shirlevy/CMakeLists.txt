cmake_minimum_required(VERSION 3.16)
project(ex6_shirlevy)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(ex6_shirlevy
        Dictionary.hpp
        test_ex6.cpp
#        tests_ex6_suchetzky.cpp
#        HashMap.hpp HushMap.cpp Dictionary.cpp
        )
