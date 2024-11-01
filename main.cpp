#include "Scop.hpp"
#include <cmath>

void viewport_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

GLFWwindow *InitGLFW() {
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        exit(0);
    }
    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window;
    window = glfwCreateWindow(1920, 1200, "window", NULL, NULL);
    if (!window) {
        std::cerr << "failed to create window" << std::endl;
        glfwTerminate();
        exit(0);
    }
    glfwMakeContextCurrent(window);

    glewExperimental = true;
    if (glewInit() != GLEW_OK) {
        std::cerr << "failed to initialize GLEW" << std::endl;
        glfwTerminate();
        exit(0);
    }
    glViewport(0, 0, 1920, 1200);
    glfwSetFramebufferSizeCallback(window, viewport_size_callback);

    glEnable(GL_DEPTH_TEST);

    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    return window;
}

void Scop(GLFWwindow *window) {
    Object test = Object("cubeoffset.obj");
    Object teapot = Object("teapot3.0.obj");

    Shader firstShader = Shader("VertexShader.shader", "FragmentShader.shader");
    Shader secondShader = Shader("VertexShader2.shader", "FragmentShader2.shader");

    Mesh mesh = Mesh(secondShader, test);
    Mesh mesh2 = Mesh(secondShader, teapot);

    float time = 0;
    float lastX = 910;
    float lastY = 600;
    mesh.SetModel(Translate(*mesh.getModel(), Vector3(0, 3, 0)));
    Camera newCamera = Camera(Vector3(0, 0 , 5), Vector3(0, 1, 0));

    while ( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
        glfwWindowShouldClose(window) == 0) {
        float timeValue = glfwGetTime();
        float redColor = (sin(timeValue) / 2.0f) + 0.5f;
        glClearColor(0.0f, 0.45f, 0.3f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        newCamera.RegisterMouseInput(window);
        newCamera.RegisterKeyboardInput(window);
        mesh.SetModel(Rotate(*mesh.getModel(), M_PI / 512, Vector3(0, 1, 0)));
        mesh.drawMesh(window, newCamera);
        mesh2.drawMesh(window, newCamera);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
}


int main() {
    GLFWwindow *window;
    window = InitGLFW();

    Scop(window);
}
