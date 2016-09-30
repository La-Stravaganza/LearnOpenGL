/*
 * 320322 Graphics and Visualization
 * Homework 2
 * Jinbo Zhang
 * jin.zhang@jacobs-university.de
 */

#include <GL/gl.h>
#include <GL/glut.h>
#include <glm/glm.hpp>
#include <global.h>
#include <callbacks.h>

void display_callback( ) {
    using namespace global;

    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity( );

    glm::vec3 camera_lookat = camera_position + camera_front;

    gluLookAt( camera_position.x, camera_position.y, camera_position.z,
               camera_lookat.x, camera_lookat.y, camera_lookat.z,
               camera_up.x, camera_up.y * glm::cos(phi), camera_up.z );  // flip the up vector if needed

    // beak
    glColor3fv( global::orange );
    glPushMatrix( );
    glTranslatef( -0.47f, 0.5f, 0.0f );
    glScalef( 0.2f, 0.3f, 0.6f );
    glutSolidSphere( 0.25f, 100, 100 );
    glPopMatrix( );

    // head
    glColor3fv( global::yellow );
    glPushMatrix( );
    glTranslatef( -0.2f, 0.5f, 0.0f );
    glutSolidSphere( 0.25f, 200, 200 );
    glPopMatrix( );

    // body
    glPushMatrix( );
    glTranslatef( 0.05f, 0.07f, 0.0f );
    glScalef( 1.0f, 0.5f, 0.6f );
    glutSolidSphere( 0.53f, 200, 200 );
    glPopMatrix( );

    // tail
    glPushMatrix( );
    glTranslatef( 0.4f, 0.18f, 0.0f );
    glutSolidSphere( 0.15f, 100, 100 );
    glPopMatrix( );

    // eye1
    glColor3fv( global::black );
    glPushMatrix( );
    glTranslatef( -0.35f, 0.65f, 0.1f );
    glutSolidSphere( 0.03f, 100, 100 );
    glPopMatrix( );

    // eye2
    glPushMatrix( );
    glTranslatef( -0.35f, 0.65f, -0.1f );
    glutSolidSphere( 0.03f, 100, 100 );
    glPopMatrix( );

    // water
    glColor3fv( global::skyblue );
    glPushMatrix( );
    glTranslatef( 0.0f, -0.27f, 0.0f );
    glScalef( 1.0f, 0.2f, 1.0f );
    glutSolidCube( 1.5f );
    glPopMatrix( );

    glutSwapBuffers( );
}

void reshape_callback( int w, int h ) {
    h = h > 0 ? h : 1;
    GLfloat ratio = 1.0f * w / h;
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity( );
    glViewport( 0, 0, w, h );
    gluPerspective( 45.0f, ratio, 0.1f, 1000.0f );
}

void keyboard_callback( unsigned char key, int x, int y ) {
    using namespace global;

    float radius = glm::length(camera_position);

    switch ( key ) {
        case 'w':
            camera_position += key_speed * camera_front;
            break;
        case 's':
            camera_position -= key_speed * camera_front;
            break;
        case 'a': // to get right vector
            camera_position -= key_speed * glm::normalize( glm::cross( camera_front, camera_up ) );
            break;
        case 'd': // to get right vector
            camera_position += key_speed * glm::normalize( glm::cross( camera_front, camera_up ) );
            break;
        case 'j':
            theta -= rotate_speed;
            camera_position = glm::vec3(radius * glm::cos(phi) * glm::sin(theta),
                                        camera_position.y,
                                        radius * glm::cos(phi) * glm::cos(theta));
            camera_front = - camera_position; // to look at (0, 0, 0)
            break;
        case 'l':
            theta += rotate_speed;
            camera_position = glm::vec3(radius * glm::cos(phi) * glm::sin(theta),
                                        camera_position.y,
                                        radius * glm::cos(phi) * glm::cos(theta));
            camera_front = - camera_position;
            break;
        case 'i':
            phi += rotate_speed;
            camera_position = glm::vec3(radius * glm::cos(phi) * glm::sin(theta),
                                        radius * glm::sin(phi),
                                        radius * glm::cos(phi) * glm::cos(theta));
            camera_front = - camera_position;
            break;
        case 'k':
            phi -= rotate_speed;
            camera_position = glm::vec3(radius * glm::cos(phi) * glm::sin(theta),
                                        radius * glm::sin(phi),
                                        radius * glm::cos(phi) * glm::cos(theta));
            camera_front = - camera_position;
            break;
        case 27:
            exit( 0 );
    }

    glutPostRedisplay( );
}
