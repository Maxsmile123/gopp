cmake_minimum_required(VERSION 3.14)
project(gopp)

include(cmake/Logging.cmake)

include(cmake/CheckCompiler.cmake)
include(cmake/CompileOptions.cmake)
include(cmake/Sanitize.cmake)
include(cmake/PrintDiagnostics.cmake)

option(GOPP_TESTS "Build GOPP tests" OFF)
option(GOPP_EXAMPLES "Build GOPP examples" ON)

#add_subdirectory(third_party)

add_subdirectory(gopp)

if(GOPP_TESTS)
    add_subdirectory(tests)
endif()


if(GOPP_EXAMPLES)
    add_subdirectory(examples)
endif()
