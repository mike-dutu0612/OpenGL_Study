#include "renderer/shader.h"
#include <stdio.h>
#include <stdlib.h>

/* 读取文件工具 */
static char* read_file(const char* path)
{
    FILE* file = fopen(path, "rb");
    if (!file) {
        fprintf(stderr, "Failed to open file: %s\n", path);
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    rewind(file);

    char* data = (char*)malloc(size + 1);
    fread(data, 1, size, file);
    data[size] = '\0';

    fclose(file);
    return data;
}

Shader shader_create(const char* vertex_path, const char* fragment_path)
{
    Shader shader = {0};

    char* vsrc = read_file(vertex_path);
    char* fsrc = read_file(fragment_path);

    unsigned int vs = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs, 1, (const char**)&vsrc, NULL);
    glCompileShader(vs);

    unsigned int fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs, 1, (const char**)&fsrc, NULL);
    glCompileShader(fs);

    shader.id = glCreateProgram();
    glAttachShader(shader.id, vs);
    glAttachShader(shader.id, fs);
    glLinkProgram(shader.id);

    glDeleteShader(vs);
    glDeleteShader(fs);

    free(vsrc);
    free(fsrc);

    return shader;
}

void shader_use(const Shader* shader)
{
    glUseProgram(shader->id);
}

void shader_set_int(const Shader* shader, const char* name, int value)
{
    glUniform1i(glGetUniformLocation(shader->id, name), value);
}

void shader_set_float(const Shader* shader, const char* name, float value)
{
    glUniform1f(glGetUniformLocation(shader->id, name), value);
}

void shader_destroy(Shader* shader)
{
    if (shader->id) {
        glDeleteProgram(shader->id);
        shader->id = 0;
    }
}
