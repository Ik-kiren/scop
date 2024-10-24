
#include "Scop.hpp"
#include "stb_image.h"
#include <cmath>

using namespace std;

void ControlCamera(GLFWwindow* window, float &lastX, float &lastY, Camera *camera)
{
    double xpos, ypos;
    float xoffset, yoffset;

    glfwGetCursorPos(window, &xpos, &ypos);
    xoffset = xpos - lastX;
    yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;

    const float sensitivity = 0.01f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    camera->RegisterMouseInput(xoffset, yoffset);
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
    Camera newCamera = Camera(Vector3(0, 0 ,5), Vector3(0, 1, 0));
    do{
        float timeValue = glfwGetTime();
        float redColor = (sin(timeValue) / 2.0f) + 0.5f;
        glClearColor(0.0f, 0.45f, 0.3f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        ControlCamera(window, lastX, lastY, &newCamera);
        mesh.SetModel(Rotate(*mesh.getModel(), M_PI / 64, Vector3(0, 1, 0)));
        newCamera.RegisterKeyboardInput(window);
        view = newCamera.GetViewMatrix();
        mesh.drawMesh();

        // Swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();

    } // Vérifie si on a appuyé sur la touche échap (ESC) ou si la fenêtre a été fermée
    while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
        glfwWindowShouldClose(window) == 0);

    glfwTerminate();
}
