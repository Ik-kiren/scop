#pragma once
#include <cmath>
#include "./Scop.hpp"
#include "./Vector3.hpp"
#include "./Matrix4.hpp"

class Camera {
 private:
    Vector3 position;
    Vector3 front;
    Vector3 up;
    Vector3 right;
    Vector3 worldUp;

    float lastPosX;
    float lastPosY;

    float yaw;
    float pitch;
    float speed;
    float sensitivity;

    void setCameraVectors();

 public:
    Camera(Vector3 cameraPos, Vector3 up);
    ~Camera();

    Vector3 GetPosition();
    Vector3 GetDirection();
    Vector3 GetUp();
    Matrix4 GetViewMatrix();

    void RegisterMouseInput(GLFWwindow *window);
    void RegisterKeyboardInput(GLFWwindow *window);
};
