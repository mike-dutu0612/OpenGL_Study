#include <stdio.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "renderer/shader.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h" 
#include <cglm/cglm.h>

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
        "../shaders/transform.vert",
        "../shaders/transform.frag"
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

        //变换
        mat4 transform;
        glm_mat4_identity(transform);

        vec3 translate = {0.5f, -0.5f, 0.0f};
        glm_translate(transform, translate);

        float time = (float)glfwGetTime();
        glm_rotate(transform, time, (vec3){0.0f, 0.0f, 1.0f});

        vec3 scale = {0.8f, 0.8f, 0.8f};
        glm_scale(transform, scale);

        int transformLoc = glGetUniformLocation(t_textures.id, "transform");
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, (const float*)transform);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);
        glBindVertexArray(VAO);
        shader_use(&t_textures);
        glUniform1i(glGetUniformLocation(t_textures.id, "texture1"), 0);
        glUniform1i(glGetUniformLocation(t_textures.id, "texture2"), 1);
        //glDrawArrays(GL_TRIANGLES, 0, 3);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        
        glfwSwapBuffers(window);
    }

    shader_destroy(&t_textures);
    glfwTerminate();
    return 0;
}
