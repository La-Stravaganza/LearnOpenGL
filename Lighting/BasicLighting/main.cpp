#include <iostream>
#include <cmath>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#define  GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void keyboard_movement();
void mouse_callback(GLFWwindow* window, double xpos, double ypos);

const GLuint Width = 800, Height = 600;

glm::vec3 cameraPosition = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
glm::vec3 lampPosition(1.2f, 0.5f, -1.0f);

bool keys[1024];

GLfloat deltaTime = 0.0f;

bool firstMouse = true;
const GLfloat Sensitivity = 0.05;
GLfloat yaw = 0.0f;
GLfloat pitch = 0.0f;
double lastX = 0.0f;
double lastY = 0.0f;


int main() {
    // 1.1 Initialize the GLFW
    glfwInit();

    // 1.2 Set GLFW options
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    // 1.3 Create a GLFW window
    GLFWwindow* window = glfwCreateWindow(Width, Height, "LearnOpengl", nullptr, nullptr);
    if (window == nullptr) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    // 1.4 Link GLFW window
    glfwMakeContextCurrent(window);

    // 2.1 Set callback functions
    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, mouse_callback);

    // 2.2 Capture the mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // 3.1 Set GLEW
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        std::cout << "Failed to initialize GLEW" << std::endl;
        glfwTerminate();
        return -1;
    }

    // 4. Set viewport
    glViewport(0, 0, Width, Height);

    // 5. Link shaders
    Shader objectShader("object.vert", "object.frag");
    Shader lampShader("lamp.vert", "lamp.frag");

    // 6. Create vertices
    GLfloat vertices[] = {
            // position           // normal
            -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
             0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
             0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
             0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
            -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

            -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
             0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
             0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
             0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
            -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
            -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

            -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
            -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
            -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
            -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
            -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
            -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

             0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
             0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
             0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
             0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
             0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
             0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

            -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
             0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
             0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
             0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

            -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
             0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
             0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
             0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
            -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
            -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
    };

    // 7. Create and set object VAO and VBO
    GLuint objectVAO;
    glGenVertexArrays(1, &objectVAO);
    glBindVertexArray(objectVAO);

    GLuint VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6* sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);
    glBindVertexArray(0);

    // 8. Create and set light VAO
    GLuint lampVAO;
    glGenVertexArrays(1, &lampVAO);
    glBindVertexArray(lampVAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);

    // 9. Enable depth test
    glEnable(GL_DEPTH_TEST);

    // 10. Game Loop
    GLfloat lastFrame = 0.0f;

    while (glfwWindowShouldClose(window) == GL_FALSE) {
        glfwPollEvents();

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        keyboard_movement();

        // Make lamp spinning
        lampPosition.x = 2 * std::sin(1.2f * currentFrame);
        lampPosition.z = 0.2f + 2 * std::cos(1.2f * currentFrame);

        // object shader
        objectShader.use();

        // vertex shader's variables
        GLint modelLocation = glGetUniformLocation(objectShader.getProgram(), "model");
        GLint viewLocation = glGetUniformLocation(objectShader.getProgram(), "view");
        GLint projectionLocation = glGetUniformLocation(objectShader.getProgram(), "projection");
        GLint lampPositionLocation = glGetUniformLocation(objectShader.getProgram(), "lampPosition");


        glm::mat4 model;
        model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0f));
        glm::mat4 view = glm::lookAt(cameraPosition, cameraPosition + cameraFront, cameraUp);
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), (GLfloat)Width / (GLfloat)Height, 0.1f, 100.0f);

        glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(projection));
        glUniform3f(lampPositionLocation, lampPosition.x, lampPosition.y, lampPosition.z);

        // fragment shader's variables
        GLint objectColorLocation = glGetUniformLocation(objectShader.getProgram(), "objectColor");
        GLint lampColorLocation = glGetUniformLocation(objectShader.getProgram(), "lampColor");

        glUniform3f(objectColorLocation, 1.0f, 0.22f, 0.33f);
        glUniform3f(lampColorLocation, 1.0f, 1.0f, 1.0f);

        // drawing
        glBindVertexArray(objectVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);


        // lamp shader
        lampShader.use();

        // vertex shader's variables
        modelLocation = glGetUniformLocation(objectShader.getProgram(), "model");
        viewLocation = glGetUniformLocation(objectShader.getProgram(), "view");
        projectionLocation = glGetUniformLocation(objectShader.getProgram(), "projection");

        glm::mat4 lampModel;
        lampModel = glm::translate(lampModel, lampPosition);
        lampModel = glm::scale(lampModel, glm::vec3(0.2f));
        glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(lampModel));
        glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(projection));

        // drawing
        glBindVertexArray(lampVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);

        glfwSwapBuffers(window);
    }

    glDeleteVertexArrays(1, &objectVAO);
    glDeleteVertexArrays(1, &lampVAO);
    glDeleteBuffers(1, &VBO);

    glfwTerminate();
    return 0;
}

// Is called whenever a key is pressed/released via GLFW
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) {
    if (action == GLFW_PRESS) {
        keys[key] = true;
    } else if (action == GLFW_RELEASE) {
        keys[key] = false;
    }

    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
}

// Change camera position when WSAD is pressed
void keyboard_movement() {
    // deltaTime makes sure same behavior in different machine
    GLfloat cameraSpeed = 5.0f * deltaTime;

    // Move camera position
    if (keys[GLFW_KEY_W]) {
        cameraPosition += cameraSpeed * cameraFront;
    } else if (keys[GLFW_KEY_S]) {
        cameraPosition -= cameraSpeed * cameraFront;
    } else if (keys[GLFW_KEY_A]) {
        cameraPosition -= cameraSpeed * glm::normalize(glm::cross(cameraFront, cameraUp)); // to get right vector
    } else if (keys[GLFW_KEY_D]) {
        cameraPosition += cameraSpeed * glm::normalize(glm::cross(cameraFront, cameraUp)); // to get right vector
    }
}

// Is called whenever the mouse is moving via GLFW
void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    // Prevent bumping at the beginning
    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    // Calculate offsets
    GLfloat xoffset = (xpos - lastX) * Sensitivity; // x-coordinate is relative to the left margin
    GLfloat yoffset = (lastY - ypos) * Sensitivity; // y-coordinate is relative to the top margin

    // Update last mouse position
    lastX = xpos;
    lastY = ypos;

    // Calculate Yaw and Pitch
    yaw += xoffset;
    pitch += yoffset;

    // Prevent weird camera movements
    if (pitch > 89.0f) {
        pitch = 89.0f;
    } else if (pitch < -89.0f) {
        pitch = -89.0f;
    }

    // The main idea is to change the front vector: front vector is the direction the camera looking at.
    glm::vec3 front;
    front.x = std::cos(glm::radians(pitch)) * std::sin(glm::radians(yaw));
    front.y = std::sin(glm::radians(pitch));
    front.z = std::cos(glm::radians(pitch)) * (-std::cos(glm::radians(yaw)));
    cameraFront = glm::normalize(front);
}