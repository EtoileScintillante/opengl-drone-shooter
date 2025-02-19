cmake_minimum_required(VERSION 3.5)
project(Drone-Shooter)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE SOURCE_FILES
    "${CMAKE_SOURCE_DIR}/src/*.cpp"
    "${CMAKE_SOURCE_DIR}/src/*.c"
    "${CMAKE_SOURCE_DIR}/main.cpp"
)

include_directories(${CMAKE_SOURCE_DIR}/include)

if (APPLE)
    # This line needs to be added otherwise miniaudio will fail to
    # load any backends and defaults to Null backend, resulting in no sound
    # See: https://github.com/mackron/miniaudio/issues/750
    add_compile_definitions(MA_NO_RUNTIME_LINKING)

    find_package(PkgConfig REQUIRED)
    pkg_check_modules(GLFW REQUIRED glfw3)
    pkg_check_modules(ASSIMP REQUIRED assimp)
    pkg_check_modules(FREETYPE REQUIRED freetype2)

    include_directories(
        ${GLFW_INCLUDE_DIRS}
        ${ASSIMP_INCLUDE_DIRS}
        ${FREETYPE_INCLUDE_DIRS}
    )

    link_directories(
        ${GLFW_LIBRARY_DIRS}
        ${ASSIMP_LIBRARY_DIRS}
        ${FREETYPE_LIBRARY_DIRS}
    )

    add_executable(Drone-Shooter ${SOURCE_FILES})
    target_link_libraries(Drone-Shooter
        ${GLFW_LIBRARIES}
        ${ASSIMP_LIBRARIES}
        ${FREETYPE_LIBRARIES}
        "-framework OpenGL"
        "-framework Cocoa"
        "-framework IOKit"
        "-framework CoreAudio"
        "-framework AudioToolbox"
        "-framework AudioUnit"
        "-framework CoreVideo"
        "-framework CoreFoundation"
    )
elseif (UNIX AND NOT APPLE)
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(GLFW REQUIRED glfw3)
    pkg_check_modules(ASSIMP REQUIRED assimp)
    pkg_check_modules(FREETYPE REQUIRED freetype2)

    include_directories(
        ${GLFW_INCLUDE_DIRS}
        ${ASSIMP_INCLUDE_DIRS}
        ${FREETYPE_INCLUDE_DIRS}
    )

    link_directories(
        ${GLFW_LIBRARY_DIRS}
        ${ASSIMP_LIBRARY_DIRS}
        ${FREETYPE_LIBRARY_DIRS}
    )

    add_executable(Drone-Shooter ${SOURCE_FILES})
    target_link_libraries(Drone-Shooter
        ${GLFW_LIBRARIES}
        ${ASSIMP_LIBRARIES}
        ${FREETYPE_LIBRARIES}
        GL
        X11
        pthread
        dl
    )
endif()

set_target_properties(Drone-Shooter PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}"
)

set(CMAKE_BUILD_TYPE Debug)
