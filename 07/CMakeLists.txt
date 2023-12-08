cmake_minimum_required(VERSION 3.5.0)
project(Day07 VERSION 0.1.0 LANGUAGES CXX)

include(CTest)
enable_testing()



# Add the executable
add_executable(Day07 day07.cpp)

# CPack settings
set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
