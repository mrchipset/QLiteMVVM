cmake_minimum_required(VERSION 3.0.0)
project(ProcessWorkbench VERSION 0.1.0)


find_package(Qt5 COMPONENTS Core Concurrent Widgets Script REQUIRED)

include_directories(../QLiteMVVM)
include_directories(../Plot)
include_directories(../Plot/QCustomPlot)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

aux_source_directory(. DIR_SRCS)

qt5_wrap_ui(UI_FILES UI/mainwindow.ui)

add_executable(${PROJECT_NAME} ${DIR_SRCS} ${UI_FILES})
target_link_libraries(${PROJECT_NAME}
                        QLiteMVVMLib
                        Plot
                        Qt5::Core
                        Qt5::Concurrent
                        Qt5::Widgets
                        Qt5::Script)

            