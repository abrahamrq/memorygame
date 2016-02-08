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
#include <algorithm>
// Tamaño inicial de la ventana
int tenthsOfASecond = 0;
bool paused = true;
bool started = false;
int turns = 3;
char deck[16] = { '0', '1', '2', '3', '4', '5', '6', '7', '0', '1', '2', '3', '4', '5', '6', '7' };
GLsizei winWidth = 1000, winHeight = 500;
void init(void){
  glClearColor(0.32, 0.6, 0.32, 1.0);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluOrtho2D(0.0, 1000.0, 0.0, 500.0);
  std::random_shuffle(&deck[0], &deck[16]);
}

void randomBackground(){
  glClearColor(rand() / (RAND_MAX + 1.), rand() / (RAND_MAX + 1.), rand() / (RAND_MAX + 1.), 1.0);
  glClear(GL_COLOR_BUFFER_BIT);
}

void randomGlColor3ub(){
  glColor3ub(rand() % 256, rand() % 256, rand() % 256);
}

void cardColor(int i){
  if (i % 2 == 0){
    glColor3ub(100, 200, 156);
  } else{
    glColor3ub(100, 200, 156);
  }
}

std::string format(int tenthsOfASecond){
  std::ostringstream buffer;
  int seconds = tenthsOfASecond / 10;
  int tenths = tenthsOfASecond % 10;
  int minutes = seconds / 60;
  seconds %= 60;
  buffer << minutes;
  buffer << ':';
  if (seconds < 10){
    buffer << '0';
  }
  buffer << seconds;
  buffer << ':';
  buffer << tenths;
  return buffer.str();
}

void timePassBy(int value){
  glutPostRedisplay();
  if (started) {
    tenthsOfASecond++;
  }
  if (!paused){
    glutTimerFunc(100, timePassBy, 0);
  }
}

void displayTime(){
  std::string time_formatted = format(tenthsOfASecond);
  glRasterPos2f(20, 20);
  for (int i = 0; i < time_formatted.size(); i++){
    glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, time_formatted[i]);
  }  
}

void displayTurns(){
  std::ostringstream buffer;
  buffer << "Turns: ";
  buffer << turns;
  std::string turns = buffer.str();
  glRasterPos2f(900, 20);
  for(int k = 0; k < turns.size(); k++){
    glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, turns[k]);
  }
}

void displayCards(){
  for (int i = 0; i < 16; i++){
    cardColor(i);
    glPushMatrix();
    glScalef(0.6f,0.6f,1);
    glTranslatef(100*i + 50, 600, 1);
    glutStrokeCharacter(GLUT_STROKE_ROMAN , deck[i]);
    glPopMatrix();
  }
}

void keyboardActions(unsigned char theKey, int mouseX, int mouseY){
  switch (theKey){
    case 's':
    case 'S':
      if (!started){
        started = true;
        paused = false;
        glutTimerFunc(100, timePassBy, 0);
      }
      break;
    case 27:
      exit(0);
      break;
    default:
      break;// do nothing
  }
  glutPostRedisplay();
}

void game(void){
  glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);
  glClearColor(0.32, 0.6, 0.32, 1.0);
  glClear(GL_COLOR_BUFFER_BIT);
  displayTurns();
  displayTime();
  displayCards();
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