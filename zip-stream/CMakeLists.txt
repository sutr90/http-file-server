set(CMAKE_CXX_STANDARD 11)
project(zip-stream)

set(SOURCE_FILES main.cpp)
add_executable(${PROJECT_NAME} ${SOURCE_FILES} zip_stream.cpp zip_stream.h zip_headers.cpp zip_headers.h)
target_link_libraries(${PROJECT_NAME} ${DLIB_LIBRARY} ${MY_LIBS})

add_library(${PROJECT_NAME}_lib STATIC ${SOURCE_FILES})
set_target_properties(${PROJECT_NAME} PROPERTIES POSITION_INDEPENDENT_CODE ON)
target_link_libraries(${PROJECT_NAME}_lib ${DLIB_LIBRARY} ${MY_LIBS})