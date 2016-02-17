//Oscar Abraham Rodriguez Quintanilla
//Tania Garrido Salido
#define _USE_MATH_DEFINES
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <ctype.h>
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
bool terminado = false;
bool desplegar = true;
bool letras = false;
int turns = 0;
char deck[16] = { '0', '1', '2', '3', '4', '5', '6', '7', '0', '1', '2', '3', '4', '5', '6', '7' };
bool deck_display[16] = { false };
bool highlight_card[16] = { false };
GLsizei winWidth = 800, winHeight = 500;
int estados = 0;
int carta1 = -1;
int carta2 = -1;
int encontradas = 0;

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
    if (highlight_card[i]){
        glColor3ub(255, 255, 100);
    } else {
        if (deck_display[i] && !letras) {
            glColor3ub(255, 255, 255);
        } else if (i % 2 == 0){
            glColor3ub(150, 51, 184);
        } else{
            glColor3ub(85, 184, 51);
        }
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
    glColor3ub(204, 16, 85);
    glRasterPos2f(100, 70);
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
    glColor3ub(204, 16, 85);
    glRasterPos2f(1300, 70);
    for(int k = 0; k < turns.size(); k++){
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, turns[k]);
    }
}

void displayCards(){
    for (int i = 0; i < 16; i++){
        letras = false;
        cardColor(i);
        glBegin(GL_POLYGON);
        glVertex2f(i * 100, 500);
        glVertex2f(i * 100 + 100, 500);
        glVertex2f(i * 100 + 100, 400);
        glVertex2f(i * 100, 400);
        glEnd();
        if (deck_display[i]){
            letras = true;
            cardColor(i+1);
            glPushMatrix();
            glScalef(0.85f, 0.85f, 1);
            glTranslatef(117.647 * i + 13 , 475, 1);
            glutStrokeCharacter(GLUT_STROKE_ROMAN , deck[i]);
            glPopMatrix();
        }
    }
}

void displayInstructions(){
    if (desplegar) {
        char start[10] = "I-Iniciar";
        char pause[10] = "P-Pausa";
        char reset[15] = "R-Reiniciar";
        char esc[10] = "Esc-Salir";
        char tania[40] = "Tania Garrido - A01138941";
        char abraham[40] = "Abraham Rodríguez - A01195653";
        
        glColor3ub(150, 51, 184);
        glRasterPos2f(350, 150);
        for(int k = 0; start[k] != '\0'; k++){
            glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, start[k]);
        }
        glRasterPos2f(550, 150);
        for(int k = 0; pause[k] != '\0'; k++){
            glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, pause[k]);
        }
        glRasterPos2f(750, 150);
        for(int k = 0; reset[k] != '\0'; k++){
            glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, reset[k]);
        }
        glRasterPos2f(1000, 150);
        for(int k = 0; esc[k] != '\0'; k++){
            glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, esc[k]);
        }
        glRasterPos2f(100, 110);
        for(int k = 0; tania[k] != '\0'; k++){
            glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, tania[k]);
        }
        glRasterPos2f(850, 110);
        for(int k = 0; abraham[k] != '\0'; k++){
            glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, abraham[k]);
        }
        
        glColor3ub(51, 151, 184);
        int x = 70;
        for(int k = 0; deck[k] != '\0'; k++){
            glRasterPos2f(x, 405);
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, deck[k]);
            x+=100;
        }
    }
}

void keyboardActions(unsigned char theKey, int mouseX, int mouseY){
    switch (theKey){
        case 'a':
        case 'A':
            desplegar = !desplegar;
            break;
        case 'i':
        case 'I':
            if (!started){
                started = true;
                paused = false;
                glutTimerFunc(100, timePassBy, 0);
            }
            break;
        case 'r':
        case 'R':
            estados = 0;
            encontradas = 0;
            paused = true;
            started = false;
            tenthsOfASecond = 0;
            turns = 0;
            shuffle_deck();
            break;
        case 'p':
        case 'P':
            if (started){
                if (paused){
                    glutTimerFunc(100, timePassBy, 0);
                }
                paused = !paused;
            }
            break;
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

void passiveMotion(int x, int y){
    for (int i = 0; i < 16; i++){
        highlight_card[i] = false;
    }
    if (y > 0 && y < glutGet(GLUT_WINDOW_HEIGHT)/5 && (!paused)){
        highlight_card[cardNumber(x)] = true;
    }
    glutPostRedisplay();
}

void mouseActions(int button, int state, int x, int y){
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && started){
        if (y > 0 && y < glutGet(GLUT_WINDOW_HEIGHT)/5 && (!paused))
        {
            if(deck_display[cardNumber(x)] == true)//nada que ahcer con una carta ya activada
            {
                
            }
            else {
                switch(estados){
                    case 0: //ninguna carta con que aparear
                        deck_display[cardNumber(x)] = true;
                        carta1 = cardNumber(x);
                        estados = 1;//avanzamos el estado
                        break;
                    case 1: //una baraja expuesta y dimos click en otra
                        deck_display[cardNumber(x)] = true;
                        carta2 = cardNumber(x);
                        estados = 2;
                        if(deck[carta1] == deck[carta2]){
                            if(encontradas == 7){
                                estados = 3;
                                turns++;
                            }
                            encontradas++;
                        }
                        
                        break;
                    case 2:// dos barajas expuestas, dan click en otra
                        if(!(deck[carta1] == deck[carta2])){//si no son pares regresamos a su esado no expuesto
                            deck_display[carta1] = false;
                            deck_display[carta2] = false;
                            
                            estados = 1;
                            carta1 = cardNumber(x);
                            deck_display[cardNumber(x)] = true;
                            turns++;
                        }else{//si hay un par expuesto
                            estados = 1;
                            carta1 = cardNumber(x);
                            deck_display[cardNumber(x)] = true;
                            turns++;
                        }
                        break;
                }
                
            }
            
        }
        
    }
    glutPostRedisplay();
}

void game(void){
    glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);
    glClearColor(67.0/255.0, 183.0/255.0, 222.0/255.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
    displayTurns();
    displayTime();
    displayCards();
    displayInstructions();
    
    
    if(estados == 3){
        glColor4f(1,.7,.2,.2);
        glRectd(150,200,1450,350);
        std::ostringstream strTexto;
        strTexto << "¡Felicidades, lo lograste en ";
        strTexto << turns;
        strTexto << " pasos!";
        std::string ganaste = strTexto.str();
        glColor3ub(255,255,255);
        glRasterPos2f(450, 270);
        for(int k = 0; ganaste[k] != '\0'; k++){
            glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, ganaste[k]);
        }
        started = false;
        paused = true;
    }
    
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
    glutPassiveMotionFunc(passiveMotion);
    glutMouseFunc(mouseActions);
    glutMainLoop();
    return 0;
}
