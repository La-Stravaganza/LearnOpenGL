/*
 * 320322 Graphics and Visualization
 * Homework 2
 * Jinbo Zhang
 * jin.zhang@jacobs-university.de
 */

#include <glm/glm.hpp>
#include <global.h>

namespace global {
    double theta = 0.0f;
    double phi = 0.0f;

    glm::vec3 camera_position = glm::vec3( 0.0f, 0.0f, 3.0f );
    glm::vec3 camera_front = glm::vec3( 0.0f, 0.0f, -1.0f );
    glm::vec3 camera_up = glm::vec3( 0.0f, 1.0f, 0.0f );
}

