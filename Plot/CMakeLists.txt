cmake_minimum_required(VERSION 3.0.0)
project(Plot VERSION 0.1.0)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

if(CMAKE_VERSION VERSION_LESS "3.7.0")
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
endif()

find_package(Qt5 COMPONENTS Widgets Core Gui PrintSupport REQUIRED)

aux_source_directory(. DIR_SRCS)

include_directories(QCustomPlot)

add_subdirectory(QCustomPlot)

add_library(${PROJECT_NAME}
                ${DIR_SRCS}
                )

target_link_libraries(${PROJECT_NAME}
                ${MOC_Files}
                Qt5::Core
                Qt5::Widgets
                Qt5::PrintSupport
                )

# set(CPACK_PROJECT_NAME ${PROJECT_NAME})
# set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
# include(CPack)
