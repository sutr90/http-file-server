cmake_minimum_required(VERSION 3.6)
project(server)

set(CMAKE_CXX_STANDARD 11)

if (WIN32)
    include_directories(D:\\tmp\\dlib)
    find_library(DLIB_LIBRARY dlib HINTS D:\\tmp\\dlib\\cmake-build-release\\dlib)
    set(MY_LIBS "-lws2_32 -lwinmm -lsqlite3")
endif (WIN32)

set(SOURCE_FILES main.cpp MyServer.cpp MyServer.h names.h names.cpp throttle.cpp throttle.h file_guard.cpp file_guard.h)
add_executable(server ${SOURCE_FILES})
target_link_libraries(${PROJECT_NAME} ${DLIB_LIBRARY} ${MY_LIBS})