set(TEST_PROJECT_NAME BigIntTest)
set(LIBRARY_TESTS_SOURCE BigIntTest.cpp ../src/BigInt.cpp)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0 -fprofile-arcs -ftest-coverage")
set(CMAKE_CXX_OUTPUT_EXTENSION_REPLACE ON)

project(${TEST_PROJECT_NAME} CXX)

add_custom_target(gcov
    COMMAND mkdir -p gcoverage
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    )

add_custom_command(TARGET gcov
    COMMAND gcov -b ${CMAKE_SOURCE_DIR}/lib/bigint/src/*.cpp -o ${CMAKE_BINARY_DIR}/gcoverage
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/gcoverage
    )

add_dependencies(gcov BigIntTest)

include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/58d77fa8070e8cec2dc1ed015d66b454c8d78850.zip
)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(${TEST_PROJECT_NAME} ${LIBRARY_TESTS_SOURCE})

target_link_libraries(${TEST_PROJECT_NAME} gtest gtest_main ${LIBRARY_NAME} )

add_test(${TEST_PROJECT_NAME} ${TEST_PROJECT_NAME})