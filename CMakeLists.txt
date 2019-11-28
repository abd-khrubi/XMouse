cmake_minimum_required(VERSION 3.14)
project(XMouse C)

find_package(X11 REQUIRED)
message(STATUS "X11 libs: ${X11_LIBRARIES}")

include_directories(${X11_LIBRARIES})


set(CMAKE_C_STANDARD 99)
set(EXEC_NAME XMouse)

add_executable(${EXEC_NAME} XMouse.c)

target_link_libraries(${EXEC_NAME} ${X11_LIBRARIES})

