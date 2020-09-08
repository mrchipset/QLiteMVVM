cmake_minimum_required(VERSION 3.0.0)
project(QCustomPlot VERSION 0.1.0)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

if(CMAKE_VERSION VERSION_LESS "3.7.0")
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
endif()

find_package(Qt5 COMPONENTS Widgets Core Gui PrintSupport REQUIRED)

add_library(${PROJECT_NAME} 
                qcustomplot.h
                qcustomplot.cpp
                )

target_link_libraries(${PROJECT_NAME}
                        ${MOC_Files}
                        Qt5::Core
                        Qt5::Gui
                        Qt5::Widgets
                        Qt5::PrintSupport
                        )

# set(CPACK_PROJECT_NAME ${PROJECT_NAME})
# set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
# include(CPack)
