cmake_minimum_required(VERSION 3.16)
project(lab3)

set(CMAKE_CXX_STANDARD 14)

add_executable(sound_processor main.cpp
        Factory.h
        Factory.cpp
        IConverter.h
        SoundProcessor.cpp
        SoundProcessor.h
        Pars.cpp
        Pars.h
        WAVContainer.h
        Config.h
        Mix.h
        Mix.cpp
        Reverse.h
        Reverse.cpp
        Mute.h
        Mute.cpp)

