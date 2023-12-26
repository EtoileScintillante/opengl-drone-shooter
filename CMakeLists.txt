cmake_minimum_required(VERSION 3.5)
project(Drone-Shooter)

# This line needs to be added otherwise miniaudio will fail to
# load any backends and defaults to Null backend, resulting in no sound
# See: https://github.com/mackron/miniaudio/issues/750
add_compile_definitions(MA_NO_RUNTIME_LINKING)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE SOURCE_FILES
    "${CMAKE_SOURCE_DIR}/src/*.cpp"
    "${CMAKE_SOURCE_DIR}/src/*.c"
    "${CMAKE_SOURCE_DIR}/main.cpp"
)

include_directories(
    ${CMAKE_SOURCE_DIR}/include
    ${GLFW_INCLUDE_DIRS}
    ${ASSIMP_INCLUDE_DIRS}
    ${FREETYPE_INCLUDE_DIRS}
)

add_executable(Drone-Shooter ${SOURCE_FILES})
target_link_libraries(Drone-Shooter
    ${CMAKE_SOURCE_DIR}/lib/libassimp.5.2.0.dylib # built for arm64
    ${CMAKE_SOURCE_DIR}/lib/libfreetype.6.dylib # built for arm64
    ${CMAKE_SOURCE_DIR}/lib/libglfw.3.3.dylib # built for arm64
    "-framework OpenGL"
    "-framework Cocoa"
    "-framework IOKit"
    "-framework CoreAudio"
    "-framework AudioToolbox"
    "-framework AudioUnit"
    "-framework CoreVideo"
    "-framework CoreFoundation"
)

# Output dir for executable
set_target_properties(Drone-Shooter PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}"
)

set(CMAKE_BUILD_TYPE Debug)
