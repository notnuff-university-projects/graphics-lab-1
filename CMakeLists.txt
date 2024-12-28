cmake_minimum_required(VERSION 3.28)
set(CMAKE_CXX_STANDARD 20)

project(lab1_graphics)

# Add list of all source files in /src to the SOURCE_FILES variables
file(GLOB_RECURSE SOURCE_FILES
        ${CMAKE_SOURCE_DIR}/src/*.c
        ${CMAKE_SOURCE_DIR}/src/*.cpp)

# Add all header files
file(GLOB_RECURSE HEADER_FILES
        ${CMAKE_SOURCE_DIR}/src/*.h
        ${CMAKE_SOURCE_DIR}/src/*.hpp)


# Define the executable
add_executable(${PROJECT_NAME} ${HEADER_FILES} ${SOURCE_FILES}
        rsc/shaders/gl_vertexShader.h
        rsc/shaders/gl_fragmentShader.h
        src/Shader.h)


# We need a CMAKE_DIR with some code to find external dependencies
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

#######################################
# LOOK for the packages that we need! #
#######################################

find_package(Freetype REQUIRED)

# OpenGL
find_package(OpenGL REQUIRED)

# GLFW
find_package(GLFW3 REQUIRED)

find_package(glm REQUIRED)

# Find the installed `stb` library
find_package(PkgConfig REQUIRED)
pkg_check_modules(STB REQUIRED stb)

# Include `stb` headers
include_directories(${STB_INCLUDE_DIRS})

# Put all libraries into a variable
set(LIBS ${GLFW3_LIBRARY} ${OPENGL_LIBRARY} Freetype::Freetype glm::glm)


# Define the include DIRs
include_directories(
        "${CMAKE_SOURCE_DIR}/src"
        "${CMAKE_SOURCE_DIR}/rsc"
        "${CMAKE_SOURCE_DIR}/glad"
        "${CMAKE_SOURCE_DIR}/KHR"
)

# Define the link libraries
target_link_libraries(${PROJECT_NAME} ${LIBS} ${STB_LIBRARIES} )

# Add post-build command to copy the file
add_custom_command(
        TARGET lab1_graphics
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/rsc"
        "$<TARGET_FILE_DIR:lab1_graphics>/rsc"
)

# add OpenGL
set(OpenGL_GL_PREFERENCE GLVND)
find_package(OpenGL REQUIRED)

