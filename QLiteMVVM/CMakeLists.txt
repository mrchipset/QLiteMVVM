cmake_minimum_required(VERSION 3.0.0)
project(QLiteMVVMLib VERSION 0.1.0)



find_package(Qt5 COMPONENTS Core Concurrent REQUIRED)


aux_source_directory(. DIR_SRCS)
aux_source_directory(Command COMMAND_SRCS)
aux_source_directory(DependencyObject DEPENDENCY_SRCS)
aux_source_directory(Global GLOBAL_SRCS)
aux_source_directory(Task TASK_SRCS)

add_library(${PROJECT_NAME}
                ${DIR_SRCS}
                ${COMMAND_SRCS}
                ${DEPENDENCY_SRCS}
                ${GLOBAL_SRCS}
                ${TASK_SRCS}
                )

target_link_libraries(${PROJECT_NAME}
                        ${MOC_Files}
                        Qt5::Core
                        Qt5::Concurrent
                        )

