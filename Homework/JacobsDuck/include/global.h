/*
 * 320322 Graphics and Visualization
 * Homework 2
 * Jinbo Zhang
 * jin.zhang@jacobs-university.de
 */

#pragma once

#include <GL/gl.h>
#include <glm/glm.hpp>

namespace global {
    const int width = 800;
    const int height = 800;

    const GLfloat yellow[] = { 1.0f, 1.0f, 0.0f };
    const GLfloat orange[] = { 1.0f, 0.55f, 0.0f };
    const GLfloat black[] = { 0.0f, 0.0f, 0.0f };
    const GLfloat skyblue[] = { 0.0f, 0.89f, 0.93f };

    const float key_speed = 0.1f;
    const float rotate_speed = 0.03f;

    extern double theta;
    extern double phi;

    extern glm::vec3 camera_position;
    extern glm::vec3 camera_front;
    extern glm::vec3 camera_up;
}
