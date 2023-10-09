cmake_minimum_required(VERSION 3.0)
project(MyOpenGLProject)

# Define source files for each executable
set(SOURCES_Planetary_motion
    main.cpp  # The main application entry point
    # Add other source files for the main executable
)

# Create an executable target for the main application
add_executable(main_program ${SOURCES_Planetary_motion})

# Find and link necessary libraries (e.g., GLFW, GLEW, OpenGL, glm, and GLUT)
find_package(PkgConfig REQUIRED)
pkg_search_module(GLFW REQUIRED glfw3)
find_package(GLEW REQUIRED)
find_package(glm REQUIRED)
find_package(GLUT REQUIRED)

# Link against GLFW, GLEW, OpenGL, glm, and GLUT for the main executable
target_link_libraries(main_program PRIVATE ${GLFW_LIBRARIES} ${GLEW_LIBRARIES} GL glm::glm ${GLUT_LIBRARY})

# Specify include directories for both executables
target_include_directories(main_program PRIVATE ${PROJECT_SOURCE_DIR})

# Include the ImGui library and header files
target_include_directories(main_program PRIVATE /home/rd238422/ImGui/imgui)

