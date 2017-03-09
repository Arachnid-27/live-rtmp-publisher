include_directories(./include)
aux_source_directory(. PROJECT_SOURCES)
set(PROJECT_LIBS x264 rtmp asound faac pthread)

# find opencv
find_package(OpenCV)
if(OPENCV_FOUND)
    include_directories(./include/cv)
    include_directories(${OpenCV_INCLUDE_DIRS})
    link_directories(${OpenCV_LIB_DIR})
    aux_source_directory(./cv CV_SOURCES)
    set(PROJECT_LIBS ${PROJECT_LIBS} ${OpenCV_LIBS})
    set(PROJECT_SOURCES ${PROJECT_SOURCES} ${CV_SOURCES})
endif()

# gen config.h
include_directories(${PROJECT_BINARY_DIR})
configure_file(
    ${PROJECT_SOURCE_DIR}/config.h.in
    ${PROJECT_BINARY_DIR}/config.h
)

add_executable(publisher ${PROJECT_SOURCES})
target_link_libraries(publisher ${PROJECT_LIBS})
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -O2 -Wall")
