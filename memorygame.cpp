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
int turns = 0;
char deck[16] = { '0', '1', '2', '3', '4', '5', '6', '7', '0', '1', '2', '3', '4', '5', '6', '7' };
bool deck_display[16] = { false };
GLsizei winWidth = 1000, winHeight = 500;

void shuffle_deck(){
  for (int i = 0; i < 16; i++){
    deck_display[i] = false;
  }
  std::random_shuffle(&deck[0], &deck[16]);
}

void init(void){
  glClearColor(0.32, 0.6, 0.32, 1.0);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluOrtho2D(0.0, 1600.0, 0.0, 500.0);
  shuffle_deck();
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
    glColor3ub(255, 255, 255);
  } else{
    glColor3ub(32, 39, 200);
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
  cardColor(1);
  std::string time_formatted = format(tenthsOfASecond);
  glRasterPos2f(20, 20);
  for (int i = 0; i < time_formatted.size(); i++){
    glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, time_formatted[i]);
  }  
}

void displayTurns(){
  cardColor(1);
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
    glBegin(GL_POLYGON);
    glVertex2f(i * 100, 500);
    glVertex2f(i * 100 + 100, 500);
    glVertex2f(i * 100 + 100, 400);
    glVertex2f(i * 100, 400);
    glEnd();
    if (deck_display[i]){
      cardColor(i+1);
      glPushMatrix();
      glScalef(0.85f, 0.85f, 1);
      glTranslatef(117.647 * i + 13 , 475, 1);
      glutStrokeCharacter(GLUT_STROKE_ROMAN , deck[i]);
      glPopMatrix();
    }
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
    case 'r':
    case 'R':
      paused = true;
      started = false;
      tenthsOfASecond = 0;
      turns = 0;
      shuffle_deck();
      break;
    case 27:
      exit(0);
      break;
    default:
      break;// do nothing
  }
  glutPostRedisplay();
}

int cardNumber(int x){
  return x/(glutGet(GLUT_WINDOW_WIDTH)/16);
}

void mouseActions(int button, int state, int x, int y){
  if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && started){
    if (y > 0 && y < glutGet(GLUT_WINDOW_HEIGHT)/5){
      std::cout << "# CARD SELECTED: "<< cardNumber(x) << '\n';
      deck_display[cardNumber(x)] = true; // TANIA CON QUE LE PONGAS FALSE AL NUMERO DE CASILLA SE BORRA DE LA PANTALLA
    }
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
  glutMouseFunc(mouseActions);
  glutMainLoop();
  return 0;
}