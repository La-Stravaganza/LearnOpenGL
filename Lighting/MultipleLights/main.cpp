#include <iostream>
#include <cmath>
#include <string>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <SOIL/SOIL.h>
#define  GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"

using std::cout;
using std::endl;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void keyboard_movement();
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
std::string construct_point_light_name(const char* part1, int i, const char* part2);

const GLuint Width = 800, Height = 600;

glm::vec3 cameraPosition = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

glm::vec3 directionalLightDirection(-1.0f, -4.0f, -1.0f);

glm::vec3 spotLightPosition(0.0f, -0.5f, 4.0f);
glm::vec3 spotLightDirection(0.0f, 0.0f, -1.0f);

const int NumberOfPointLight = 4;

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
             // Positions         // Normals           // Texture Coordinates
            -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
             0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
             0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
             0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
            -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

            -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
             0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
             0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
             0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
            -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
            -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

            -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
            -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
            -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
            -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
            -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

             0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
             0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
             0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
             0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
             0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
             0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

            -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
             0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
             0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
             0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

            -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
             0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
             0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
             0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
            -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
            -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
    };

    glm::vec3 cubePositions[] = {
            glm::vec3( 0.0f,  0.0f,  0.0f),
            glm::vec3( 2.0f,  5.0f, -15.0f),
            glm::vec3(-1.5f, -2.2f, -2.5f),
            glm::vec3(-3.8f, -2.0f, -12.3f),
            glm::vec3( 2.4f, -0.4f, -3.5f),
            glm::vec3(-1.7f,  3.0f, -7.5f),
            glm::vec3( 1.3f, -2.0f, -2.5f),
            glm::vec3( 1.5f,  2.0f, -2.5f),
            glm::vec3( 1.5f,  0.2f, -1.5f),
            glm::vec3(-1.3f,  1.0f, -1.5f)
    };

    glm::vec3 pointLightPositions[] = {
            glm::vec3( 0.7f,  0.2f,  2.0f),
            glm::vec3( 2.3f, -3.3f, -4.0f),
            glm::vec3(-4.0f,  2.0f, -12.0f),
            glm::vec3( 0.0f,  0.0f, -3.0f)
    };

    // 7. Create and set object VAO and VBO
    GLuint objectVAO;
    glGenVertexArrays(1, &objectVAO);
    glBindVertexArray(objectVAO);

    GLuint VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
    glEnableVertexAttribArray(2);
    glBindVertexArray(0);

    // 8. Create and set light VAO
    GLuint lampVAO;
    glGenVertexArrays(1, &lampVAO);
    glBindVertexArray(lampVAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);

    // 9. Create diffuse map and specular map
    GLuint diffuseMap;
    glGenTextures(1, &diffuseMap);
    glBindTexture(GL_TEXTURE_2D, diffuseMap);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int imageWidth, imageHeight;
    unsigned char* image = SOIL_load_image("img/box.png", &imageWidth, &imageHeight, 0, SOIL_LOAD_RGB);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imageWidth, imageHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);

    SOIL_free_image_data(image);
    glBindTexture(GL_TEXTURE_2D, 0);

    GLuint specularMap;
    glGenTextures(1, &specularMap);
    glBindTexture(GL_TEXTURE_2D, specularMap);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    image = SOIL_load_image("img/boundary.png", &imageWidth, &imageHeight, 0, SOIL_LOAD_RGB);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imageWidth, imageHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);

    SOIL_free_image_data(image);
    glBindTexture(GL_TEXTURE_2D, 0);

    // 10. Enable depth test
    glEnable(GL_DEPTH_TEST);

    // 11. Game Loop
    GLfloat lastFrame = 0.0f;

    while (glfwWindowShouldClose(window) == GL_FALSE) {
        glfwPollEvents();

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        keyboard_movement();

        // object shader
        objectShader.use();

        // vertex shader's variables
        GLint modelLocation = glGetUniformLocation(objectShader.getProgram(), "model");
        GLint viewLocation = glGetUniformLocation(objectShader.getProgram(), "view");
        GLint projectionLocation = glGetUniformLocation(objectShader.getProgram(), "projection");

        glm::mat4 view = glm::lookAt(cameraPosition, cameraPosition + cameraFront, cameraUp);
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), (GLfloat)Width / (GLfloat)Height, 0.1f, 100.0f);

        glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(projection));

        // fragment shader's variables

        // material
        GLint materialDiffuseLocation = glGetUniformLocation(objectShader.getProgram(), "material.diffuse");
        GLint materialSpecularLocation = glGetUniformLocation(objectShader.getProgram(), "material.specular");
        GLint materialShininessLocation = glGetUniformLocation(objectShader.getProgram(), "material.shininess");
        glUniform1i(materialDiffuseLocation, 0);
        glUniform1i(materialSpecularLocation, 1);
        glUniform1f(materialShininessLocation, 64.0f);

        // directionalLight
        GLint directionalLightDirectionLocation = glGetUniformLocation(objectShader.getProgram(), "directionalLight.direction");
        GLint directionalLightAmbientLocation = glGetUniformLocation(objectShader.getProgram(), "directionalLight.ambient");
        GLint directionalLightDiffuseLocation = glGetUniformLocation(objectShader.getProgram(), "directionalLight.diffuse");
        GLint directionalLightSpecularLocation = glGetUniformLocation(objectShader.getProgram(), "directionalLight.specular");

        glm::vec4 directionalLightDirectionInViewSpace = view * glm::vec4(directionalLightDirection, 0.0f);
        glUniform3f(directionalLightDirectionLocation, directionalLightDirectionInViewSpace.x, directionalLightDirectionInViewSpace.y, directionalLightDirectionInViewSpace.z);
        glUniform3f(directionalLightAmbientLocation, 0.2f, 0.2f, 0.2f);
        glUniform3f(directionalLightDiffuseLocation, 0.5f, 0.5f, 0.5f);
        glUniform3f(directionalLightSpecularLocation, 1.0f, 1.0f, 1.0f);

        // pointLight
        for (int i = 0; i < NumberOfPointLight; ++i) {
            GLint pointLightPositionLocation = glGetUniformLocation(objectShader.getProgram(), construct_point_light_name("pointLights[", i, "].position").c_str());
            GLint pointLightAmbientLocation = glGetUniformLocation(objectShader.getProgram(), construct_point_light_name("pointLights[", i, "].ambient").c_str());
            GLint pointLightDiffuseLocation = glGetUniformLocation(objectShader.getProgram(), construct_point_light_name("pointLights[", i, "].diffuse").c_str());
            GLint pointLightSpecularLocation = glGetUniformLocation(objectShader.getProgram(), construct_point_light_name("pointLights[", i, "].specular").c_str());
            GLint pointLightConstantLocation = glGetUniformLocation(objectShader.getProgram(), construct_point_light_name("pointLights[", i, "].constant").c_str());
            GLint pointLightLinearLocation = glGetUniformLocation(objectShader.getProgram(), construct_point_light_name("pointLights[", i, "].linear").c_str());
            GLint pointLightQuadraticLocation = glGetUniformLocation(objectShader.getProgram(), construct_point_light_name("pointLights[", i, "].quadratic").c_str());

            glm::vec4 pointLightPositionInViewSpace = view * glm::vec4(pointLightPositions[i], 1.0f);
            glUniform3f(pointLightPositionLocation, pointLightPositionInViewSpace.x, pointLightPositionInViewSpace.y, pointLightPositionInViewSpace.z);
            glUniform3f(pointLightAmbientLocation, 0.2f, 0.2f, 0.2f);
            glUniform3f(pointLightDiffuseLocation, 0.5f, 0.5f, 0.5f);
            glUniform3f(pointLightSpecularLocation, 1.0f, 1.0f, 1.0f);
            glUniform1f(pointLightConstantLocation, 1.0f);
            glUniform1f(pointLightLinearLocation, 0.09f);
            glUniform1f(pointLightQuadraticLocation, 0.032f);
        }

        // spotLight
        GLint spotLightPositionLocation = glGetUniformLocation(objectShader.getProgram(), "spotLight.position");
        GLint spotLightDirectionLocation = glGetUniformLocation(objectShader.getProgram(), "spotLight.direction");
        GLint spotLightAmbientLocation = glGetUniformLocation(objectShader.getProgram(), "spotLight.ambient");
        GLint spotLightDiffuseLocation = glGetUniformLocation(objectShader.getProgram(), "spotLight.diffuse");
        GLint spotLightSpecularLocation = glGetUniformLocation(objectShader.getProgram(), "spotLight.specular");
        GLint spotLightCutoffLocation = glGetUniformLocation(objectShader.getProgram(), "spotLight.cutoff");
        GLint spotLightOuterCutoffLocation = glGetUniformLocation(objectShader.getProgram(), "spotLight.outerCutoff");

        spotLightPosition.x = 1.5f * glm::sin(currentFrame);
        glm::vec4 spotLightPositionInViewSpace = view * glm::vec4(spotLightPosition, 1.0f);
        glUniform3f(spotLightPositionLocation, spotLightPositionInViewSpace.x, spotLightPositionInViewSpace.y, spotLightPositionInViewSpace.z);
        glm::vec4 spotLightDirectionInViewSpace = view * glm::vec4(spotLightDirection, 0.0f);
        glUniform3f(spotLightDirectionLocation, spotLightDirectionInViewSpace.x, spotLightDirectionInViewSpace.y, spotLightDirectionInViewSpace.z);
        glUniform3f(spotLightAmbientLocation, 0.2f, 0.2f, 0.2f);
        glUniform3f(spotLightDiffuseLocation, 0.5f, 0.5f, 0.5f);
        glUniform3f(spotLightSpecularLocation, 1.0f, 1.0f, 1.0f);
        glUniform1f(spotLightCutoffLocation, glm::cos(glm::radians(12.5f)));
        glUniform1f(spotLightOuterCutoffLocation, glm::cos(glm::radians(15.0f)));

        // load texture
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, diffuseMap);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, specularMap);

        // drawing
        glBindVertexArray(objectVAO);
        for(GLuint i = 0; i < 10; i++) {
            glm::mat4 model;
            model = glm::translate(model, cubePositions[i]);
            GLfloat angle = 20.0f * i;
            model = glm::rotate(model, angle, glm::vec3(1.0f, 0.3f, 0.5f));
            glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
        glBindVertexArray(0);

        // lamp shader
        lampShader.use();

        // vertex shader's variables
        modelLocation = glGetUniformLocation(lampShader.getProgram(), "model");
        viewLocation = glGetUniformLocation(lampShader.getProgram(), "view");
        projectionLocation = glGetUniformLocation(lampShader.getProgram(), "projection");

        glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(projection));

        // drawing
        glBindVertexArray(lampVAO);
        for (int i = 0; i < NumberOfPointLight; ++i) {
            glm::mat4 lampModel;
            lampModel = glm::translate(lampModel, pointLightPositions[i]);
            lampModel = glm::scale(lampModel, glm::vec3(0.2f));
            glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(lampModel));
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
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
    front.x = glm::cos(glm::radians(pitch)) * glm::sin(glm::radians(yaw));
    front.y = glm::sin(glm::radians(pitch));
    front.z = glm::cos(glm::radians(pitch)) * (-glm::cos(glm::radians(yaw)));
    cameraFront = glm::normalize(front);
}

std::string construct_point_light_name(const char* part1, int i, const char* part2) {
    std::string string1(part1);
    std::string string2(part2);
    return string1 + std::to_string(i) + string2;
}
