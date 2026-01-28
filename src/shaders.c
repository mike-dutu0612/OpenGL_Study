#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <math.h>

//三角形加颜色顶点数据
float vertices[] = {
    //位置                      //颜色
    -0.5f, -0.5f, 0.0f,         1.0f, 0.0f, 0.0f,
     0.5f, -0.5f, 0.0f,         0.0f, 1.0f, 0.0f,
     0.0f, 0.5f, 0.0f,          0.0f, 0.0f, 1.0f,
};

//顶点着色器
const char *vertexShaderSource = "#version 330 core\n"
                                 "layout (location = 0) in vec3 aPos;\n"
                                 "layout (location = 1) in vec3 aColor;\n"

                                 "out vec3 ourColor;\n"

                                 "void main()\n"
                                 "{\n"
                                 "	gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
                                 "  ourColor = aColor;\n"
                                 "}\0";
//片段着色器
const char *fragmentShaderSource = "#version 330 core\n"
                                   "out vec4 FragColor;\n"
                                   "in vec3 ourColor;\n"
                                   "void main()\n"
                                   "{\n"
                                   "   FragColor = vec4(ourColor, 1.0);\n"
                                   "}\0";

// 窗口大小改变时的回调函数
void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    glfwGetWindowSize(window, &width, &height);
    printf("Window resized to: %dx%d\n", width, height);
}

// 处理键盘输入（ESC 退出）
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}


int main()
{
    // 1. 初始化 GLFW
    if (!glfwInit())
    {
        fprintf(stderr, "Failed to initialize GLFW!\n");
        return -1;
    }
    // 2. 设置 OpenGL 版本（3.3 核心模式）
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // 3. 创建窗口（800x600，标题 OPENGL_STUDY）
    GLFWwindow *window = glfwCreateWindow(800, 600, "OPENGL_STUDY (WSL Ubuntu 24.04)", NULL, NULL);
    if (window == NULL)
    {
        fprintf(stderr, "Failed to create GLFW window!\n");
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // 4. 加载 OpenGL 函数（通过 GLFW）
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        fprintf(stderr, "Failed to initialize GLAD!\n");
        glfwTerminate();
        return -1;
    }

    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        printf("ERROR:%s\n", infoLog);
    }

    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        printf("ERROR:%s\n", infoLog);
    }

    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();

    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        printf("ERROR:%s\n", infoLog);
    }
    glUseProgram(shaderProgram);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    unsigned int VBO;
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);



    while (!glfwWindowShouldClose(window))
    {
        // 处理输入
        processInput(window);

        // 渲染指令：设置背景色为蓝色
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f); // 红=0.2, 绿=0.3, 蓝=0.3, 透明度=1.0
        glClear(GL_COLOR_BUFFER_BIT);         // 清空颜色缓冲区
        
        glUseProgram(shaderProgram);
        
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);



        // 交换缓冲区 + 检查事件
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // 6. 清理资源
    glfwTerminate();
    printf("Program exited normally!\n");
    return 0;
}
