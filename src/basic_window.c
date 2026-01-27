#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdio.h>

// 窗口大小改变时的回调函数
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glfwGetWindowSize(window, &width, &height);
    printf("Window resized to: %dx%d\n", width, height);
}

// 处理键盘输入（ESC 退出）
void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}

int main() {
    // 1. 初始化 GLFW
    if (!glfwInit()) {
        fprintf(stderr, "Failed to initialize GLFW!\n");
        return -1;
    }

    // 2. 设置 OpenGL 版本（3.3 核心模式）
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // 3. 创建窗口（800x600，标题 OPENGL_STUDY）
    GLFWwindow* window = glfwCreateWindow(800, 600, "OPENGL_STUDY (WSL Ubuntu 24.04)", NULL, NULL);
    if (window == NULL) {
        fprintf(stderr, "Failed to create GLFW window!\n");
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // 4. 加载 OpenGL 函数（通过 GLFW）
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        fprintf(stderr, "Failed to initialize GLAD!\n");
        glfwTerminate();
        return -1;
    }

    // 5. 渲染循环（直到窗口关闭）
    while (!glfwWindowShouldClose(window)) {
        // 处理输入
        processInput(window);

        // 渲染指令：设置背景色为蓝色
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f); // 红=0.2, 绿=0.3, 蓝=0.3, 透明度=1.0
        glClear(GL_COLOR_BUFFER_BIT);          // 清空颜色缓冲区

        // 交换缓冲区 + 检查事件
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // 6. 清理资源
    glfwTerminate();
    printf("Program exited normally!\n");
    return 0;
}