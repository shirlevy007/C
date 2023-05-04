cmake_minimum_required(VERSION 3.16)
project(ex3b_shirlevy C)

set(CMAKE_C_STANDARD 11)

include_directories(.)

add_executable(ex3b_shirlevy
        linked_list.c
        linked_list.h
        markov_chain.h
        markov_chain.c
        tweets_generator.c
#        snakes_and_ladders.c
        )
