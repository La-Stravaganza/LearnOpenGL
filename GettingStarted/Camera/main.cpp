#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>
#include <string>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <SOIL/SOIL.h>
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"


// Function prototypes
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void keyboard_movement();
void mouse_callback(GLFWwindow* window, double xpos, double ypos);

// Window dimensions
const GLuint Width = 960, Height = 600;

// Variables about cameras
glm::vec3 cameraPosition = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

// Variable for key_callback
bool keys[1024];

// Variable for storing time between frames
GLfloat deltaTime = 0.0f;

// Variables for mouse_callback
bool firstMouse = true;
const GLfloat Sensitivity = 0.05;
GLfloat yaw = 0.0f;
GLfloat pitch = 0.0f;
double lastX = 0.0;
double lastY = 0.0;


int main() {
    /**
     * 1. Initialize GLFW, setup required options, create a window
     * 2. Set key callback functions
     * 3. Initialize GLEW
     * 4. Set a viewport
     * 5. Set up shaders
     * 6. Create vertices and cube positions
     * 7. Create VAO, VBO, and setup the bindings
     * 8. Set vertex attribute pointers
     * 9. Unbind VAO
     * 10. Create texture1, set parameters, load image, free resources
     * 11. So does texture2
     * 12. Enable depth testing
     * 13. Game Loop:
     *      13.1. Poll events
     *      13.2. Clear the color buffer and depth buffer
     *      13.3. Update the deltaTime and camera position (if WSAD is pressed)
     *      13.4. Use the shader program
     *      13.5. Send transformation matrix to the vertex shader
     *      13.6. Create view and projection matrix and send them to the vertex shader
     *      13.7. Active GL_TEXTURE0, bind texture1, correspond uniform variables
     *      13.8. So does texture2
     *      13.9. Bind VAO
     *      13.10. Draw cubes
     *      13.11. Unbind VAO
     *      13.12. Swap buffers
     * 14. Recycle resources (VAO, VBO)
     * 15. Terminate glfw
     */

    //******************************************************************************************************************

    // 1.1. Init GLFW
    glfwInit();

    // 1.2. Set all the required options for GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    // 1.3. Create a GLFW window object that we can use for GLFW's functions
    GLFWwindow* window = glfwCreateWindow(Width, Height, "LearnOpenGL", nullptr, nullptr);
    if (window == nullptr) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    // 1.4. Make window our current window
    glfwMakeContextCurrent(window);

    //******************************************************************************************************************

    // 2.1. Set the required callback functions
    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, mouse_callback);

    // 2.2. Capture the mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    //******************************************************************************************************************

    // 3.1. Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
    glewExperimental = GL_TRUE;
    // 3.2. Initialize GLEW to setup the OpenGL Function pointers
    if (glewInit() != GLEW_OK) {
        std::cout << "Failed to initialize GLEW" << std::endl;
        return -1;
    }

    //******************************************************************************************************************

    // 4. Define the viewport dimensions
    glViewport(0, 0, Width, Height);

    //******************************************************************************************************************

    // 5. Link shaders
    Shader aShader("vs.vert", "fs.frag");

    //******************************************************************************************************************

    // 6.1. Create vertices
    // Note: Texture could be distorted
    GLfloat vertices[] = {
            // position           // texture coordinates
            -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
             0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
             0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
             0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
            -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
             0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
             0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
             0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
            -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

            -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
            -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
            -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

             0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
             0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
             0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
             0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
             0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
             0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
             0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
             0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
             0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

            -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
             0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
             0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
             0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
            -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
            -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };

    // 6.2. Create 10 cubes in different positions
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

    //******************************************************************************************************************

    // 7.1.1. Create a vertex array object and bind it
    GLuint VAO;
    glGenVertexArrays(1, &VAO);

    // 7.1.2. Make VAO the "active" vertex array
    // i.e: opengl->vertex_array = VAO
    glBindVertexArray(VAO);

    // 7.2.1. Create a vertex buffer object
    GLuint VBO;
    glGenBuffers(1, &VBO);

    // 7.2.2. Make VBO the "active" array buffer
    // i.e: opengl->array_buffer = VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    // 7.2.3. Copy vertices into it
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    //******************************************************************************************************************

    // 8. Set the vertex attributes pointers

    // Attribute position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    // Attribute textureCoordinate
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    //******************************************************************************************************************

    // 9. Unbind VAO
    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); // TODO: Not OK, why?
    // glBindBuffer(GL_ARRAY_BUFFER, 0); // TODO: OK, why?
    glBindVertexArray(0);

    //******************************************************************************************************************

    // 10.1. Create texture1
    GLuint texture1;
    glGenTextures(1, &texture1);

    // 10.2. Bind texture1 to GL_TEXTURE_2D
    // i.e: opengl->GL_TEXTURE_2D = texture1
    glBindTexture(GL_TEXTURE_2D, texture1);

    // 10.3. Set texture1 wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // 10.4. Set texture1 filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // minify
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // magnify

    // 10.5. Load image
    int imageWidth, imageHeight;
    unsigned char* image = SOIL_load_image("doge.jpg", &imageWidth, &imageHeight, 0, SOIL_LOAD_RGB);

    // 10.6. Generate texture and mipmap
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imageWidth, imageHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);

    // 10.7. Free image and GL_TEXTURE_2D
    SOIL_free_image_data(image);
    glBindTexture(GL_TEXTURE_2D, 0);

    //******************************************************************************************************************

    // 11.1. Create texture2
    GLuint texture2;
    glGenTextures(1, &texture2);

    // 11.2. Bind texture2 to GL_TEXTURE_2D
    // i.e: opengl->GL_TEXTURE_2D = texture1
    glBindTexture(GL_TEXTURE_2D, texture2);

    // 11.3. Set texture2 wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // 11.4. Set texture2 filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // 11.5. Load image
    image = SOIL_load_image("longcat.jpg", &imageWidth, &imageHeight, 0, SOIL_LOAD_RGB);

    // 11.6. Generate texture and mipmap
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imageWidth, imageHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);

    // 11.7. Free image and GL_TEXTURE_2D
    SOIL_free_image_data(image);
    glBindTexture(GL_TEXTURE_2D, 0);

    //******************************************************************************************************************

    // 12. Enable depth testing
    glEnable(GL_DEPTH_TEST);

    //******************************************************************************************************************

    // 13. Game loop
    GLfloat lastFrame = 0.0f;

    while (glfwWindowShouldClose(window) == false) {
        // 13.1. Check if any events have been activated (key pressed, mouse moved etc.) and call corresponding response functions
        glfwPollEvents();

        // 13.2. Clear the color buffer and depth buffer
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // 13.3. Update the deltaTime and camera position (if WSAD is pressed)
        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        keyboard_movement();

        // 13.4. Use the shader program
        aShader.use();

        // 13.5. Get uniform variables' location from the vertex shader
        GLint modelLocation = glGetUniformLocation(aShader.getProgram(), "model");
        GLint viewLocation = glGetUniformLocation(aShader.getProgram(), "view");
        GLint projectionLocation = glGetUniformLocation(aShader.getProgram(), "projection");

        // 13.6.1. Create view and projection matrix
        glm::mat4 view = glm::lookAt(cameraPosition, cameraPosition + cameraFront, cameraUp);
        glm::mat4 projection;
        projection = glm::perspective(glm::radians(45.0f), (GLfloat)Width / (GLfloat)Height, 0.1f, 100.0f);

        // 13.6.2. send them to the vertex shader
        glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(projection));

        // 13.7.1. Active 1st texture unit Bind texture 1
        glActiveTexture(GL_TEXTURE0);

        // 13.7.2. Bind texture 1
        glBindTexture(GL_TEXTURE_2D, texture1);

        // 13.7.3. Corresponds variable uniformTexture1 in the shader
        glUniform1i(glGetUniformLocation(aShader.getProgram(), "texture1"), 0);

        // 13.8. So does texture 2
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);
        glUniform1i(glGetUniformLocation(aShader.getProgram(), "texture2"), 1);

        // 13.9. Bind VAO
        glBindVertexArray(VAO);

        // 13.10. Draw 10 cubes
        for (GLint i = 0; i < 10; ++i) {
            // 13.10.1. Create model matrix
            glm::mat4 model;

            // 13.10.2. Translate cube into desired position
            model = glm::translate(model, cubePositions[i]);

            // 13.10.3. Make it rotating
            model = glm::rotate(model, glm::radians((GLfloat)(20 * i)), glm::vec3(1.0f, 0.3f, 0.5f));

            // 13.10.4. Send it to the vertex shader
            glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));

            // 13.10.5. Do the actual drawing
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        // 13.11. Unbind VAO
        glBindVertexArray(0);

        // 13.12. Swap the screen buffers
        glfwSwapBuffers(window);
    }

    //******************************************************************************************************************

    // 14. De-allocate resources
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    //******************************************************************************************************************

    // 15. Terminate GLFW, clearing any resources allocated by GLFW.
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
