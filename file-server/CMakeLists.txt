set(CMAKE_CXX_STANDARD 11)
project(file-server)

if (WIN32)
    set(MY_LIBS ${MY_LIBS} ws2_32 winmm)
endif (WIN32)

set(SOURCE_FILES main.cpp MyServer.cpp MyServer.h throttle.cpp throttle.h file_guard.cpp file_guard.h ../limit_type.h admin_interface.cpp admin_interface.h dir_utils.cpp dir_utils.h)


add_executable(${PROJECT_NAME} ${SOURCE_FILES})
target_link_libraries(${PROJECT_NAME} ${DLIB_LIBRARY} ${MY_LIBS} url-generator_lib)
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/config
        ${CMAKE_CURRENT_BINARY_DIR}/config)
