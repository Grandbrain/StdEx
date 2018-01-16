#[[ Set CMake minimum required version ]]
cmake_minimum_required(VERSION 3.6)

#[[ Set C++ standard version ]]
set(CMAKE_CXX_STANDARD 14)

#[[ Set header files ]]
set(HEADER_FILES
        ../Source/buffer.h
        ../Source/delegate.h
        ../Source/pimpl.h
        ../Source/range_access.h)

#[[ Set test source files ]]
set(TEST_SOURCE_FILES
        testBuffer.cpp
        testDelegate.cpp
        testPimpl.cpp
        testRangeAccess.cpp)

#[[ Set test entry point ]]
set(TEST_ENTRY_POINT main.cpp)

#[[ Add executable target ]]
add_executable(stdex-test
        ${HEADER_FILES}
        ${TEST_SOURCE_FILES}
        ${TEST_ENTRY_POINT})

#[[ Add include directories ]]
target_include_directories(stdex-test PUBLIC ../External/catch/include)
target_include_directories(stdex-test PUBLIC ../Source)
