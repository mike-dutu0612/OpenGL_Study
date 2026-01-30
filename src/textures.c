#include <stdio.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "renderer/shader.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h" 


float vertices[] = {
    // 位置                       颜色                        纹理坐标  
     0.5f,  0.5f, 0.0f,         1.0f, 0.0f, 0.0f,           1.0f, 1.0f, 
     0.5f, -0.5f, 0.0f,         0.0f, 1.0f, 0.0f,           1.0f, 0.0f, 
    -0.5f, -0.5f, 0.0f,         0.0f, 0.0f, 1.0f,           0.0f, 0.0f, 
    -0.5f,  0.5f, 0.0f,         1.0f, 1.0f, 0.0f,           0.0f, 1.0f,
};

unsigned int indices[] = 
{  
    0, 1, 3, // first triangle
    1, 2, 3  // second triangle
};


float borderColor[] = {1.0f, 1.0f, 0.0f, 1.0f};




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

    GLFWwindow* window = glfwCreateWindow(800, 600, "textures Test", NULL, NULL);
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

    /* ===== 测试 textures 模块 ===== */
    Shader t_textures = shader_create(
        "../shaders/textures.vert",
        "../shaders/textures.frag"
    );

    printf("t_textures program id = %u\n", t_textures.id);

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
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(0 * sizeof(float)));
    glEnableVertexAttribArray(0);
    // 颜色属性（索引1）：从第3个float开始，偏移3
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // 纹理坐标属性（索引2）：从第6个float开始，偏移6
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
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

    /* 主循环 */
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        shader_use(&t_textures);
        glBindTexture(GL_TEXTURE_2D, texture);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        
        glfwSwapBuffers(window);
    }

    shader_destroy(&t_textures);
    glfwTerminate();
    return 0;
}
