set(CMAKE_CXX_STANDARD 11)
project(file-server)


set(SOURCE_FILES main.cpp MyServer.cpp MyServer.h throttle.cpp throttle.h file_guard.cpp file_guard.h)
add_executable(${PROJECT_NAME} ${SOURCE_FILES})
target_link_libraries(${PROJECT_NAME} ${DLIB_LIBRARY} ${MY_LIBS})