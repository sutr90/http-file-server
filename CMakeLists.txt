cmake_minimum_required(VERSION 3.6)

if (WIN32)
    include_directories(D:\\tmp\\dlib)
    find_library(DLIB_LIBRARY dlib HINTS D:\\tmp\\dlib\\cmake-build-release\\dlib)
    set(MY_LIBS "-lws2_32 -lwinmm -lsqlite3")
endif (WIN32)

add_subdirectory(file-server)
add_subdirectory(url-generator)