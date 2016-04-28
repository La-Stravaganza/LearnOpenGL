#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>
#include <string>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <SOIL/SOIL.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"


// Function prototypes
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

// Window dimensions
const GLuint WIDTH = 960, HEIGHT = 600;

int main() {
    /**
     * 1. Initialize GLFW, setup required options, create a window
     * 2. Set key callback functions
     * 3. Initialize GLEW
     * 4. Set a viewport
     * 5. Set up shaders
     * 6. Create vertices and indices
     * 7. Create VAO, VBO, EBO, and setup the bindings
     * 8. Set vertex attribute pointers
     * 9. Unbind VAO
     * 10. Create texture1, set parameters, load image, free resources
     * 11. So does texture2
     * 12. Define a transformation matrix
     * 13. Game Loop:
     *      13.1. Poll events
     *      13.2. Clear color buffer
     *      13.3. Send transformation matrix to the vertex shader
     *      13.4. Active GL_TEXTURE0, bind texture1, correspond uniform variables
     *      13.5. So does texture2
     *      13.6. Use the shader program
     *      13.7. Bind VAO
     *      13.8. Do the drawing
     *      13.9. Unbind VAO
     *      13.10. Swap buffers
     * 14. Recycle resources (VAO, VBO, EBO)
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
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "LearnOpenGL", nullptr, nullptr);
    if (window == nullptr) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    // 1.4. Make window our current window
    glfwMakeContextCurrent(window);

    //******************************************************************************************************************

    // 2. Set the required callback functions
    glfwSetKeyCallback(window, key_callback);

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
    glViewport(0, 0, WIDTH, HEIGHT);

    //******************************************************************************************************************

    // 5. Link shaders
    Shader aShader("vs.vert", "fs.frag");

    //******************************************************************************************************************

    // 6.1. Create vertices
    // Note: Texture could be distorted
    GLfloat vertices[] = {
          // position            color             texture coordinate
             0.8f,  0.8f,  0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // upper right corner
             0.8f, -0.8f,  0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // lower right corner
            -0.8f, -0.8f,  0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // lower left corner
            -0.8f,  0.8f,  0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f  // upper left corner
    };

    // 6.2. Create indices
    GLuint indices[] = {
            0, 1, 3,
            1, 2, 3
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

    // 7.3.1. Create an element buffer object and copy indices into it
    GLuint EBO;
    glGenBuffers(1, &EBO);

    // 7.3.2. Make EBO the "active" element array buffer
    // i.e: opengl->element_array_buffer = EBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

    // 7.3.3. Copy indices into it
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    //******************************************************************************************************************

    // 8. Set the vertex attributes pointers

    // Attribute position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    // Attribute color
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    // Attribute textureCoordinate
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
    glEnableVertexAttribArray(2);

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

    // 12. Define a transformation matrix
    glm::mat4 transformationMatrix;
    transformationMatrix = glm::scale(transformationMatrix, glm::vec3(0.5f, 0.5f, 0.0f)); // scale x0.5
    transformationMatrix = glm::rotate(transformationMatrix, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f)); // rotate 90 degree
    transformationMatrix = glm::translate(transformationMatrix, glm::vec3(0.2f, -0.2f, 0.0f)); //  translate 0.2f rightward, 0.2f downward

    //******************************************************************************************************************

    // 13. Game loop
    while (glfwWindowShouldClose(window) == false) {
        // 13.1. Check if any events have been activated (key pressed, mouse moved etc.) and call corresponding response functions
        glfwPollEvents();

        // 13.2. Clear the color buffer
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // 13.3. Send transformation matrix to the vertex shader
        GLint uniformTransformationLocation = glGetUniformLocation(aShader.getProgram(), "uniformTransformation");
        glUniformMatrix4fv(uniformTransformationLocation, 1, GL_FALSE, glm::value_ptr(transformationMatrix));

        // 13.4.1. Active 1st texture unit Bind texture 1
        glActiveTexture(GL_TEXTURE0);

        // 13.4.2. Bind texture 1
        glBindTexture(GL_TEXTURE_2D, texture1);

        // 13.4.3. Corresponds variable uniformTexture1 in the shader
        glUniform1i(glGetUniformLocation(aShader.getProgram(), "uniformTexture1"), 0);

        // 13.5. So does texture 2
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);
        glUniform1i(glGetUniformLocation(aShader.getProgram(), "uniformTexture2"), 1);

        // 13.6. Use the shader program
        aShader.use();

        // 13.7. Bind VAO
        glBindVertexArray(VAO);

        // 13.8. Do actual drawing
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); // draw a rectangle with glDrawElements

        // 13.9. Unbind VAO
        glBindVertexArray(0);

        // 13.10. Swap the screen buffers
        glfwSwapBuffers(window);
    }

    //******************************************************************************************************************

    // 14. De-allocate resources
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    //******************************************************************************************************************

    // 15. Terminate GLFW, clearing any resources allocated by GLFW.
    glfwTerminate();
    return 0;
}

// Is called whenever a key is pressed/released via GLFW
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) {
    std::cout << key << std::endl;
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
}
