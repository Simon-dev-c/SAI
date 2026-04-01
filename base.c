#define GL_SILENCE_DEPRECATION
#include "mouvement.h"
#include "terrain.h"

int collision(){

    // vérifier que x_vue,y_vue,z_vue ne rentre pas en collision avec un objet
    // plusieurs façons de considérer le joueur

    return 0; // false
}


void Affichage(){
    glClearColor(0.2f, 0.3f, 0.7f, 0.8f); // marron
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-5,5,-5,5,10,3000);

    float visionActuX,visionActuY,visionActuZ = 0;
    visionActuX = x_vue + lookX;
    visionActuY = y_vue + lookY;
    visionActuZ = z_vue + lookZ;

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(x_vue,y_vue,z_vue, visionActuX, visionActuY, visionActuZ,upX,upY,upZ);

    // pour déplacer les axes dans un endroit visible
    visionActuX += lookX * 10;
    visionActuY += lookY * 10;
    visionActuZ += lookZ * 10;

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
        glVertex3f(-1000, 0, 1000);
        glVertex3f(1000, 0, 1000);
        glVertex3f(1000, 0, -1000);
        glVertex3f(-1000, 0, -1000);
    glEnd();

    affiche_mur(0,0, 50,5,300, 55);

    affiche_mur(50,0,-50,70,300,-45);

    affiche_mur(-50,0,-25,-80,300,-20);

    affiche_mur(0,0,20,5,300,25);

    affiche_boule(30,36,30,52);

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


    float x_tmp = x_vue;
    float y_tmp = y_vue;
    float z_tmp = z_vue;

    if (liste_touche_enfonce[2]) // Z
    {
        x_vue += forwardX * vitesse;
        z_vue += forwardZ * vitesse;

        if (collision()){
            x_vue = x_tmp;
            z_vue = z_tmp;
        }
    }
    if (liste_touche_enfonce[3]) // S
    {
        x_vue -= forwardX * vitesse;
        z_vue -= forwardZ * vitesse;

        if (collision()){
            x_vue = x_tmp;
            z_vue = z_tmp;
        }
    }
    if (liste_touche_enfonce[0]) // Q
    {
        x_vue -= rightX * vitesse;
        z_vue -= rightZ * vitesse;

        if (collision()){
            x_vue = x_tmp;
            z_vue = z_tmp;
        }
    }
    if (liste_touche_enfonce[1]) // D
    {
        x_vue += rightX * vitesse;
        z_vue += rightZ * vitesse;

        if (collision()){
            x_vue = x_tmp;
            z_vue = z_tmp;
        }
    }
    if (liste_touche_enfonce[4]){

        if (niveauEssence > 0){
            y_vue += vitesse;
            niveauEssence--;

            if (collision()){
                y_vue = y_tmp;
            }
        }

    }
    if (liste_touche_enfonce[5]){
        y_vue -= vitesse;

        if (collision()){
            y_vue = y_tmp;
        }
    }

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