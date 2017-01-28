cmake_minimum_required(VERSION 3.5)

if (WIN32)
    include_directories(D:\\tmp\\dlib)
    find_library(DLIB_LIBRARY dlib HINTS D:\\tmp\\dlib\\cmake-build-release\\dlib)
endif (WIN32)

if (UNIX)
    set(DLIB_LIBRARY dlib)
endif (UNIX)

set(MY_LIBS sqlite3)

add_subdirectory(file-server)
add_subdirectory(url-generator)
