#define GL_SILENCE_DEPRECATION
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string.h>
#include "OpenGL/gl.h"
#include "GLUT/glut.h"

int WindowSizeX = 1200;
int WindowSizeY = 800;

int zoom_fenetre = 10;

float r = 50;
float x_vue = 0,y_vue = 10,z_vue = 0;

int centre_z_cube = 55;

int upX = 0 , upY = 1 , upZ = 0;

float lookX = 0,lookY = 0,lookZ = 55;

// Deplacement vue par la souris
/* utiliser des angles :
On utilise généralement :
yaw → rotation gauche/droite
pitch → rotation haut/bas
*/

float yaw = 0.0f;
float pitch = 0.0f;
float sensibilite = 0.1f;


// derniere position de la souris
int lastX = -1;
int lastY = -1;


int liste_touche_enfonce[] = {0,0,0,0,0,0};
// gauche, droite, avant, arriere, haut, bas


// JEU

int niveauVie = 100;
int niveauEssence = 150;

#define MAX_VIE 100
#define MAX_ESSENCE 150




void tracerRectangle2D(int x1, int y1, int x2, int y2){
    glBegin(GL_QUADS);
    glVertex2f(x1,y1);
    glVertex2f(x1,y2);
    glVertex2f(x2,y2);
    glVertex2f(x2,y1);
    //glVertex2i(x+1,y+1);
    glEnd();
}

void affiche_cube( int x1, int y1, int z1, int x2, int y2, int z2){
    glBegin(GL_QUADS);
    // Rouge
    glColor3f(0.5, 0.5, 0.5);
    glVertex3f(x1, y1, z1);
    glVertex3f(x2, y1, z1);
    glVertex3f(x2, y1, z2);
    glVertex3f(x1, y1, z2);

    // Vert
    glColor3f(0.55, 0.55, 0.55);
    glVertex3f(x1, y2, z1);
    glVertex3f(x2, y2, z1);
    glVertex3f(x2, y2, z2);
    glVertex3f(x1, y2, z2);
    
    // Face Dessus
    glColor3f(0.58, 0.58, 0.58);
    glVertex3f(x1, y1, z1);
    glVertex3f(x1, y2, z1);
    glVertex3f(x1, y2, z2);
    glVertex3f(x1, y1, z2);

    // Face Dessous
    glColor3f(0.6, 0.6, 0.6);
    glVertex3f(x2, y1, z1);
    glVertex3f(x2, y2, z1);
    glVertex3f(x2, y2, z2);
    glVertex3f(x2, y1, z2);

    // Face droit
    glColor3f(0.45, 0.45, 0.45);
    glVertex3f(x1, y1, z1);
    glVertex3f(x2, y1, z1);
    glVertex3f(x2, y2, z1);
    glVertex3f(x1, y2, z1);

    // Face gauche
    glColor3f(0.475, 0.475, 0.475);
    glVertex3f(x1, y1, z2);
    glVertex3f(x2, y1, z2);
    glVertex3f(x2, y2, z2);
    glVertex3f(x1, y2, z2);

    glEnd();
}


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


void Affichage(){
    glClearColor(0.2f, 0.3f, 0.7f, 0.8f); // marron
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-5,5,-5,5,10,300);

    float visionActuX,visionActuY,visionActuZ = 0;
    visionActuX = x_vue + lookX;
    visionActuY = y_vue + lookY;
    visionActuZ = z_vue + lookZ;

    gluLookAt(x_vue,y_vue,z_vue, visionActuX, visionActuY, visionActuZ,upX,upY,upZ);

    // pour déplacer les axes dans un endroit visible
    visionActuX += lookX * 10;
    visionActuY += lookY * 10;
    visionActuZ += lookZ * 10;


    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Axes
    glBegin(GL_LINES);

        // Axe X en rouge
        glColor3f(1.0f, 0.0f, 0.0f);
        glVertex3f(-0.5f + visionActuX, visionActuY, visionActuZ);
        glVertex3f(0.5f + visionActuX, visionActuY, visionActuZ);

        // Axe Y en vert
        glColor3f(0.0f, 1.0f, 0.0f);
        glVertex3f(visionActuX, -0.5f + visionActuY, visionActuZ);
        glVertex3f(visionActuX, 0.5f + visionActuY, visionActuZ);

        // Axe Z en bleu
        glColor3f(0.0f, 0.0f, 1.0f);
        glVertex3f(visionActuX, visionActuY, -0.5f + visionActuZ);
        glVertex3f(visionActuX, visionActuY, 0.5f + visionActuZ);

    glEnd();

    glBegin(GL_QUADS);
        glColor3f(0.1f, 0.8f, 0.2f);
        glVertex3f(-1000, -10, 1000);
        glVertex3f(1000, -10, 1000);
        glVertex3f(1000, -10, -1000);
        glVertex3f(-1000, -10, -1000);
    glEnd();

    affiche_cube(-5,-5,centre_z_cube-5,5,5,centre_z_cube+5);

    // --- Passer en 2D ---
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();           // Sauve la projection 3D
    glLoadIdentity();
    gluOrtho2D(0, WindowSizeX, 0, WindowSizeY); // coordonnées pixels

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    int barreVieX = 20;
    int barreVieY = 20;

    int barreEssenceX = 20;
    int barreEssenceY = 40;

    glBegin(GL_QUADS);

        glColor3f(1.0,0.0,0.0); // rouge
        glVertex2f(barreVieX, barreVieY);
        glVertex2f(2 * niveauVie + barreVieX, barreVieY);
        glVertex2f(2 * niveauVie + barreVieX, 10 + barreVieY);
        glVertex2f(barreVieX, 10 + barreVieY);

        glColor3f(1.0,1.0,1.0); // blanc
        glVertex2f(barreEssenceX, barreEssenceY);
        glVertex2f(2 * niveauEssence + barreEssenceX, barreEssenceY);
        glVertex2f(2 * niveauEssence + barreEssenceX, 10 + barreEssenceY);
        glVertex2f(barreEssenceX, 10 + barreEssenceY);
    glEnd();

    glPopMatrix();            // Restaure modelview
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();            // Restaure projection 3D
    glMatrixMode(GL_MODELVIEW);


    // affiche tout les éléments :
    glutSwapBuffers();
}

/*
void Animer()
{
    // faire en fonction des touches enfoncées le mouvement correspondant
    if (liste_touche_enfoncé[0]){
        x_vue++;
    }
    if (liste_touche_enfoncé[1]){
        x_vue--;
    }
    if (liste_touche_enfoncé[2]){
        z_vue++;
    }
    if (liste_touche_enfoncé[3]){
        z_vue--;
    }
    if (liste_touche_enfoncé[4]){
        y_vue++;
    }
    if (liste_touche_enfoncé[5]){
        y_vue--;
    }

    glutPostRedisplay();
}*/
 void Animer()
{
    float vitesse = 0.5f;

    // vecteur avant (projection XZ)
    float forwardX = lookX;
    float forwardZ = lookZ;

    // normalisation simplifiée
    float length = sqrt(forwardX * forwardX + forwardZ * forwardZ);
    forwardX = forwardX / length;
    forwardZ = forwardZ / length;

    // vecteur droite
    float rightX = -forwardZ;
    float rightZ = forwardX;

    if (liste_touche_enfonce[2]) // Z
    {
        x_vue += forwardX * vitesse;
        z_vue += forwardZ * vitesse;
    }
    if (liste_touche_enfonce[3]) // S
    {
        x_vue -= forwardX * vitesse;
        z_vue -= forwardZ * vitesse;
    }
    if (liste_touche_enfonce[0]) // Q
    {
        x_vue -= rightX * vitesse;
        z_vue -= rightZ * vitesse;
    }
    if (liste_touche_enfonce[1]) // D
    {
        x_vue += rightX * vitesse;
        z_vue += rightZ * vitesse;
    }
    if (liste_touche_enfonce[4]){

        if (niveauEssence > 0){
            y_vue += vitesse;
            niveauEssence--;
         }
    }
    if (liste_touche_enfonce[5]) y_vue -= vitesse;

    calcul_direction();

    glutPostRedisplay();




}


int main(int argc, char* argv[]){
    srand(time(NULL));

    
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_SINGLE | GLUT_DEPTH);

    glutInitWindowSize(WindowSizeX, WindowSizeY);
    glutInitWindowPosition(50, 50);

    glutCreateWindow("Une␣maison");
    glEnable(GL_DEPTH_TEST);

    glutDisplayFunc(Affichage);

    glutIdleFunc(Animer);

    glutKeyboardFunc(GererClavier);

    glutKeyboardUpFunc(GererClavierRelache); // touche relâchée

    glutPassiveMotionFunc(MouvementSourisPassive);


    glutSetCursor(GLUT_CURSOR_NONE);

    glutMainLoop();
    return 0;
}