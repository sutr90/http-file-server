set(CMAKE_CXX_STANDARD 11)
project(url-generator)

set(SOURCE_FILES main.cpp names.cpp names.h options.cpp options.h url_generator.cpp url_generator.h)
add_executable(${PROJECT_NAME} ${SOURCE_FILES})
target_link_libraries(${PROJECT_NAME} ${DLIB_LIBRARY} ${MY_LIBS})
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/config
        ${CMAKE_CURRENT_BINARY_DIR}/config)
