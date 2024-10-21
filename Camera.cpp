#include "Camera.hpp"


Camera::Camera(Vector3 cameraPos, Vector3 up) : position(cameraPos), worldUp(up)
{
    yaw = -90.0f;
    pitch = 0.0f;
    speed = 0.01f;
    sensitivity = 0.01f;
    setCameraVectors();
}

Camera::~Camera()
{
}

void Camera::setCameraVectors()
{
    Vector3 tmpFront = Vector3(0);
    tmpFront.x = cos(yaw * (M_PI / 180) * cos(pitch * (M_PI / 180)));
    tmpFront.y = sin(pitch * (M_PI / 180));
    tmpFront.z = sin(yaw * (M_PI / 180) * cos(pitch * (M_PI / 180)));

    front = normalized(tmpFront);

    right = normalized(cross(front, worldUp));
    up = normalized(cross(right, front));
}
