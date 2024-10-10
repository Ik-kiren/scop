
#include "Scop.hpp"
#include "stb_image.h"
#include <cmath>

using namespace std;

Matrix4 Translate(Matrix4 mat, Vector3 vec)
{
    Matrix4 translationMat = Matrix4(1.0);
    translationMat[3] = Vector4(vec);
    return translationMat * mat;
}

Matrix4 Rotate(Matrix4 mat, float radians, Vector3 axis)
{
    Matrix4 rotateMat = Matrix4(
        Vector4(cos(radians) + powf(axis[0], 2.0) * (1 - cos(radians)), axis[0] * axis[1] * (1 - cos(radians)) + axis[2] * sin(radians), axis[0] * axis[2] * (1 - cos(radians)) - axis[1] * sin(radians), 0),
        Vector4(axis[1] * axis[0] * (1 - cos(radians)) - axis[2] * sin(radians), cos(radians) + powf(axis[1], 2.0) * (1 - cos(radians)), axis[1] * axis[2] * (1 - cos(radians)) + axis[0] * sin(radians), 0),
        Vector4(axis[2] * axis[0] * (1 - cos(radians)) + axis[1] * sin(radians), axis[2] * axis[1] * (1 - cos(radians)) - axis[0] * sin(radians), cos(radians) + powf(axis[2], 2.0) * (1 - cos(radians)), 0),
        Vector4(0, 0, 0 ,1)
    );
    return rotateMat;
}

Matrix4 Perspective(float fov, float aspectRatio, float near, float far)
{
    const float DEG2RAD = acos(-1.0f) / 180;
    float tangent = tan(fov / 2 * DEG2RAD);
    float right = near * tangent;
    float top = right / aspectRatio;

    Matrix4 matrix = Matrix4(
        Vector4(near / right, 0, 0, 0),
        Vector4(0, near / top, 0, 0),
        Vector4(0, 0, -(far + near) / (far - near), - 1),
        Vector4(0, 0, -(2 * far * near) / (far - near), 0)
    );
    return matrix;
}


int main() {
    Matrix4 mat2 = Matrix4(1.0);
    mat2[3] = Vector4(1, 0, 0, 1);
    Matrix4 mat = Rotate(mat2, 3 *M_PI / 4, Vector3(0, 1, 0));
    
    Matrix4 model = Matrix4(1.0);
    model = Rotate(model, -M_PI / 3, Vector3(1, 1, 0));

    std::cout << model << std::endl;

    Matrix4 view = Matrix4(1);
    view = Translate(view, Vector3(0, 0, -5));

    Matrix4 projection = Perspective(60, 1920 / 1200, 0.1, 100);

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
    window = glfwCreateWindow(1920, 1200, "tuto 1", NULL, NULL);
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
    static const GLfloat vertices[] = {
        -1.0f, -1.0f, 0.0f, 0.4f, 0.5f, 0.0f,   0.0f, 0.0f,
        1.0f, -1.0f, 0.0f,  0.7f, 0.7f, 0.25f,  1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,   0.9f, 0.9f, 0.3f,   0.5f, 1.0f,

        -1.0f, -1.0f, 1.0f, 0.4f, 0.5f, 0.0f,   0.0f, 0.0f,
        1.0f, -1.0f, 1.0f,  0.7f, 0.7f, 0.25f,  1.0f, 0.0f,
        0.0f, 1.0f, 1.0f,   0.9f, 0.9f, 0.3f,   0.5f, 1.0f
    };
    /*static const GLfloat vertices2[] = {
        -0.5f, 0.001f, 0.0f,
        0.5f, 0.001f, 0.0f,
        0.0f, -1.0f, 0.0f

    };*/
    static const GLfloat vertices2[] = {
        -1.0f, -1.0f, 0.0f,
        -1.0f, -1.0f, 1.0f,
        1.0f, -1.0f, 1.0f,

        1.0f, -1.0f, 0.0f,
        1.0f, -1.0f, 1.0f,
        -1.0f, -1.0f, 0.0f,

        1.0f, -1.0f, 0.0f,
        1.0f, -1.0f, 1.0f,
        0.0f, 1.0f, 1.0f,

        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 1.0f,
        1.0f, -1.0f, 0.0f,

        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 1.0f,
        -1.0f, -1.0f, 0.0f,

        -1.0f, -1.0f, 1.0f,
        -1.0f, -1.0f, 0.0f,
        0.0f, 1.0f, 1.0f
    };
    unsigned int indices[] = {
        0, 1, 2,
    };
    unsigned int indices2[] = {
        0, 1, 2
    };

    GLuint VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

    GLuint VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    GLuint EBO;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int height;
    int width;
    int nrChannel;
    stbi_set_flip_vertically_on_load(true);


    unsigned char *data = stbi_load("Triforce.png", &width, &height, &nrChannel, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);
    

    glVertexAttribPointer(
            0,
            3,
            GL_FLOAT,
            GL_FALSE,
            8 * sizeof(float),
            (void*)0
    );
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(
            1,
            3,
            GL_FLOAT,
            GL_FALSE,
            8 * sizeof(float),
            (void*) (3 * sizeof(float))
    );
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(
            2,
            2,
            GL_FLOAT,
            GL_FALSE,
            8 * sizeof(float),
            (void *) (6 * sizeof(float))
    );
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    GLuint VAO2;
	glGenVertexArrays(1, &VAO2);
	glBindVertexArray(VAO2);

    GLuint VBO2;
    glGenBuffers(1, &VBO2);
    glBindBuffer(GL_ARRAY_BUFFER, VBO2);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices2, GL_STATIC_DRAW);

    GLuint EBO2;
    glGenBuffers(1, &EBO2);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO2);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(
            0,
            3,
            GL_FLOAT,
            GL_FALSE,
            0,
            (void*)0
    );
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

    //GLuint programID = LoadShaders( "VertexShader.shader", "FragmentShader.shader" );
    Shader firstShader = Shader("VertexShader.shader", "FragmentShader.shader");
    //GLuint programID2 = LoadShaders( "VertexShader.shader", "FragmentShader2.shader" );
    Shader secondShader = Shader("VertexShader2.shader", "FragmentShader2.shader");

    do{
        float timeValue = glfwGetTime();
        float redColor = (sin(timeValue) / 2.0f) + 0.5f;
        glClearColor(0.0f, 0.45f, 0.3f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        firstShader.use();
        //glUseProgram(programID);
        model = Rotate(model, (float)glfwGetTime() * -M_PI / 3, Vector3(1, 1, 0));
        firstShader.set_matrix4("model", model);
        firstShader.set_matrix4("view", view);
        firstShader.set_matrix4("projection", projection);
        firstShader.set_float("timeValue", redColor);
        firstShader.set_float("offset", 0.0);

        glBindTexture(GL_TEXTURE_2D, texture);
        glBindVertexArray(VAO);
        //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        secondShader.use();
        //glUseProgram(programID2);
        model = Rotate(model, (float)glfwGetTime() * -M_PI / 3, Vector3(1, 1, 0));
        secondShader.set_matrix4("model", model);
        secondShader.set_matrix4("view", view);
        secondShader.set_matrix4("projection", projection);
        secondShader.set_float("offset", 0.0);

        glBindVertexArray(VAO2);
        //glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
        glDrawArrays(GL_TRIANGLES, 0, 19);

        // Swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();

    } // Vérifie si on a appuyé sur la touche échap (ESC) ou si la fenêtre a été fermée
    while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
    glfwWindowShouldClose(window) == 0 );

    glDeleteBuffers(1, &VBO);
	glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
	glDeleteVertexArrays(1, &VAO);

    glfwTerminate();
}
