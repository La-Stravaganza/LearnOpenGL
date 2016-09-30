/*
 * 320322 Graphics and Visualization
 * Homework 2
 * Jinbo Zhang
 * jin.zhang@jacobs-university.de
 *
 * README:
 *
 * w, s, a, d to move the camera forward, backward, leftward and rightward
 * j, l to rotate horizontally
 * i, k to rotate vertically
 *
 */

#include <GL/gl.h>
#include <GL/glut.h>
#include <global.h>
#include <callbacks.h>

int main( int argc, char* argv[] ) {
    glutInit( &argc, argv );

    glutInitWindowSize( global::width, global::height );
    glutInitWindowPosition( 0, 0 );
    glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH );

    glutCreateWindow( "The Duck" );

    glClearColor( 0.6f, 0.6f, 0.6f, 0.0f );

    glutDisplayFunc( display_callback );
    glutReshapeFunc( reshape_callback );
    glutKeyboardFunc( keyboard_callback );

    glViewport( 0, 0, global::width, global::height );
    glMatrixMode( GL_PROJECTION );
    gluPerspective( 45.0f, 1.0f, 0.1f, 1000.0f );

    glEnable( GL_DEPTH_TEST );

    glutMainLoop( );
}
