cmake_minimum_required(VERSION 3.21)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug")
endif()

project(sdl3-cgltf-opengl-box3d-fps-starter CXX C)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# -----------------------------------------------------------------------------
# Locate System OpenGL & Threads Packages
# -----------------------------------------------------------------------------
find_package(OpenGL REQUIRED)
find_package(Threads REQUIRED)

# -----------------------------------------------------------------------------
# Locate SDL3 and SDL3_image Packages
# -----------------------------------------------------------------------------
if(WIN32)
    set(SDL3_DIR "C:/libs/sdl3-3.4.16-mingw/lib/cmake/SDL3")
    set(SDL3_image_DIR "C:/libs/sdl3_image-3.4.6-mingw/lib/cmake/SDL3_image")
else()
    set(SDL3_DIR "$ENV{HOME}/libs/sdl3-3.4.16-ubuntu/lib/cmake/SDL3")
    set(SDL3_image_DIR "$ENV{HOME}/libs/sdl3_image-3.4.6-ubuntu/lib/cmake/SDL3_image")
endif()

find_package(SDL3 REQUIRED CONFIG)
find_package(SDL3_image REQUIRED CONFIG)

# -----------------------------------------------------------------------------
# Set Include Paths for Header-Only, Source, and Physics Libraries
# -----------------------------------------------------------------------------
if(WIN32)
    set(GLM_INCLUDE_DIR "C:/libs/glm-1.0.3/glm")
    set(CGLTF_INCLUDE_DIR "C:/libs/cgltf-1.15")
    set(GLAD_INCLUDE_DIR "C:/libs/glad-0.1.36/opengl-3.3/include")
    set(GLAD_SOURCE "C:/libs/glad-0.1.36/opengl-3.3/src/glad.c")
    set(BOX3D_DIR "C:/libs/box3d-0.1.0-mingw")
else()
    set(GLM_INCLUDE_DIR "$ENV{HOME}/libs/glm-1.0.3")
    set(CGLTF_INCLUDE_DIR "$ENV{HOME}/libs/cgltf-1.15")
    set(GLAD_INCLUDE_DIR "$ENV{HOME}/libs/glad-0.1.36/opengl-3.3/include")
    set(GLAD_SOURCE "$ENV{HOME}/libs/glad-0.1.36/opengl-3.3/src/glad.c")
    set(BOX3D_DIR "$ENV{HOME}/libs/box3d-0.1.0-ubuntu")
endif()

# -----------------------------------------------------------------------------
# Define Imported Static Library for Box3D
# -----------------------------------------------------------------------------
add_library(Box3D_Static STATIC IMPORTED)
set_target_properties(Box3D_Static PROPERTIES IMPORTED_LOCATION "${BOX3D_DIR}/lib/libbox3d.a")

# -----------------------------------------------------------------------------
# Target Definition & Sources
# -----------------------------------------------------------------------------
add_executable(app
    src/app.h
    src/fps_controller.h
    src/fps_controller.cpp
    src/main.cpp
    src/model_utils.h
    src/model_utils.cpp
    src/physics_debug.h
    src/physics_debug.cpp
    src/texture_utils.h
    src/texture_utils.cpp
    src/shader_utils.h
    src/shader_utils.cpp
    ${GLAD_SOURCE}
)

target_include_directories(app PRIVATE 
    ${GLM_INCLUDE_DIR}
    ${CGLTF_INCLUDE_DIR}
    ${GLAD_INCLUDE_DIR}
    ${BOX3D_DIR}/include
)

# -----------------------------------------------------------------------------
# Copy Assets Directory to Build Output
# -----------------------------------------------------------------------------
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/assets")
    add_custom_command(
        TARGET app POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E remove_directory "$<TARGET_FILE_DIR:app>/assets"
        COMMAND ${CMAKE_COMMAND} -E copy_directory
                "${CMAKE_CURRENT_SOURCE_DIR}/assets"
                "$<TARGET_FILE_DIR:app>/assets"
        COMMENT "Copying assets directory"
    )
endif()

# -----------------------------------------------------------------------------
# Link Libraries & Flags
# -----------------------------------------------------------------------------
target_link_libraries(app PRIVATE 
    SDL3_image::SDL3_image-static
    SDL3::SDL3-static
    OpenGL::GL
    Box3D_Static
    Threads::Threads
)

if(WIN32)
    target_compile_options(app PRIVATE -Os)
    target_link_options(app PRIVATE -static -s -static-libgcc -static-libstdc++)

    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        target_link_options(app PRIVATE -mconsole)
    else()
        target_link_options(app PRIVATE -mwindows)
    endif()
endif()