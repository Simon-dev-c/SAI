#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string.h>
#include "OpenGL/gl.h"
#include "GLUT/glut.h"
#include "param_jeu.h"

// derniere position de la souris
int lastX = -1;
int lastY = -1;

int liste_touche_enfonce[] = {0,0,0,0,0,0};
// gauche, droite, avant, arriere, haut, bas


// Deplacement vue par la souris
/* utiliser des angles :
On utilise généralement :
yaw → rotation gauche/droite
pitch → rotation haut/bas
*/
float yaw = 90.0f;
float pitch = 0.0f;
float sensibilite = 0.1f;


// Direction où l'on regarde
float lookX = 0,lookY = 0,lookZ = 0;

void GererClavier(unsigned char touche, int x, int y)
{
    // gauche, droite, avant, arriere, haut, bas
    if (touche == 'q' ){
        liste_touche_enfonce[0] = 1;
    }
    if (touche == 'd' ){
        liste_touche_enfonce[1] = 1;
    }
    if (touche == 'z' ){
        liste_touche_enfonce[2] = 1;
    }
    if (touche == 's' ){
        liste_touche_enfonce[3] = 1;
    }
    if (touche == ' ' ){
        liste_touche_enfonce[4] = 1;
    }
    if (touche == 'c' ){
        liste_touche_enfonce[5] = 1;
    }
}

void GererClavierRelache(unsigned char touche, int x, int y)
{
    // gauche, droite, avant, arriere, haut, bas
    if (touche == 'q' ){
        liste_touche_enfonce[0] = 0;
    }
    if (touche == 'd' ){
        liste_touche_enfonce[1] = 0;
    }
    if (touche == 'z' ){
        liste_touche_enfonce[2] = 0;
    }
    if (touche == 's' ){
        liste_touche_enfonce[3] = 0;
    }
    if (touche == ' ' ){
        liste_touche_enfonce[4] = 0;
    }
    if (touche == 'c' ){
        liste_touche_enfonce[5] = 0;
    }
}

void MouvementSourisPassive(int x, int y)
{
    if (lastX == -1)
    {
        lastX = x;
        lastY = y;
        return;
    }

    int centreEcranX = WindowSizeX/2;
    int centreEcranY = WindowSizeY/2;

    float sourisX = (x - lastX) * sensibilite;
    float sourisY = (y - lastY) * sensibilite;

    lastX = x;
    lastY = y;

    
    // deplacement de lookX, lookY et lookZ

    /* utiliser des angles :
    On utilise généralement :
    yaw → rotation gauche/droite
    pitch → rotation haut/bas
    */

    // appliquer sensibilité
    yaw   += sourisX;
    pitch -= sourisY;

    // limiter le pitch (évite retournement)
    if (pitch > 89.0f) pitch = 89.0f;
    if (pitch < -89.0f) pitch = -89.0f;

    // il faut recentrer quand la souris sort
    if (x > WindowSizeX || x < 0 || y > WindowSizeY || y < 0){
        lastX = centreEcranX;
        lastY = centreEcranY;
        glutWarpPointer(centreEcranX, centreEcranY);
    }

    glutPostRedisplay();
}

void calcul_direction(){
    // conversion en radians (UNE seule fois)
    float yawRad   = yaw * M_PI / 180.0f;
    float pitchRad = pitch * M_PI / 180.0f;

    // pré-calculs
    float cosPitch = cos(pitchRad);

    // direction
    lookX = cos(yawRad) * cosPitch;
    lookY = sin(pitchRad);
    lookZ = sin(yawRad) * cosPitch;
}