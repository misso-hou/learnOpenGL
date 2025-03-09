
# 设置 CMake 最低版本要求
cmake_minimum_required(VERSION 3.18)

project(learnOGL)

# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# opengl相关库
find_package(glfw3 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(glm REQUIRED)

# 添加头文件搜索路径
# include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

# 查找 src 目录下的所有源文件
set(SOURCES
    src/main.cpp
    src/input_handler.cpp
    src/opengl_lib/glad.c
    # 可以继续添加其他源文件
)
# file(GLOB SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c")

# 添加可执行文件
add_executable(${PROJECT_NAME} ${SOURCES})

# 添加头文件路径
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

# 链接 GLFW 和 OpenGL 库
target_link_libraries(${PROJECT_NAME} glfw OpenGL::GL glm::glm)