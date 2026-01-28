cmake_minimum_required(VERSION 3.10)

project(OpenGL_Projects C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Wno-unused-parameter")

set(OpenGL_GL_PREFERENCE "GLVND")

# ========== include 路径 ==========
include_directories(
    ${CMAKE_SOURCE_DIR}/include
)

# ========== glad ==========
set(GLAD_SOURCE ${CMAKE_SOURCE_DIR}/src/glad.c)
if(NOT EXISTS ${GLAD_SOURCE})
    message(FATAL_ERROR "未找到 glad.c")
endif()

# ========== engine 源文件 ==========
file(GLOB_RECURSE ENGINE_SOURCES
    ${CMAKE_SOURCE_DIR}/engine/*.c
)

# ========== app 源文件 ==========
file(GLOB APP_SOURCES
    ${CMAKE_SOURCE_DIR}/src/*.c
)
list(REMOVE_ITEM APP_SOURCES ${GLAD_SOURCE})

# ========== 依赖 ==========
find_package(PkgConfig REQUIRED)
pkg_search_module(GLFW REQUIRED glfw3)
find_package(OpenGL REQUIRED)

# ========== 生成可执行文件 ==========
foreach(SOURCE_FILE ${APP_SOURCES})
    get_filename_component(EXEC_NAME ${SOURCE_FILE} NAME_WE)

    add_executable(${EXEC_NAME}
        ${SOURCE_FILE}
        ${GLAD_SOURCE}
        ${ENGINE_SOURCES}
    )

    target_link_libraries(${EXEC_NAME}
        ${OPENGL_LIBRARIES}
        ${GLFW_LIBRARIES}
        m
    )

    set_target_properties(${EXEC_NAME} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin
    )

    message(STATUS "已配置目标：${EXEC_NAME}")
endforeach()
