# 指定CMake最低版本要求
cmake_minimum_required(VERSION 3.10)

# 项目名称（可根据你的需求修改）
project(OpenGL_Projects C)

# 设置C语言标准为C11，确保兼容性
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# 编译选项：开启基础警告，关闭未使用参数警告（新手友好）
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Wno-unused-parameter")

# 解决OpenGL相关的CMP0072警告（关键配置）
set(OpenGL_GL_PREFERENCE "GLVND")

# ========== 头文件目录配置 ==========
# 假设你的头文件放在include目录下（glad、glfw的头文件都在这里）
include_directories(
    ${CMAKE_SOURCE_DIR}/include  # 项目本地include目录
    /usr/include                 # 系统默认头文件目录（备用）
)

# ========== 依赖文件配置 ==========
# 指定glad.c的路径（请确保路径与你的实际文件位置一致）
set(GLAD_SOURCE ${CMAKE_SOURCE_DIR}/src/glad.c)

# 检查glad.c是否存在，避免编译错误
if(NOT EXISTS ${GLAD_SOURCE})
    message(FATAL_ERROR "未找到glad.c文件，请检查路径：${GLAD_SOURCE}")
endif()

# ========== 收集源文件 ==========
# 收集src目录下所有.c文件（排除glad.c）
file(GLOB_RECURSE APP_SOURCES 
    ${CMAKE_SOURCE_DIR}/src/*.c
)
# 移除glad.c，避免重复编译
list(REMOVE_ITEM APP_SOURCES ${GLAD_SOURCE})

# ========== 查找系统库 ==========
# 查找GLFW库（确保已安装libglfw3-dev）
find_package(PkgConfig REQUIRED)
pkg_search_module(GLFW REQUIRED glfw3)
include_directories(${GLFW_INCLUDE_DIRS})

# 查找OpenGL库
find_package(OpenGL REQUIRED)

# ========== 批量生成可执行文件 ==========
if(APP_SOURCES)
    foreach(SOURCE_FILE ${APP_SOURCES})
        # 提取文件名（去掉路径和.c后缀）作为可执行文件名称
        get_filename_component(EXEC_NAME ${SOURCE_FILE} NAME_WE)
        
        # 生成可执行文件：当前.c文件 + glad.c
        add_executable(${EXEC_NAME} ${SOURCE_FILE} ${GLAD_SOURCE})
        
        # 链接依赖库
        target_link_libraries(${EXEC_NAME}
            ${OPENGL_LIBRARIES}
            ${GLFW_LIBRARIES}
            m  # 数学库（部分系统需要）
        )
        
        # 可选：将生成的可执行文件输出到bin目录（整洁项目结构）
        set_target_properties(${EXEC_NAME} PROPERTIES
            RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin
        )
        
        message(STATUS "已配置编译目标：${EXEC_NAME} (源文件：${SOURCE_FILE})")
    endforeach()
else()
    message(WARNING "在src目录下未找到除glad.c外的.c文件，请新建.c文件后重新编译")
endif()