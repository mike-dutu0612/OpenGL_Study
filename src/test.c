#include <stdio.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "renderer/shader.h"

static void framebuffer_size_callback(GLFWwindow* window, int w, int h)
{
    glViewport(0, 0, w, h);
}

int main(void)
{
    /* GLFW 初始化 */
    if (!glfwInit()) {
        fprintf(stderr, "Failed to init GLFW\n");
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "Shader Test", NULL, NULL);
    if (!window) {
        fprintf(stderr, "Failed to create window\n");
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    /* GLAD */
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        fprintf(stderr, "Failed to init GLAD\n");
        return -1;
    }

    /* ===== 测试 Shader 模块 ===== */
    Shader shader = shader_create(
        "../shaders/basic.vert",
        "../shaders/basic.frag"
    );

    printf("Shader program id = %u\n", shader.id);

    /* 主循环 */
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        shader_use(&shader);

        glfwSwapBuffers(window);
    }

    shader_destroy(&shader);
    glfwTerminate();
    return 0;
}
