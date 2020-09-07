#-------------------------------------------------------------------------------
#                               Base settings.
#-------------------------------------------------------------------------------

# Set CMake minimum required version.
cmake_minimum_required(VERSION 3.0)

# Set project information.
project(StdEx VERSION 2.0.0 LANGUAGES CXX)

# Set C++ standard version.
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


#-------------------------------------------------------------------------------
#                            Macros and functions.
#-------------------------------------------------------------------------------

# Finds platform (OS) name.
function(find_platform_name RESULT)
    string(TOLOWER ${CMAKE_SYSTEM_NAME} PLATFORM_NAME)
    set(${RESULT} ${PLATFORM_NAME} PARENT_SCOPE)
endfunction()

# Finds host architecture type.
function(find_architecture_type RESULT)
    set(ARCHITECTURE_TYPE ${CMAKE_SYSTEM_PROCESSOR})

    if (MSVC64 OR MINGW64)
        set(X86_64 1)
    elseif (MINGW OR (MSVC AND NOT CMAKE_CROSSCOMPILING))
        set(X86 1)
    elseif (${ARCHITECTURE_TYPE} MATCHES "amd64.*|x86_64.*|AMD64.*")
        set(X86_64 1)
    elseif (${ARCHITECTURE_TYPE} MATCHES "i686.*|i386.*|x86.*|amd64.*|AMD64.*")
        set(X86 1)
    elseif (${ARCHITECTURE_TYPE} MATCHES "^(aarch64.*|AARCH64.*)")
        set(AARCH64 1)
    elseif (${ARCHITECTURE_TYPE} MATCHES "^(arm.*|ARM.*)")
        set(ARM 1)
    elseif (${ARCHITECTURE_TYPE} MATCHES "^(powerpc|ppc)64le")
        set(PPC64LE 1)
    elseif (${ARCHITECTURE_TYPE} MATCHES "^(powerpc|ppc)64")
        set(PPC64 1)
    elseif (${ARCHITECTURE_TYPE} MATCHES "^(mips.*|MIPS.*)")
        set(MIPS 1)
    endif ()

    if (${CMAKE_SIZEOF_VOID_P} EQUAL 4)
        if (X86_64)
            unset(X86_64)
            set(X86)
        endif ()
        if (AARCH64)
            unset(AARCH64)
            set(ARM)
        endif ()
    endif ()

    if (X86)
        string(TOLOWER "X86" ARCHITECTURE_TYPE)
    elseif (X86_64)
        string(TOLOWER "X86_64" ARCHITECTURE_TYPE)
    elseif (ARM)
        string(TOLOWER "ARM" ARCHITECTURE_TYPE)
    elseif (AARCH64)
        string(TOLOWER "AARCH64" ARCHITECTURE_TYPE)
    elseif (PPC64)
        string(TOLOWER "PPC64" ARCHITECTURE_TYPE)
    elseif (PPC64LE)
        string(TOLOWER "PPC64LE" ARCHITECTURE_TYPE)
    elseif (MIPS)
        string(TOLOWER "MIPS" ARCHITECTURE_TYPE)
    endif ()

    set(${RESULT} ${ARCHITECTURE_TYPE} PARENT_SCOPE)
endfunction()

# Finds target build type.
function(find_build_type RESULT)
    string(TOLOWER ${CMAKE_BUILD_TYPE} BUILD_TYPE)
    set(${RESULT} ${BUILD_TYPE} PARENT_SCOPE)
endfunction()

# Finds dependency directory.
function(find_dependency_directory RESULT PATTERN PARENT)
    file(GLOB CHILDREN RELATIVE ${PARENT} ${PARENT}/*)
    foreach (CHILD ${CHILDREN})
        if (IS_DIRECTORY ${PARENT}/${CHILD})
            if (${CHILD} MATCHES ${PATTERN})
                string(TOLOWER ${CHILD} CHILD)
                set(${RESULT} ${CHILD} PARENT_SCOPE)
                return()
            endif ()
        endif ()
    endforeach ()
endfunction()

# Finds absolute dependency library path.
function(find_dependency_path RESULT PATTERN PARENT)
    find_dependency_directory(DEPENDENCY_DIRECTORY ${PATTERN} ${PARENT})
    find_platform_name(PLATFORM_NAME)
    find_architecture_type(ARCHITECTURE_TYPE)
    find_build_type(BUILD_TYPE)
    string(
        CONCAT PATH
        "${PARENT}/${DEPENDENCY_DIRECTORY}/lib/"
        "${PLATFORM_NAME}/${ARCHITECTURE_TYPE}/${BUILD_TYPE}")

    if (IS_DIRECTORY ${PATH})
        set(${RESULT} ${PATH} PARENT_SCOPE)
    else ()
        message(
            FATAL_ERROR
            "The project doesn't support this platform or architecture."
        )
    endif ()
endfunction()

# Configures build type options.
function(configure_build_type_options)
    if (NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
        set(
            CMAKE_BUILD_TYPE "Debug" CACHE
            STRING "Choose the type of build." FORCE
        )
        set_property(
            CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS
            "Debug"
            "Release"
            "MinSizeRel"
            "RelWithDebInfo"
        )
    endif ()
endfunction()

# Configures test build options.
function(configure_test_build_options)
    if (NOT BUILD_TESTS)
        option(BUILD_TESTS "Choose whether to build tests." ON)
    endif ()
endfunction()


#-------------------------------------------------------------------------------
#                        Build configuration settings.
#-------------------------------------------------------------------------------

# Configure build type options.
configure_build_type_options()

# Configure test build options.
configure_test_build_options()


#-------------------------------------------------------------------------------
#                          Build variables settings.
#-------------------------------------------------------------------------------

# Set test executable target name.
set(TEST_TARGET "stdex-test")

# Set path to external libraries.
set(EXTERNAL_PATH ${CMAKE_CURRENT_SOURCE_DIR}/External)

# Set path to source files.
set(SOURCE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/Source)

# Set path to test files.
set(TEST_PATH ${CMAKE_CURRENT_SOURCE_DIR}/Test)

# Set Catch test framework directory.
find_dependency_directory(CATCH_DIRECTORY "catch-*" ${EXTERNAL_PATH})


#-------------------------------------------------------------------------------
#                           Project files settings.
#-------------------------------------------------------------------------------

# Set source header files.
set(
    SOURCE_HEADER_FILES
    ${SOURCE_PATH}/buffer.hpp
    ${SOURCE_PATH}/delegate.hpp
    ${SOURCE_PATH}/pimpl.hpp
    ${SOURCE_PATH}/string_utils.hpp
)

# Set source source files.
set(
    SOURCE_SOURCE_FILES
)

# Set test header files.
set(
    TEST_HEADER_FILES
)

# Set test source files.
set(
    TEST_SOURCE_FILES
    ${TEST_PATH}/test_buffer.cpp
    ${TEST_PATH}/test_delegate.cpp
    ${TEST_PATH}/test_pimpl.cpp
    ${TEST_PATH}/test_string_utils.cpp
    ${TEST_PATH}/main.cpp
)

# Set source files to list.
list(
    APPEND SOURCE_FILES
    ${SOURCE_HEADER_FILES}
    ${SOURCE_SOURCE_FILES}
)

# Set test files to list.
list(
    APPEND TEST_FILES
    ${TEST_HEADER_FILES}
    ${TEST_SOURCE_FILES}
)


#-------------------------------------------------------------------------------
#                       Include directories settings.
#-------------------------------------------------------------------------------

# Set source include directories to list.
list(
    APPEND SOURCE_INCLUDE_DIRECTORIES
    ${SOURCE_PATH}
)

# Set test include directories to list.
list(
    APPEND TEST_INCLUDE_DIRECTORIES
    ${SOURCE_INCLUDE_DIRECTORIES}
    ${EXTERNAL_PATH}/${CATCH_DIRECTORY}/include
)


#-------------------------------------------------------------------------------
#                            Test target settings.
#-------------------------------------------------------------------------------

# Add test executable target.
add_executable(
    ${TEST_TARGET}
    ${SOURCE_FILES} ${TEST_FILES}
)

# Add test include directories.
target_include_directories(
    ${TEST_TARGET}
    PUBLIC ${TEST_INCLUDE_DIRECTORIES}
)
