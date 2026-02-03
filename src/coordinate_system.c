#include <stdio.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "renderer/shader.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h" 
#include <cglm/cglm.h>

float vertices[] = {
    // 位置                              纹理坐标  
     0.5f,  0.5f, 0.0f,                1.0f, 1.0f, 
     0.5f, -0.5f, 0.0f,                1.0f, 0.0f, 
    -0.5f, -0.5f, 0.0f,                0.0f, 0.0f, 
    -0.5f,  0.5f, 0.0f,                0.0f, 1.0f,
};

unsigned int indices[] = 
{  
    0, 1, 3, // first triangle
    1, 2, 3  // second triangle
};



static void framebuffer_size_callback(GLFWwindow* window, int w, int h)
{
    glViewport(0, 0, w, h);
}

static void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;




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

    GLFWwindow* window = glfwCreateWindow(800, 600, "coordinatek_system Test", NULL, NULL);
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

    /* ===== 测试 coordinate_system 模块 ===== */
    Shader coordinatek_system = shader_create(
        "../shaders/coordinate_system.vert",
        "../shaders/coordinate_system.frag"
    );

    printf("coordinatek_system program id = %u\n", coordinatek_system.id);

    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // 位置属性（索引0）：从第0个float开始，偏移0
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(0 * sizeof(float)));
    glEnableVertexAttribArray(0);
    // 纹理坐标属性（索引2）：从第6个float开始，偏移6
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    unsigned int texture1;
    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    stbi_set_flip_vertically_on_load(1);

    int width, height, nrChannels;
    unsigned char *data = stbi_load("../pic/container.jpg", &width, &height, &nrChannels, 0);
    if(data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        printf("Texture loaded successfully: (size: %dx%d, original channels: %d)\n", 
               width, height, nrChannels);
    } 
    else
    {
        printf("Failed to load texture!!!");
    }
    
    stbi_image_free(data);


    unsigned int texture2;
    glGenTextures(1, &texture2);
    glBindTexture(GL_TEXTURE_2D, texture2);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width2, height2, nrChannels2;
    unsigned char *data2 = stbi_load("../pic/awesomeface.png", &width2, &height2, &nrChannels2, 0);
    if(data2)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width2, height2, 0, GL_RGBA, GL_UNSIGNED_BYTE, data2);
        glGenerateMipmap(GL_TEXTURE_2D);
        printf("Texture loaded successfully: (size: %dx%d, original channels: %d)\n", 
               width2, height2, nrChannels2);
    } 
    else
    {
        printf("Failed to load texture!!!");
    }

    stbi_image_free(data2);


    /* 主循环 */
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        processInput(window);

        mat4 model;         // cglm的矩阵类型（float[16]）
        mat4 view;
        mat4 projection;

        // 1. 初始化矩阵为单位矩阵
        glm_mat4_identity(model);
        glm_mat4_identity(view);
        glm_mat4_identity(projection);

        // 2. 模型矩阵：绕X轴旋转-55度
        glm_rotate(model, glm_rad(-55.0f), (vec3){1.0f, 0.0f, 0.0f});

        // 3. 视图矩阵：沿Z轴平移-3.0f
        glm_translate(view, (vec3){0.0f, 0.0f, -3.0f});

        // 4. 投影矩阵：透视投影
        glm_perspective(glm_rad(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f, projection);

        // 6. 获取uniform变量位置（核心：替代setMat4的关键）
        unsigned int modelLoc = glGetUniformLocation(coordinatek_system.id, "model");
        unsigned int viewLoc  = glGetUniformLocation(coordinatek_system.id, "view");
        unsigned int projLoc  = glGetUniformLocation(coordinatek_system.id, "projection");

        // 7. 传递矩阵到着色器（纯OpenGL原生方式，无自定义方法）
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, (const float*)model);   // 直接传入cglm矩阵
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, (const float*)view);     // cglm mat4是float数组，无需额外处理
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, (const float*)projection);


        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);
        glBindVertexArray(VAO);
        shader_use(&coordinatek_system);
        glUniform1i(glGetUniformLocation(coordinatek_system.id, "texture1"), 0);
        glUniform1i(glGetUniformLocation(coordinatek_system.id, "texture2"), 1);
        //glDrawArrays(GL_TRIANGLES, 0, 3);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        
        glfwSwapBuffers(window);
    }

    shader_destroy(&coordinatek_system);
    glfwTerminate();
    return 0;
}
