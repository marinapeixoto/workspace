project(imhist)
find_package(OpenCV REQUIRED)
add_executable(imhist imhist.cpp)
target_link_libraries(imhist ${OpenCV_LIBS})

