cmake_minimum_required(VERSION 3.0.0)
project(QLiteMVVM VERSION 0.1.0)

# include(CTest)
# enable_testing()

# include text specifications
include(Specifications.txt)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(CMAKE_VERSION VERSION_LESS "3.7.0")
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
endif()

add_definitions(-w)

add_subdirectory(QLiteMVVM)
add_subdirectory(Plot)
add_subdirectory(samples)
add_subdirectory(ProcessWorkbench)
# set(CPACK_PROJECT_NAME ${PROJECT_NAME})
# set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
# include(CPack)
