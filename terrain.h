#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string.h>
#include "OpenGL/gl.h"
#include "GLUT/glut.h"

typedef struct{
    int x;
    int y;
    int z;
}point;

typedef struct {
    point p1;
    point p2;
}mur;

typedef struct {

}boule;

// Pour la qualité des sphères
int longitude = 20;
int latitude = 20;


void affiche_mur( int x1, int y1, int z1, int x2, int y2, int z2){
    glBegin(GL_QUADS);
    
    glColor3f(0.5, 0.5, 0.5);
    glVertex3f(x1, y1, z1);
    glVertex3f(x2, y1, z1);
    glVertex3f(x2, y1, z2);
    glVertex3f(x1, y1, z2);

    glColor3f(0.55, 0.55, 0.55);
    glVertex3f(x1, y2, z1);
    glVertex3f(x2, y2, z1);
    glVertex3f(x2, y2, z2);
    glVertex3f(x1, y2, z2);
    
    glColor3f(0.58, 0.58, 0.58);
    glVertex3f(x1, y1, z1);
    glVertex3f(x1, y2, z1);
    glVertex3f(x1, y2, z2);
    glVertex3f(x1, y1, z2);

    glColor3f(0.6, 0.6, 0.6);
    glVertex3f(x2, y1, z1);
    glVertex3f(x2, y2, z1);
    glVertex3f(x2, y2, z2);
    glVertex3f(x2, y1, z2);

    glColor3f(0.45, 0.45, 0.45);
    glVertex3f(x1, y1, z1);
    glVertex3f(x2, y1, z1);
    glVertex3f(x2, y2, z1);
    glVertex3f(x1, y2, z1);

    glColor3f(0.475, 0.475, 0.475);
    glVertex3f(x1, y1, z2);
    glVertex3f(x2, y1, z2);
    glVertex3f(x2, y2, z2);
    glVertex3f(x1, y2, z2);

    glEnd();
}

void affiche_boule(int rayon, int x, int y, int z){
    GLUquadric* quad = gluNewQuadric();
    
    glPushMatrix();                 // sauvegarde la matrice

    glTranslatef(x, y, z);              // position (x, y, z)
    gluSphere(quad, rayon, longitude, latitude);       // dessin de la sphère

    glPopMatrix();                  // restaure la matrice
}

//faire tout les différents affichage