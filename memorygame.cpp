#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <stdlib.h>

#include <stdio.h> // Para usar el sprintf

#include <ctype.h>
#include <sstream>
#include <iostream>
#include <stdlib.h>     /* srand, rand */
#include <time.h>
#include <string>

#ifndef CARD_H_INCLUDED
#define CARD_H_INCLUDED
#include <string>

class Card{
private:
    std::string value;
    int height;
    int width;
    double x;
    double y;
    bool hint;
    bool expuesta;
    bool print;
    
    void writeText(std::string text, int x, int y, void *font){
        glColor3ub(255,255,255); // output the string, in red
        int len, i;
        glRasterPos2f((GLfloat)x, (GLfloat)y);
        for (i = 0; i < text.length(); i++) {
            glutBitmapCharacter(font, text[i]);
        }
    }
    
    std::string intToString(int value) {
        std::stringstream ss;
        ss << value;
        return ss.str();
    }
    
public:
    Card(){
        value = "Null";
        height = 200;
        width = 200;
        x = 0;
        y = 0;
        expuesta = true;
        hint = true;
        print = false;
    }
    
    Card(std::string v, int h, int w, double x, double y)
    {
        value = v;
        height = h;
        width = w;
        this->x = x;
        this->y = y;
        expuesta = false;
        hint = true;
        print = false;
    }
    
    std::string getValue(){ return value;}
    int getHeight(){ return height;}
    int getWidth() { return width;}
    int getX() { return x;}
    int getY() { return y;}
    bool isExposed() {return expuesta;}
    std::string getAreaString(){
        return "Position x From: " + intToString(x) + " To: " + intToString(x + width) + "\n" +
        "Position y From: " + intToString(y - height) + " To : " + intToString(y) + "\n" +
        "Width: " + intToString(width) + " Height: " + intToString(height) + "\n";
    }
    
    void hintSwap(){ hint = !hint;}
    void setValue(std::string v) { this->value = v;}
    void setHeight(int h){ height = h;}
    void setWidth(int w){ width = w;}
    void setExpuesta(bool e) { expuesta = e;}
    void setPrint(bool p) { print = p;}
    void setPosition(int x, int y){this->x = x; this-> y = y;}
    
    bool itsOnMe(int xMouse, int yMouse)
    {
        return(xMouse >= x && xMouse <= x + width && yMouse >= y - height && yMouse <= y);
    }
    void draw()
    {
        if(expuesta){
            if(!print){
                glColor3ub(0, 175, 100);
            }else{
                glColor3ub(0, 85, 100);
            }
            glBegin(GL_POLYGON);
            glVertex2d( x        ,y - height);
            glVertex2d( x        ,y         );
            glVertex2d( x + width,y         );
            glVertex2d( x + width,y - height);
            glEnd();
            writeText(value, x + width / 2, y - height / 2, GLUT_BITMAP_TIMES_ROMAN_24);
        }else {
            glColor3ub(85, 15, 173);
            glBegin(GL_POLYGON);
            glVertex2d( x        ,y - height);
            glVertex2d( x        ,y         );
            glVertex2d( x + width,y         );
            glVertex2d( x + width,y - height);
            glEnd();
        }
        if(hint)
            writeText(value,x+width/2 -5,y - 10,GLUT_BITMAP_9_BY_15);
    }
};

#endif // CARD_H_INCLUDED


using namespace std;

char s[30];
double t;

bool detenido = true;
bool reseteado = true;
int timeAtStart = 0;
int timeAtPause = 0;
//Referente al estado del memorama
int estadoMemorama = 0;//default
int inCaOn;
int inCaTw;
int inCaTr;

//Cosas de cartas
int limitCards = 24;
Card arreglo[24];
int widthAux = 100;
int heightAux = 200;

int timeInit;
int timeFini;
bool waitForMultipleObjects = false; // variable para ver pareja equivocada
int cantidadPares = 0;
int cantidadIntentos = 0;
int cantidadMaximaPares = limitCards / 3;

int xMax = 300;
int xMin = -300;
int yMax = 300;
int yMin = -300;

//


string texto;
string paresEncontrados = "";
// Correspondiente al reloj

std::string intToString(int value) {
    std::stringstream ss;
    ss << value;
    return ss.str();
}
void writeText(std::string text, int x, int y, void *font){
    glColor3f(1,1,1); // output the string, in white
    glRasterPos2f((GLfloat)x, (GLfloat)y);
    for (unsigned int i = 0; i < text.length(); i++) {
        glutBitmapCharacter(font, text[i]);
    }
}
string formato(int i){
    string d = intToString( i % 10);
    i /= 10;
    string c = intToString( i % 10);
    i /=10;
    string b = intToString( i % 6);
    string a = intToString( i / 6 );
    return (a + ":" + b + c + ":" + d);
}

void resetCards(){
    //randomnize section
    srand (time(NULL));
    for(int i = 0; i < limitCards; i++){
        /* initialize random seed: */
        int aux = rand() % 16;//random numba from 0 to 15
        string v = arreglo[i].getValue();
        arreglo[i].setValue(arreglo[aux].getValue());
        arreglo[aux].setValue(v);
        arreglo[i].setExpuesta(false);
        
        
    }
    
    for(int i = 0; i < limitCards; i++){
        if(i % 8 == 0 && i !=0) {
            cout << endl;
        }
        cout << arreglo[i].getValue() << " ";
    }
    cout << endl << endl;
}

void inicia() {
    if(reseteado){
        reseteado = false;
        detenido = false;
        timeAtStart = glutGet(GLUT_ELAPSED_TIME);
    }else if(detenido){
        detenido = false;
        timeAtStart += (glutGet(GLUT_ELAPSED_TIME) - timeAtPause);
    }
}
void detiene() {
    timeAtPause = glutGet(GLUT_ELAPSED_TIME);
    detenido = true;
}
void resetea(){
    reseteado = true;
    resetCards();
    cantidadIntentos = 0;
    cantidadPares = 0;
    estadoMemorama = 0;
    paresEncontrados = "";
    t = 0.0;
}
void cronos(int value){ // Nuevo
    if(!detenido && !reseteado) {
        t = (glutGet(GLUT_ELAPSED_TIME) - timeAtStart) / 100;
    }
    int time = (int)t;
    texto = formato(time);
    sprintf(s, "%s", texto.c_str());
    glutTimerFunc(100, cronos, 1);
    glutPostRedisplay();
}
void resizeCards(){
    int xAux = 5;
    int yAux = 0;
    for(int i = 0; i < limitCards; i++)
    {
        if(i % 8 == 0 && i != 0){
            yAux += heightAux + 10;
            xAux = xMin + 5;
        }
        arreglo[i].setPosition(xAux,yMax - yAux);
        arreglo[i].setWidth(widthAux);
        arreglo[i].setHeight(heightAux);
        xAux += widthAux + 10;
    }
}
static void resize(int width, int height){
    glViewport (0, 0, (GLsizei) width, (GLsizei) height); // se reorganiza la vista con las nuevas dimensiones
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    
    widthAux = width / (limitCards / (limitCards / 8)) - 10;
    heightAux = height / 1.75  /(limitCards / 8) - 10;
    gluOrtho2D(0,width-1,0,height-1);
    xMin = 0;
    yMin = 0;
    xMax = width;
    yMax = height;
    
    resizeCards();
    
    glMatrixMode(GL_MODELVIEW);
}

static void display(void){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//Al inicio
    
    for(int i = 0; i < limitCards; i++)
    {
        arreglo[i].draw();
    }
    
    writeText(texto,10,10,GLUT_BITMAP_TIMES_ROMAN_24);
    writeText("Pasos: " + intToString(cantidadIntentos),120,10,GLUT_BITMAP_TIMES_ROMAN_24);
    writeText("Trios: " + intToString(cantidadPares),220,10,GLUT_BITMAP_TIMES_ROMAN_24);
    writeText("I-Iniciar P-Pausa R-Reiniciar H-Hacks! Esc-Salir",10,70,GLUT_BITMAP_TIMES_ROMAN_24);
    writeText("Tripletas encontradas: " + paresEncontrados,10,100,GLUT_BITMAP_TIMES_ROMAN_24);
    
    writeText("Jesus David De La Fuente Amaya A00811755",xMax - 270, 50,GLUT_BITMAP_HELVETICA_12);
    writeText("David Orlando Quilla A01138786",xMax - 270, 20,GLUT_BITMAP_HELVETICA_12);
    
    if(estadoMemorama == 3){
        int x1Aux = xMax / 2 - 150;
        int y1Aux = yMax / 2 - 100;
        int x2Aux = x1Aux + 360;
        int y2Aux = y1Aux + 50;
        glColor3b(255,255,255);
        glRectd(x1Aux,y1Aux,x2Aux,y2Aux);
        writeText("Felicidades, lo lograste en " + intToString(cantidadIntentos) + " pasos!!",x1Aux + 10,y1Aux + 20,GLUT_BITMAP_TIMES_ROMAN_24);
        detenido = true;
    }
    
    glutSwapBuffers();
}

void menu(int v){
    switch (v)
    {
            
        case 1: //Iniciar
            inicia();
            break;
        case 2: //Pausa
            detiene();
            break;
        case 3: //Reiniciar
            resetea();
            break;
        case 4: //Salir
            exit(0);
            break;
        case 5:
            for(int i = 0; i < limitCards; i++)
            {
                arreglo[i].hintSwap();
            }
            break;
    }
}

void init(void){
    glClearColor(20/255.0,242/255.0,234/255.0,1);
    //glClearColor(0.5,1.0,1.0,1.0);//color de la pantalla
    glColor3ub(200,50,100);//color de linea
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    
    
    int auxValue = 0;
    
    for(int i = 0; i < limitCards; i++)
    {
        if( i % 3 == 0)
            auxValue++;
        arreglo[i].setValue(intToString(auxValue));
    }
    resetCards();
    resizeCards();
    
    // sub menu
    int submenu;
    submenu=glutCreateMenu(menu);
    glutAddMenuEntry("Jesus David De La Fuente Amaya A00811755", 0);
    glutAddMenuEntry("David Orlando Quilla Murillo A01138786", 0);
    // Menu
    glutCreateMenu(menu);
    glutAddSubMenu("Autores", submenu);
    glutAddMenuEntry("Iniciar",1);
    glutAddMenuEntry("Pausa",2);
    glutAddMenuEntry("Reiniciar",3);
    glutAddMenuEntry("Mostrar/Esconder Ayuda",5);
    glutAddMenuEntry("Salir",4);
    glutAttachMenu(GLUT_RIGHT_BUTTON);
}


static void key(unsigned char key, int x, int y){
    switch (key)
    {
        case 27 : //Escape key Salir
        case 'Q':
        case 'q':
            exit(0);
            break;
        case 'i': //Iniciar
        case 'I':
            inicia();
            break;
        case 'r': case 'R': //Reiniciar
            resetea();
            break;
        case 'p': case 'P': //Pausa
            detiene();
            break;
        case 'H':
        case 'h':
            for(int i = 0; i < limitCards; i++)
            {
                arreglo[i].hintSwap();
            }
            break;
    }
    glutPostRedisplay();
}
static void idle(void){
    glutPostRedisplay();
}

void statesHandler(int x, int y){
    int xAux = x + xMin;
    int yAux = yMax - y;
    for(int i = 0 ; i < limitCards; i++){
        if(arreglo[i].itsOnMe(xAux,yAux))
        {
            if(arreglo[i].isExposed())//nada que ahcer con una carta ya activada
                break;
            switch(estadoMemorama){
                case 0: //ninguna carta con que aparear
                    arreglo[i].setExpuesta(true);
                    inCaOn = i;
                    estadoMemorama = 1;//avanzamos el estado
                    break;
                case 1: //una baraja expuesta y dimos click en otra
                    arreglo[i].setExpuesta(true);
                    inCaTw = i;
                    estadoMemorama = 2;
                    break;
                case 2:// dos barajas expuestas, dan click en otra
                    if(!(arreglo[inCaOn].getValue() == arreglo[inCaTw].getValue())){//si no son pares regresamos a su esado no expuesto
                        arreglo[inCaOn].setExpuesta(false);
                        arreglo[inCaTw].setExpuesta(false);
                        
                        estadoMemorama = 1;
                        inCaOn = i;
                        arreglo[inCaOn].setExpuesta(true);
                        cantidadIntentos++;
                    }else{//si hay un par expuesto
                        inCaTr = i;
                        arreglo[inCaTr].setExpuesta(true);
                        estadoMemorama = 4;
                        if(arreglo[inCaOn].getValue() == arreglo[inCaTr].getValue()){
                            cantidadIntentos++;
                            cantidadPares++;
                            paresEncontrados += arreglo[inCaOn].getValue() + " ";
                            arreglo[inCaOn].setPrint(true);
                            arreglo[inCaTw].setPrint(true);
                            arreglo[inCaTr].setPrint(true);
                            
                            if(cantidadPares == cantidadMaximaPares)
                                estadoMemorama = 3;
                        }
                    }
                    break;
                case 4:
                    if(!(arreglo[inCaOn].getValue() == arreglo[inCaTr].getValue())){
                        arreglo[inCaOn].setExpuesta(false);
                        arreglo[inCaTw].setExpuesta(false);
                        arreglo[inCaTr].setExpuesta(false);
                        cantidadIntentos++;
                    }
                    
                    estadoMemorama = 1;
                    inCaOn = i;
                    arreglo[inCaOn].setExpuesta(true);
                    break;
            }
            break;
        }
    }
}

void MouseClicks(int button, int state, int x, int y){
    if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        if(!detenido && !reseteado){
            statesHandler(x,y);
        }
    }
}
int main(int argc, char *argv[]){
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(xMax - xMin,yMax - yMin);
    glutInitWindowPosition(0, 0);
    glutCreateWindow("Examen Parcial 1 - Memorama");
    init();
    glutTimerFunc(100, cronos, 1); // Para el reloj
    glutReshapeFunc(resize);
    glutDisplayFunc(display);
    glutKeyboardFunc(key);
    glutMouseFunc(MouseClicks);
    glutIdleFunc(idle);
    glutMainLoop();
    return EXIT_SUCCESS;
}
