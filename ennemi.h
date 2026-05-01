#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string.h>
#include "OpenGL/gl.h"
#include "GLUT/glut.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

// Pour la qualité des sphères
int longitude = 20;
int latitude = 20;

typedef struct {
    int x = 0;
    int y = 20;
    int z = 1000;
    int rayon = 20;
}ennemi;

ennemi e;


void affiche_boule(int rayon, int x, int y, int z){
    GLUquadric* quad = gluNewQuadric();
    
    glPushMatrix();                 // sauvegarde la matrice

    glTranslatef(x, y, z);              // position (x, y, z)
    gluSphere(quad, rayon, longitude, latitude);       // dessin de la sphère

    glPopMatrix();                  // restaure la matrice
}

void affiche_ennemi(){
    affiche_boule(e.rayon,e.x,e.y,e.z);
}

void vecteur_direction(){
    
}