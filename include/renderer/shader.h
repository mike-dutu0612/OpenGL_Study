#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>

typedef struct Shader {
    unsigned int id;
} Shader;

/* 创建 shader（从文件） */
Shader shader_create(const char* vertex_path, const char* fragment_path);

/* 使用 shader */
void shader_use(const Shader* shader);

/* uniform 工具函数 */
void shader_set_int(const Shader* shader, const char* name, int value);
void shader_set_float(const Shader* shader, const char* name, float value);

/* 销毁 */
void shader_destroy(Shader* shader);

#endif // SHADER_H
