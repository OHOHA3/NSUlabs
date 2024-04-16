cmake_minimum_required(VERSION 3.11)
project(factory)

set(CMAKE_CXX_STANDARD 14)
include_directories(.)

add_executable(factory
        GoodMan.cpp
        GoodMan.h
        Factory.cpp
        Factory.h
        IStrategy.h
        main.cpp
        BadMan.cpp
        BadMan.h
        Game.cpp
        Game.h
        CrazyMan.cpp
        CrazyMan.h
        MimicMan.cpp
        MimicMan.h
        TrickyMan.cpp
        TrickyMan.h
        WiseMan.cpp
        WiseMan.h
        Choice.h
        Mode.h)
