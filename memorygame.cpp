//Oscar Abraham Rodriguez Quintanilla
//Tania Garrido Salido
#define _USE_MATH_DEFINES
#ifdef __APPLE__
#include <GLUT/glut.h>#D1EAF0
#else
#include <GL/glut.h>
#endif

#include <stdlib.h>
#include <math.h>
#include <sstream>
#include <string>
#include <iostream>
GLsizei winWidth = 600, winHeight = 300;
void init(void){
    glClearColor(1.0,1.0,1.0,1.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, 100.0, 0.0, 100.0);
}

void randomBackground(){
    glClearColor(rand() / (RAND_MAX + 1.), rand() / (RAND_MAX + 1.), rand() / (RAND_MAX + 1.), 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
}

void randomGlColor3ub(){
    glColor3ub(rand() % 256, rand() % 256, rand() % 256);
}


void keyboardActions(unsigned char theKey, int mouseX, int mouseY){
    switch (theKey){
        case 27:
            exit(0);
            break;
        default:
            break;
    }
    glutPostRedisplay();
}

void game(void){
    glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
    glFlush();
}

int main(int argc, char** argv){
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(winWidth,winHeight);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Memory Game");
    init();
    glutDisplayFunc(game);
    glutKeyboardFunc(keyboardActions);
    glutMainLoop();
    return 0;
}