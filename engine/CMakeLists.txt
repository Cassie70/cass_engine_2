cmake_minimum_required(VERSION 3.20)

project(cass_engine LANGUAGES C CXX)

# ================== ENGINE ==================

add_library(engine STATIC
    "core/Application.cpp"
    "core/Window.cpp"
    "renderer/Renderer.cpp"
    "core/Time.cpp" 
    "renderer/Renderer2D.cpp"
 "renderer/camera/OrthographicCamera.cpp" "renderer/Texture2D.cpp" "input/Input.cpp")

target_include_directories(engine PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/core
    ${CMAKE_CURRENT_SOURCE_DIR}/renderer
    ${CMAKE_CURRENT_SOURCE_DIR}/input
    ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/cassLinear
    ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/stb
)

target_compile_features(engine PUBLIC cxx_std_20)

# ================== DEPENDENCIES ==================

# ================== GLAD ==================

add_library(glad STATIC
    dependencies/glad/src/glad.c
)

target_include_directories(glad PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/glad/include
)

# ---------- GLFW ----------
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)

add_subdirectory(dependencies/glfw)
add_subdirectory(dependencies/freetype)

# ---------- OpenGL ----------
find_package(OpenGL REQUIRED)

# ================== LINKING ==================

target_link_libraries(engine PUBLIC
    glfw
    glad
    OpenGL::GL
    freetype
)
