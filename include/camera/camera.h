#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <cglm/cglm.h>

typedef enum {
    CAMERA_FORWARD,
    CAMERA_BACKWARD,
    CAMERA_LEFT,
    CAMERA_RIGHT
} Camera_Movement;

#define CAMERA_YAW         -90.0f
#define CAMERA_PITCH        0.0f
#define CAMERA_SPEED        1.5f
#define CAMERA_SENSITIVITY 0.03f
#define CAMERA_ZOOM        45.0f

typedef struct {
    vec3 Position;
    vec3 Front;
    vec3 Up;
    vec3 Right;
    vec3 WorldUp;
    float Yaw;
    float Pitch;
    float MovementSpeed;
    float MouseSensitivity;
    float Zoom;
} Camera;

Camera camera_create(vec3 position, vec3 up, float yaw, float pitch);
Camera camera_createWithValues(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch);
void camera_getViewMatrix(Camera *camera, mat4 view);
void camera_processKeyboard(Camera *camera, Camera_Movement direction, float deltaTime);
void camera_processMouseMovement(Camera *camera, float xoffset, float yoffset, bool constrainPitch);
void camera_processMouseScroll(Camera *camera, float yoffset);

#endif // CAMERA_H
