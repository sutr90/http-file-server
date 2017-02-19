set(CMAKE_CXX_STANDARD 11)
project(file-server)

if (WIN32)
    set(MY_LIBS ${MY_LIBS} ws2_32 winmm)
endif (WIN32)

set(SOURCE_FILES main.cpp MyServer.cpp MyServer.h throttle.cpp throttle.h file_guard.cpp file_guard.h ../limit_type.h admin_interface.cpp admin_interface.h dir_utils.cpp dir_utils.h ../logging.h ../logging.cpp ../streamer.h ../streamer.cpp streamer_network.cpp streamer_network.h mime_type_detector.cpp mime_type_detector.h)


add_executable(${PROJECT_NAME} ${SOURCE_FILES})
target_link_libraries(${PROJECT_NAME} ${DLIB_LIBRARY} ${MY_LIBS} zip-stream_lib url-generator_lib)


file(COPY ${CMAKE_SOURCE_DIR}/config DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ${CMAKE_SOURCE_DIR}/file.html DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ${CMAKE_SOURCE_DIR}/${PROJECT_NAME}/mime.types DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

file(COPY ${CMAKE_SOURCE_DIR}/res DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
