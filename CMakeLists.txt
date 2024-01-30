cmake_minimum_required(VERSION 3.20)

project(video-to-sound CXX)

find_package(OpenCV REQUIRED)
find_package(portaudio REQUIRED)

add_executable(video-to-sound src/main.cpp src/compute.cu)
target_link_libraries(video-to-sound ${OpenCV_LIBS})
target_link_libraries(video-to-sound portaudio)

target_include_directories(video-to-sound PUBLIC ${OpenCV_INCLUDE_DIRS})
target_include_directories(video-to-sound PUBLIC ${portaudio_INCLUDE_DIRS})
