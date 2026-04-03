#define GL_SILENCE_DEPRECATION
#include "mouvement.h"
#include "terrain.h"

// Collision sphère–sphère
int collision_boule(float cx, float cy, float cz, float rayon_objet) {
    float dx = x_vue - cx;
    float dy = y_vue - cy;
    float dz = z_vue - cz;
    float dist2 = dx*dx + dy*dy + dz*dz;
    float somme = rayon_objet + RAYON_JOUEUR;
    return dist2 < somme * somme;
}

// Collision sphère–AABB (boîte)
int collision_mur(float x1, float y1, float z1, float x2, float y2, float z2) {
    float min_x = x1 < x2 ? x1 : x2;
    float max_x = x1 > x2 ? x1 : x2;
    float min_y = y1 < y2 ? y1 : y2;
    float max_y = y1 > y2 ? y1 : y2;
    float min_z = z1 < z2 ? z1 : z2;
    float max_z = z1 > z2 ? z1 : z2;

    float cx = x_vue < min_x ? min_x : (x_vue > max_x ? max_x : x_vue);
    float cy = y_vue < min_y ? min_y : (y_vue > max_y ? max_y : y_vue);
    float cz = z_vue < min_z ? min_z : (z_vue > max_z ? max_z : z_vue);

    float dx = x_vue - cx;
    float dy = y_vue - cy;
    float dz = z_vue - cz;
    float dist2 = dx*dx + dy*dy + dz*dz;

    float rayon = RAYON_JOUEUR + 7.0f;  // +10 = near plane de glFrustum
    return dist2 < rayon * rayon;
}

int collision() {
    // Sol à y=0
    if (y_vue < RAYON_JOUEUR + 10.0f) return 1;

    // Boule
    if (collision_boule(36, 30, 52, 30)) return 1;

    // Murs
    if (collision_mur(0,   0, 50,  5, 300,  55)) return 1;
    if (collision_mur(50,  0,-50, 70, 300, -45)) return 1;
    if (collision_mur(-50, 0,-25,-80, 300, -20)) return 1;
    if (collision_mur(0,   0, 20,  5, 300,  25)) return 1;

    return 0;
}


void Affichage(){
    glClearColor(0.2f, 0.3f, 0.7f, 0.8f); // marron
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-8,8,-5,5,10,3000);

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
    glVertex3f(-5000, 0,  5000);
    glVertex3f( 5000, 0,  5000);
    glVertex3f( 5000, 0, -5000);
    glVertex3f(-5000, 0, -5000);
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
void Animer() {
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
        if (collision()) {
            x_vue = x_tmp;
            z_vue = z_tmp;
        }
    }
    if (liste_touche_enfonce[3]) // S
    {
        x_vue -= forwardX * vitesse;
        z_vue -= forwardZ * vitesse;
        if (collision()) {
            x_vue = x_tmp;
            z_vue = z_tmp;
        }
    }
    if (liste_touche_enfonce[0]) // Q
    {
        x_vue -= rightX * vitesse;
        z_vue -= rightZ * vitesse;
        if (collision()) {
            x_vue = x_tmp;
            z_vue = z_tmp;
        }
    }
    if (liste_touche_enfonce[1]) // D
    {
        x_vue += rightX * vitesse;
        z_vue += rightZ * vitesse;
        if (collision()) {
            x_vue = x_tmp;
            z_vue = z_tmp;
        }
    }

    // Gravité appliquée chaque frame
    vitesse_y -= GRAVITE;

    // Au sol ?
    int au_sol = (y_vue <= SOL_Y + 0.5f);

    if (au_sol) {
        vitesse_y = 0;
        if (liste_touche_enfonce[4]) {
            vitesse_y = FORCE_SAUT;       // saut simple, pas de perte d'essence
        }
    } else if (liste_touche_enfonce[4] && niveauEssence > 0) {
        vitesse_y += FORCE_JETPACK;       // jetpack en l'air
        niveauEssence--;
    }

    // Appliquer la vitesse verticale
    y_tmp = y_vue;
    y_vue += vitesse_y;
    if (collision()) {
        y_vue = y_tmp;
        vitesse_y = 0;                    // atterrissage ou plafond
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