cmake_minimum_required(VERSION 3.0.0)
project(Samples VERSION 0.1.0)

find_package(Qt5 COMPONENTS Core Concurrent Widgets Qml  Script REQUIRED)

include_directories(../QLiteMVVM)
include_directories(../Plot)
include_directories(../Plot/QCustomPlot)

add_executable(TestDemo testMyObject.cc myObject.cc)
target_link_libraries(TestDemo  QLiteMVVMLib
                        Plot
                        Qt5::Core
                        Qt5::Concurrent
                        Qt5::Qml
                        Qt5::Widgets
                        Qt5::Script)

add_executable(TestPlot testPlot.cc)
target_link_libraries(TestPlot  QLiteMVVMLib
                        Plot
                        Qt5::Core
                        Qt5::Concurrent
                        Qt5::Qml
                        Qt5::Widgets
                        Qt5::Script)
            