#include "camera/camera.h"

static void camera_updateCameraVectors(Camera *camera);

Camera camera_create(vec3 position, vec3 up, float yaw, float pitch)
{
    Camera camera;
    glm_vec3_copy(position, camera.Position);
    glm_vec3_copy(up, camera.WorldUp);
    camera.Yaw = yaw;
    camera.Pitch = pitch;
    camera.MovementSpeed = CAMERA_SPEED;
    camera.MouseSensitivity = CAMERA_SENSITIVITY;
    camera.Zoom = CAMERA_ZOOM;

    camera.Front[0] = 0.0f;
    camera.Front[1] = 0.0f;
    camera.Front[2] = -1.0f;

    camera_updateCameraVectors(&camera);
    return camera;
}

Camera camera_createWithValues(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch)
{
    Camera camera;
    camera.Position[0] = posX;
    camera.Position[1] = posY;
    camera.Position[2] = posZ;
    camera.WorldUp[0] = upX;
    camera.WorldUp[1] = upY;
    camera.WorldUp[2] = upZ;
    camera.Yaw = yaw;
    camera.Pitch = pitch;
    camera.MovementSpeed = CAMERA_SPEED;
    camera.MouseSensitivity = CAMERA_SENSITIVITY;
    camera.Zoom = CAMERA_ZOOM;

    camera.Front[0] = 0.0f;
    camera.Front[1] = 0.0f;
    camera.Front[2] = -1.0f;

    camera_updateCameraVectors(&camera);
    return camera;
}

void camera_getViewMatrix(Camera *camera, mat4 view)
{
    vec3 target;
    glm_vec3_add(camera->Position, camera->Front, target);
    glm_lookat(camera->Position, target, camera->Up, view);
}

void camera_processKeyboard(Camera *camera, Camera_Movement direction, float deltaTime)
{
    float velocity = camera->MovementSpeed * deltaTime;
    vec3 temp;

    if (direction == CAMERA_FORWARD) {
        glm_vec3_scale(camera->Front, velocity, temp);
        glm_vec3_add(camera->Position, temp, camera->Position);
    }
    if (direction == CAMERA_BACKWARD) {
        glm_vec3_scale(camera->Front, velocity, temp);
        glm_vec3_sub(camera->Position, temp, camera->Position);
    }
    if (direction == CAMERA_LEFT) {
        glm_vec3_scale(camera->Right, velocity, temp);
        glm_vec3_sub(camera->Position, temp, camera->Position);
    }
    if (direction == CAMERA_RIGHT) {
        glm_vec3_scale(camera->Right, velocity, temp);
        glm_vec3_add(camera->Position, temp, camera->Position);
    }
}

void camera_processMouseMovement(Camera *camera, float xoffset, float yoffset, bool constrainPitch)
{
    xoffset *= camera->MouseSensitivity;
    yoffset *= camera->MouseSensitivity;

    camera->Yaw += xoffset;
    camera->Pitch += yoffset;

    if (constrainPitch) {
        if (camera->Pitch > 89.0f) camera->Pitch = 89.0f;
        if (camera->Pitch < -89.0f) camera->Pitch = -89.0f;
    }

    camera_updateCameraVectors(camera);
}

void camera_processMouseScroll(Camera *camera, float yoffset)
{
    camera->Zoom -= yoffset;
    if (camera->Zoom < 1.0f) camera->Zoom = 1.0f;
    if (camera->Zoom > 45.0f) camera->Zoom = 45.0f;
}

void camera_updateCameraVectors(Camera *camera)
{
    vec3 front;
    front[0] = cosf(glm_rad(camera->Yaw)) * cosf(glm_rad(camera->Pitch));
    front[1] = sinf(glm_rad(camera->Pitch));
    front[2] = sinf(glm_rad(camera->Yaw)) * cosf(glm_rad(camera->Pitch));
    glm_vec3_normalize(front);
    glm_vec3_copy(front, camera->Front);

    glm_vec3_cross(camera->Front, camera->WorldUp, camera->Right);
    glm_vec3_normalize(camera->Right);

    glm_vec3_cross(camera->Right, camera->Front, camera->Up);
}
