
#include "Scop.hpp"
#include "stb_image.h"
#include <glm/gtc/matrix_transform.hpp>
#include <cmath>

using namespace std;

Vector3 ControlCamera(GLFWwindow* window, float &lastX, float &lastY, float &yaw, float &pitch, float &xoffset, float &yoffset)
{
    double xpos, ypos;

    glfwGetCursorPos(window, &xpos, &ypos);
    xoffset = xpos - lastX;
    yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;

    const float sensitivity = 0.001f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch += yoffset;

    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;
    Vector3 direction = Vector3(
        cos(yaw * (M_PI / 180) * cos(pitch * (M_PI / 180))),
        sin(pitch * (M_PI / 180)),
        sin(yaw * (M_PI / 180) * cos(pitch * (M_PI / 180)))
    );

    return normalized(direction);
}

int main() {
    std::vector<GLfloat> tmpVertices2;
    std::vector<GLuint> tmpIndices;

    Matrix4 model = Matrix4(1.0);

    Matrix4 view = Matrix4(1);
    view = Translate(view, Vector3(0, 0, -10));

    Matrix4 projection = Perspective(60.0f, 1920 / 1200, 0.1f, 100.0f);
    //Matrix4 projection = Orthographique(-0, 0.9, -0.9, 0.9, 0.1, 100);

    Parser("42.obj", &tmpVertices2, &tmpIndices);

    GLfloat vertices2[tmpVertices2.size()];
    GLuint indices[tmpIndices.size()];

    for (size_t i = 0; i < tmpVertices2.size(); i++)
    {
        vertices2[i] = tmpVertices2[i];
    }
    for (size_t i = 0; i < tmpIndices.size(); i++)
    {
        indices[i] = tmpIndices[i];
    }

    if (!glfwInit()) {
        cerr << "Failed to initialize GLFW" << endl;
        return -1;
    }
    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window;
    window = glfwCreateWindow(1920, 1200, "window", NULL, NULL);
    if (!window) {
        cerr << "failed to create window" << endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    glewExperimental =true;
    if (glewInit() != GLEW_OK) {
        cerr << "failed to initialize GLEW" << endl;
        return -1;
    }

    glEnable(GL_DEPTH_TEST);

    Shader firstShader = Shader("VertexShader.shader", "FragmentShader.shader");
    Shader secondShader = Shader("VertexShader2.shader", "FragmentShader2.shader");
    Mesh mesh = Mesh(tmpVertices2, tmpIndices, secondShader, &view, &projection);

    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    float time = 0;
    float lastX = 910;
    float lastY = 600;
    Vector3 cameraPos = Vector3(0, 0, 5);
    float yaw = -90, pitch = 0;
    float xoffset = 0, yoffset = 0;
    float speed = 0.1f;
    do{
        float timeValue = glfwGetTime();
        float redColor = (sin(timeValue) / 2.0f) + 0.5f;
        glClearColor(0.0f, 0.45f, 0.3f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        Vector3 cameraDirection = ControlCamera(window, lastX, lastY, yaw, pitch, xoffset, yoffset);
        //Vector3 cameraDirection = Vector3(0, 0, 1);
        
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            cameraPos = cameraPos + normalized(cross(cameraDirection, Vector3(0, 1, 0))) * speed;
        else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            cameraPos = cameraPos - normalized(cross(cameraDirection, Vector3(0, 1, 0))) * speed;
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            cameraPos = cameraPos + cameraDirection * speed;
        else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            cameraPos = cameraPos - cameraDirection * speed;
        //std::cout << (xpos - 1920.0 / 2.0) / 1920.0 << " / " << (ypos - 1200.0 / 2.0) / 1200.0 << std::endl;
        //projection = Rotate(projection, 0.05 * -M_PI / 3, Vector3((ypos - 1920.0 / 2.0) / 1920.0, (xpos - 1200.0 / 2.0) / 1200.0, 0));
        //*mesh.getModel() = Translate(*mesh.getModel(), Vector3(cos(glfwGetTime()) / 8, 0, 0));
        Vector3 newFrontDirection = Vector3(cameraPos.x + cameraDirection.x, cameraPos.y + cameraDirection.y, cameraPos.z + cameraDirection.z);

        view = lookAt(cameraPos, newFrontDirection, Vector3(0, 1, 0));
        
        //std::cout << view << std::endl;
        //view = Rotate(view, 0.1 * M_PI / 3, Vector3((ypos - 1920.0 / 2.0) / 1920.0, (xpos - 1200.0 / 2.0) / 1200.0, 0));
        mesh.drawMesh();
        // Swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();

    } // Vérifie si on a appuyé sur la touche échap (ESC) ou si la fenêtre a été fermée
    while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
        glfwWindowShouldClose(window) == 0);

    glfwTerminate();
}
