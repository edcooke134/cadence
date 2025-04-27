cmake_minimum_required(VERSION 3.22)

project(OrthHitDetection VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(OrthHitDetection 
	main.cpp
        int2.h
        int2.cpp
        impl.h
        impl.cpp
)

set_target_properties(OrthHitDetection PROPERTIES
        WIN32_EXECUTABLE ON
        MACOSX_BUNDLE ON
)
