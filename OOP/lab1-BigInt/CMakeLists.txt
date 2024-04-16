cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 17)
set(PROJECT_NAME lab1-BigInt)
set(MAIN_DIR main)
set(MAIN ${MAIN_DIR}/main.cpp)
set(LIBRARY_DIR library )
set(LIBRARY BigInt )

project(${PROJECT_NAME} CXX)

add_executable(${PROJECT_NAME} ${MAIN})

add_subdirectory(library/BigInt)

target_link_libraries(${PROJECT_NAME} PUBLIC BigIntLib)