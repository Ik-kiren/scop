#version 330 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 vertexPosition_modelspace;

out vec4 vertexPos;
uniform float offset;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main(){

    gl_Position = projection * view * model * vec4(vertexPosition_modelspace, 1.0);
    vertexPos = gl_Position;
}
