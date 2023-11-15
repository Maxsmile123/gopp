set(EXAMPLE_TARGET gopp_example_condvar)

add_executable(${EXAMPLE_TARGET} main.cpp)
target_link_libraries(${EXAMPLE_TARGET} gopp)
