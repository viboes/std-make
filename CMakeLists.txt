cmake_minimum_required(VERSION 3.0)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

project(Jasel CXX)

find_package(Boost)

if (NOT BOOST_FOUND)
    message(WARNING "Boost was not found; Forcing directory.")
    set(Boost_INCLUDE_DIRS "$ENV{BOOST_ROOT}")
    message("${Boost_INCLUDE_DIRS}")
    
endif()

find_package(Git)

include_directories(include)
include_directories(${Boost_INCLUDE_DIRS}) 

enable_testing()
include(CTest)
include(CheckCXXCompilerFlag)
include(CMakeDependentOption)


option(JASEL_MODULES "Enables Clang Modules." OFF)

find_program(MEMORYCHECK_COMMAND valgrind)
if(MEMORYCHECK_COMMAND)
  message("Valgrind: ${MEMORYCHECK_COMMAND}")
  set(MEMORYCHECK_COMMAND_OPTIONS "--trace-children=yes --leak-check=full")
else()
  message("Valgrind not found")
endif()

macro(jasel_append_flag testname flag)
  check_cxx_compiler_flag(${flag} ${testname})
  if (${testname})
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${flag}")
  endif()
endmacro()

# Select C++ standard to be used for compiling the tests,
# for example: 11, 14, 17, 1z, 1y, ...
#
if(NOT JASEL_CXX_STD)
  # Defaults to C++11 if not set:
  set(JASEL_CXX_STD 11)
endif()

if("x${CMAKE_CXX_COMPILER_ID}" MATCHES "x.*Clang")
  # Clang/C2 will blow up with various parts of the standard library
  # if compiling with -std less than c++14.
  if(("x${CMAKE_CXX_SIMULATE_ID}" STREQUAL "xMSVC") AND (JASEL_CXX_STD EQUAL 11))
    set(CMAKE_CXX_STANDARD 14)
    set(JASEL_CXX_STD 14)
  endif()
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++${JASEL_CXX_STD} -ftemplate-backtrace-limit=0")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Weverything -Werror -pedantic-errors")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-c++98-compat -Wno-c++98-compat-pedantic")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-weak-vtables -Wno-padded")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-missing-prototypes -Wno-missing-variable-declarations")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-shadow -Wno-old-style-cast")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-documentation -Wno-documentation-unknown-command")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-reserved-id-macro -Wno-disabled-macro-expansion -Wno-missing-noreturn")
  
  if(CMAKE_SYSTEM_NAME MATCHES "Darwin")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-global-constructors -Wno-exit-time-destructors")
  endif()
  if (JASEL_MODULES)
    if(NOT JASEL_LIBCXX_MODULE)
      set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fmodules -fmodule-map-file=${PROJECT_SOURCE_DIR}/include/module.modulemap -fmodules-cache-path=${PROJECT_BINARY_DIR}/module.cache")
    else()
      set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fmodules -fmodule-map-file=${PROJECT_SOURCE_DIR}/include/module.modulemap -fmodules-cache-path=${PROJECT_BINARY_DIR}/module.cache -fno-implicit-module-maps -fmodule-map-file=${JASEL_LIBCXX_MODULE}")
    endif()
  endif()
  set(CMAKE_CXX_FLAGS_DEBUG "-O0 -fno-inline -g3 -fstack-protector-all")
  set(CMAKE_CXX_FLAGS_RELEASE "-Ofast -g0 -DNDEBUG")
elseif(CMAKE_COMPILER_IS_GNUCXX)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++${JASEL_CXX_STD} -ftemplate-backtrace-limit=0")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic -Werror -pedantic-errors")
  if(CMAKE_CXX_COMPILER_VERSION VERSION_LESS "6.0")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-strict-overflow")
    if(CMAKE_CXX_COMPILER_VERSION VERSION_LESS "5.0")
      set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-missing-field-initializers")
    endif()
  endif()
  set(CMAKE_CXX_FLAGS_DEBUG "-O0 -fno-inline -g3 -fstack-protector-all")
  set(CMAKE_CXX_FLAGS_RELEASE "-Ofast -g0 -DNDEBUG")
# else()
#   message(FATAL_ERROR "Unknown compiler. Good luck!")
endif()

jasel_append_flag(JASEL_HAS_MARCH_NATIVE "-march=native")
jasel_append_flag(JASEL_HAS_MTUNE_NATIVE "-mtune=native")

# Test all headers
# file(GLOB_RECURSE JASEL_PUBLIC_HEADERS
#                   RELATIVE "${CMAKE_SOURCE_DIR}/include"
#                   "${CMAKE_SOURCE_DIR}/include/*.hpp")
# include(TestHeaders)
# if(JASEL_NO_HEADER_CHECK)
#   add_custom_target(headers)
# else()
#   add_custom_target(headers ALL)
# endif()
# generate_standalone_header_tests(EXCLUDE_FROM_ALL MASTER_TARGET headers HEADERS ${JASEL_PUBLIC_HEADERS})

# Grab the jasel version numbers:
include(${CMAKE_SOURCE_DIR}/Version.cmake)
set(JASEL_VERSION ${JASEL_MAJOR}.${JASEL_MINOR}.${JASEL_PATCHLEVEL})

# Try to build a new version.hpp
configure_file(version.hpp.in include/jasel/version.hpp @ONLY)
file(STRINGS ${CMAKE_CURRENT_BINARY_DIR}/include/jasel/version.hpp JASEL_OLD_VERSION_HPP)
file(STRINGS ${CMAKE_CURRENT_SOURCE_DIR}/include/jasel/version.hpp JASEL_NEW_VERSION_HPP)

# If the new version.hpp is materially different from the one in the source
# directory, update it, amend the most recent commit, and tag the commit.
# if(NOT JASEL_NEW_VERSION_HPP STREQUAL JASEL_OLD_VERSION_HPP)
#   # Check that Version.cmake is the only changed file:
#   execute_process(
#     COMMAND ${GIT_EXECUTABLE} -C "${CMAKE_CURRENT_SOURCE_DIR}" status -s -uno
#     OUTPUT_VARIABLE JASEL_GIT_STATUS
#     OUTPUT_STRIP_TRAILING_WHITESPACE
#   )
#   if (NOT "x${JASEL_GIT_STATUS}" STREQUAL "x M Version.cmake")
#     #message(FATAL_ERROR "${GIT_EXECUTABLE} -C ${CMAKE_CURRENT_SOURCE_DIR} status -s -uno")
#     message(FATAL_ERROR "Cannot update version.hpp: jasel source directory has a dirty status")
#   endif()
#   file(
#     COPY ${CMAKE_CURRENT_BINARY_DIR}/include/jasel/version.hpp
#     DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/include/jasel
#   )
#   execute_process(
#     COMMAND ${GIT_EXECUTABLE} -C "${CMAKE_CURRENT_SOURCE_DIR}" add -u
#   )
#   execute_process(
#     COMMAND ${GIT_EXECUTABLE} -C "${CMAKE_CURRENT_SOURCE_DIR}" commit --amend --no-edit
#   )
#   execute_process(
#     COMMAND ${GIT_EXECUTABLE} -C "${CMAKE_CURRENT_SOURCE_DIR}" tag -f -a "${JASEL_VERSION}" -m "${JASEL_VERSION}"
#   )
# endif()

add_subdirectory(doc)
add_subdirectory(test)
#add_subdirectory(example)
#add_subdirectory(perf)

install(DIRECTORY include/ DESTINATION include
        FILES_MATCHING PATTERN "*.hpp")
