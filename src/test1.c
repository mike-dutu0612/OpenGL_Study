#include <stdio.h>
#include <math.h>
#include "cglm/cglm.h"

static int float_eq(float a, float b)
{
    return fabsf(a - b) < 1e-5f;
}

static int test_vec3_add_dot()
{
    vec3 a = {1.0f, 2.0f, 3.0f};
    vec3 b = {4.0f, 5.0f, 6.0f};
    vec3 out;

    glm_vec3_add(a, b, out);
    if (!float_eq(out[0], 5.0f) || !float_eq(out[1], 7.0f) || !float_eq(out[2], 9.0f)) {
        return 0;
    }

    float dot = glm_vec3_dot(a, b);
    return float_eq(dot, 32.0f);
}

static int test_mat4_identity_translate()
{
    mat4 m;
    glm_mat4_identity(m);

    if (!float_eq(m[0][0], 1.0f) || !float_eq(m[1][1], 1.0f) || !float_eq(m[2][2], 1.0f) || !float_eq(m[3][3], 1.0f)) {
        return 0;
    }

    vec3 t = {1.0f, 2.0f, 3.0f};
    glm_translate(m, t);

    return float_eq(m[3][0], 1.0f) && float_eq(m[3][1], 2.0f) && float_eq(m[3][2], 3.0f);
}

int main(void)
{
    int ok = 1;

    ok &= test_vec3_add_dot();
    ok &= test_mat4_identity_translate();

    if (ok) {
        printf("cglm 测试通过\n");
        return 0;
    }

    printf("cglm 测试失败\n");
    return 1;
}
