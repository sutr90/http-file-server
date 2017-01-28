set(CMAKE_CXX_STANDARD 11)
project(url-generator)

if (WIN32)
    set(MY_LIBS "-lsqlite3")
endif (WIN32)

set(SOURCE_FILES main.cpp names.cpp names.h options.cpp options.h)
add_executable(${PROJECT_NAME} ${SOURCE_FILES})
target_link_libraries(${PROJECT_NAME} ${DLIB_LIBRARY} ${MY_LIBS})