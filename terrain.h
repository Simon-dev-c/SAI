#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string.h>
#include "OpenGL/gl.h"
#include "GLUT/glut.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


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
    point p;
    int rayon;
}boule;

typedef struct {
    char* nom_fichier;
    int width;
    int height;
    int channels;
    unsigned char *data;
    int x;
    int y;
    int z;
}image;

image img1;


// Pour la qualité des sphères
int longitude = 20;
int latitude = 20;

// Tableau des murs pour gerer les collisions
#define MAX_MUR 20000

mur tableau_mur[MAX_MUR];
int nb_mur = 0;

// Tableau des boules pour gerer les collisions
#define MAX_BOULE 20000

boule tableau_boule[MAX_BOULE];
int nb_boule = 0;

boule tableau_objet_rammassable[MAX_BOULE];
int nb_objet_rammassable = 0;


mur porte_sortie;


void init_tableaux(){
    point p1,p2;
    p1.x = 0;
    p1.y = 0;
    p1.z = 0;
    p2.x = 0;
    p2.y = 0;
    p2.z = 0;
    mur m;
    m.p1 = p1;
    m.p2 = p2;
    boule b;
    b.p = p1;
    b.rayon = 0;
    for (int i = 0;i<MAX_MUR;i++){
        tableau_mur[i] = m;
    }
    for (int i = 0;i<MAX_BOULE;i++){
        tableau_boule[i] = b;
    }
    for (int i = 0;i<MAX_BOULE;i++){
        tableau_objet_rammassable[i] = b;
    }
}

void init_images(){
    img1.nom_fichier = "images/image.png";
    img1.x = 400;
    img1.y = 100;
    img1.z = 500;
    img1.width = 800;
    img1.height = 800;

}

void affiche_sol(int x1, int z1, int x2, int z2){
    glBegin(GL_QUADS);
        glColor3f(0.1f, 0.8f, 0.2f); // vert
        glVertex3f(x1, 0, z2);
        glVertex3f(x2, 0, z2);
        glVertex3f(x2, 0, z1);
        glVertex3f(x1, 0, z1);
    glEnd();
}

void creer_mur(int x1, int y1, int z1, int x2, int y2, int z2){
    if (nb_mur < MAX_MUR){
        point p1,p2;
        p1.x = x1;
        p1.y = y1;
        p1.z = z1;
        p2.x = x2;
        p2.y = y2;
        p2.z = z2;
        mur m;
        m.p1 = p1;
        m.p2 = p2;
        tableau_mur[nb_mur] = m;
        nb_mur++;
    }
}

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

void affiche_porte( int x1, int y1, int z1, int x2, int y2, int z2){
    glBegin(GL_QUADS);
    
    glVertex3f(x1, y1, z1);
    glVertex3f(x2, y1, z1);
    glVertex3f(x2, y1, z2);
    glVertex3f(x1, y1, z2);

    glVertex3f(x1, y2, z1);
    glVertex3f(x2, y2, z1);
    glVertex3f(x2, y2, z2);
    glVertex3f(x1, y2, z2);
    
    glVertex3f(x1, y1, z1);
    glVertex3f(x1, y2, z1);
    glVertex3f(x1, y2, z2);
    glVertex3f(x1, y1, z2);

    glVertex3f(x2, y1, z1);
    glVertex3f(x2, y2, z1);
    glVertex3f(x2, y2, z2);
    glVertex3f(x2, y1, z2);

    glVertex3f(x1, y1, z1);
    glVertex3f(x2, y1, z1);
    glVertex3f(x2, y2, z1);
    glVertex3f(x1, y2, z1);

    glVertex3f(x1, y1, z2);
    glVertex3f(x2, y1, z2);
    glVertex3f(x2, y2, z2);
    glVertex3f(x1, y2, z2);

    glEnd();
}

void creer_boule(int rayon, int x, int y, int z){
    if (nb_boule < MAX_BOULE){
        point p;
        p.x = x;
        p.y = y;
        p.z = z;
        boule b;
        b.p = p;
        b.rayon = rayon;
        tableau_boule[nb_boule] = b;
        nb_boule++;
    }
}

void affiche_boule(int rayon, int x, int y, int z){
    GLUquadric* quad = gluNewQuadric();
    
    glPushMatrix();                 // sauvegarde la matrice

    glTranslatef(x, y, z);              // position (x, y, z)
    gluSphere(quad, rayon, longitude, latitude);       // dessin de la sphère

    glPopMatrix();                  // restaure la matrice
}

GLuint textureID;  // variable globale ou membre

// Chargement au début
void load_image_texture( image img ) {
    unsigned char *data = stbi_load(img.nom_fichier, &img.width, &img.height, &img.channels, 0);
    if (!data) {
        printf("Erreur chargement image\n");
        return;
    }

    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    // Choisir le bon format selon le nombre de canaux
    GLenum format;
    if (img.channels == 1)
        format = GL_RED;
    else if (img.channels == 3)
        format = GL_RGB;
    else if (img.channels == 4)
        format = GL_RGBA;
    else {
        printf("Nombre de canaux non supporté : %d\n", img.channels);
        stbi_image_free(data);
        return;
    }

    // Envoyer la texture au GPU
    glTexImage2D(GL_TEXTURE_2D, 0, format, img.width, img.height, 0, format, GL_UNSIGNED_BYTE, data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_image_free(data);
}

void draw_image(image img) {
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, textureID);

    glPushMatrix();
    glTranslatef(img.x, img.y, img.z);  // position dans l'espace 3D

    glColor3f(1.0f, 1.0f, 1.0f); // couleur neutre

    glBegin(GL_QUADS);
        glTexCoord2f(1, 1); glVertex3f(0, 0, 0);
        glTexCoord2f(0, 1); glVertex3f(img.width, 0, 0);
        glTexCoord2f(0, 0); glVertex3f(img.width, img.height, 0);
        glTexCoord2f(1, 0); glVertex3f(0, img.height, 0);
    glEnd();

    glPopMatrix();
    glDisable(GL_TEXTURE_2D);   // désactive la texture
}

void drawText(float x, float y, const char* text) {
    glRasterPos2f(x, y);

    for (const char* c = text; *c != '\0'; c++) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
    }
}

void creer_piece_ferme(point p1, point p2, int largeur_mur){
    // un coin
    creer_mur(p1.x,p1.y, p1.z,p2.x,p2.y, p1.z + largeur_mur);
    creer_mur(p1.x,p1.y, p1.z,p1.x + largeur_mur,p2.y, p2.z);

    // coin en face
    creer_mur(p2.x,p1.y, p2.z,p2.x,p2.y, p1.z + largeur_mur);
    creer_mur(p2.x,p1.y, p2.z,p1.x + largeur_mur,p2.y, p2.z);

    //dessous / dessus
    creer_mur(p1.x,p1.y, p1.z,p2.x,p1.y + largeur_mur, p2.z);
    creer_mur(p2.x,p2.y, p2.z,p1.x,p2.y + largeur_mur, p1.z);
}

void creer_mur_trouer(point p1, point p2, int largeur_mur, int largeur_trou){
    // Important : il faut que p2 - p1 >= 0 pour tout coordonnées
    /*
    ___________________________________________p2
    |           |                |            |
    |           |        B1      |            |
    |           O________________X            | _________p2 - (1/2)*(p2 - p1 - largeur_trou)
    |           |                |            |
    |     A1    |       trou     |      A2    |
    |           |                |            |
    |           X________________O            |
    |           |                |            |
    |           |        B2      |            |
    |___________|________________|____________|
    p1          |                |
                |           (.. + largeur_trou)
                |
                |
    p1 + (1/2)*(p2 - p1 - largeur_trou)
    */

    // trou = (1/2)*(p2 - p1 - largeur_trou)
    int x_trou_inf = (p2.x - p1.x - largeur_trou)/2;
    int y_trou_inf = (p2.y - p1.y - largeur_trou)/2;
    int z_trou_inf = (p2.z - p1.z - largeur_trou)/2;

    /* Pas besoin de calculer : géométrique donc on utilise l'autre point
    int x_trou_sup = (1/2)*(p2.x - p1.x + largeur_trou);
    int y_trou_sup = (1/2)*(p2.y - p1.y + largeur_trou);
    int z_trou_sup = (1/2)*(p2.z - p1.z + largeur_trou);
    */
    //printf("x trou inf : %d \n",x_trou_inf);
    // sens X
    if (p1.z == p2.z){
        // A1
        creer_mur(p1.x,p1.y,p1.z, p1.x + x_trou_inf, p2.y, p1.z + largeur_mur);
        // A2
        creer_mur(p2.x - x_trou_inf ,p1.y,p1.z, p2.x, p2.y, p1.z + largeur_mur);

        // B1
        creer_mur(p1.x + x_trou_inf , p2.y - y_trou_inf , p1.z, p2.x - x_trou_inf , p2.y, p1.z + largeur_mur);
        // B2
        creer_mur(p1.x + x_trou_inf ,p1.y,p1.z, p2.x - x_trou_inf, p1.y + y_trou_inf ,p1.z + largeur_mur);
    }else{
        // sens Z
        if (p1.x == p2.x){

            // A1
            creer_mur(p1.x,p1.y,p1.z, p1.x + largeur_mur, p2.y, p1.z + z_trou_inf);
            // A2
            creer_mur(p1.x ,p1.y,p2.z - z_trou_inf, p1.x + largeur_mur, p2.y, p2.z);

            // B1
            creer_mur(p1.x , p2.y - y_trou_inf , p1.z + z_trou_inf, p1.x + largeur_mur , p2.y, p2.z - z_trou_inf);
            // B2
            creer_mur(p1.x,p1.y,p1.z + z_trou_inf, p1.x + largeur_mur, p1.y + y_trou_inf ,p2.z - z_trou_inf);

        }else{
            if (p1.y == p2.y){
                // A1
                creer_mur(p1.x,p1.y,p1.z, p1.x + x_trou_inf, p1.y + largeur_mur, p2.z );
                // A2
                creer_mur(p2.x - x_trou_inf ,p1.y,p1.z, p2.x, p1.y + largeur_mur, p2.z);

                // B1
                creer_mur(p1.x + x_trou_inf, p1.y , p2.z - z_trou_inf , p2.x - x_trou_inf, p1.y + largeur_mur, p2.z);
                // B2
                creer_mur(p1.x + x_trou_inf, p1.y, p1.z, p2.x - x_trou_inf, p1.y + largeur_mur,p1.z + z_trou_inf);
            }
        }
    }

}

void creer_mur_avec_porte(point p1, point p2, int largeur_mur, int largeur_porte, int hauteur_porte){
    // Important : il faut que p2 - p1 >= 0 pour tout coordonnées
    /*
    ___________________________________________p2
    |           |                |            |
    |           |                |            |
    |           |                |            | _________p2 - (1/2)*(p2 - p1 - largeur_trou)
    |           |        B       |            |
    |     A1    |                |      A2    |
    |           |                |            |
    |           X________________O            |
    |           |                |            |
    |           |     porte      |            |
    |___________|________________|____________|
    p1          |                |
                |           (.. + largeur_trou)
                |
                |
    p1 + (1/2)*(p2 - p1 - largeur_trou)
    */


    // trou = (1/2)*(p2 - p1 - largeur_trou)
    int x_trou_inf = (p2.x - p1.x - largeur_porte)/2;
    int y_trou_inf = (p2.y - p1.y - largeur_porte)/2;
    int z_trou_inf = (p2.z - p1.z - largeur_porte)/2;

    /* Pas besoin de calculer : géométrique donc on utilise l'autre point
    int x_trou_sup = (1/2)*(p2.x - p1.x + largeur_trou);
    int y_trou_sup = (1/2)*(p2.y - p1.y + largeur_trou);
    int z_trou_sup = (1/2)*(p2.z - p1.z + largeur_trou);
    */
    //printf("x trou inf : %d \n",x_trou_inf);
    // sens X
    if (p1.z == p2.z){
        // A1
        creer_mur(p1.x,p1.y,p1.z, p1.x + x_trou_inf, p2.y, p1.z + largeur_mur);
        // A2
        creer_mur(p2.x - x_trou_inf ,p1.y,p1.z, p2.x, p2.y, p1.z + largeur_mur);

        // B
        creer_mur(p1.x + x_trou_inf , p1.y + hauteur_porte , p1.z, p2.x - x_trou_inf , p2.y, p1.z + largeur_mur);
    }else{
        // sens Z
        if (p1.x == p2.x){

            // A1
            creer_mur(p1.x,p1.y,p1.z, p1.x + largeur_mur, p2.y, p1.z + z_trou_inf);
            // A2
            creer_mur(p1.x ,p1.y,p2.z - z_trou_inf, p1.x + largeur_mur, p2.y, p2.z);

            // B
            creer_mur(p1.x , p1.y + hauteur_porte , p1.z + z_trou_inf, p1.x + largeur_mur , p2.y, p2.z - z_trou_inf);

        }else{
            if (p1.y == p2.y){
                // A1
                creer_mur(p1.x,p1.y,p1.z, p1.x + x_trou_inf, p1.y + largeur_mur, p2.z );
                // A2
                creer_mur(p2.x - x_trou_inf ,p1.y,p1.z, p2.x, p1.y + largeur_mur, p2.z);

                // B
                creer_mur(p1.x + x_trou_inf, p1.y , p1.z + largeur_porte , p2.x - x_trou_inf, p1.y + largeur_mur, p2.z);
            }
        }
    }

}

void creer_piece_avec_porte(point p1, point p2, int largeur_mur, int largeur_trou, int hauteur_porte, int trou_avant, int trou_gauche, int trou_droite, int trou_arriere, int trou_dessous, int trou_dessus){
    // pour chaque mur normal, on va avoir 4 murs qui vont representer le même mur qu'avant mais avec un trou au milieu
    // il faut que pour tout coordonnées : p2 - p1 >= 0

    point p1_mur, p2_mur;
    // un coin
    switch (trou_avant){
        case 2:
            p1_mur.x = p1.x;
            p1_mur.y = p1.y;
            p1_mur.z = p1.z;
            p2_mur.x = p2.x;
            p2_mur.y = p2.y;
            p2_mur.z = p1.z;
            creer_mur_avec_porte(p1_mur, p2_mur, largeur_mur, largeur_trou, hauteur_porte);
            break;
        case 1:
            p1_mur.x = p1.x;
            p1_mur.y = p1.y;
            p1_mur.z = p1.z;
            p2_mur.x = p2.x;
            p2_mur.y = p2.y;
            p2_mur.z = p1.z;
            creer_mur_trouer(p1_mur, p2_mur, largeur_mur, largeur_trou);
            break;
        case 0:
            creer_mur(p1.x,p1.y, p1.z,p2.x,p2.y, p1.z + largeur_mur);
            break;
    }
    switch (trou_gauche){
        case 2:
            p1_mur.x = p1.x;
            p1_mur.y = p1.y;
            p1_mur.z = p1.z;
            p2_mur.x = p1.x;
            p2_mur.y = p2.y;
            p2_mur.z = p2.z;
            creer_mur_avec_porte(p1_mur, p2_mur, largeur_mur, largeur_trou, hauteur_porte);
            break;
        case 1:
            p1_mur.x = p1.x;
            p1_mur.y = p1.y;
            p1_mur.z = p1.z;
            p2_mur.x = p1.x;
            p2_mur.y = p2.y;
            p2_mur.z = p2.z;
            creer_mur_trouer(p1_mur, p2_mur, largeur_mur, largeur_trou);
            break;
        case 0:
            creer_mur(p1.x,p1.y, p1.z,p1.x + largeur_mur,p2.y, p2.z);
            break;
    }

    // coin en face
    switch (trou_droite){
        case 2:
            p1_mur.x = p2.x;
            p1_mur.y = p1.y;
            p1_mur.z = p1.z;
            p2_mur.x = p2.x;
            p2_mur.y = p2.y;
            p2_mur.z = p2.z;
            creer_mur_avec_porte(p1_mur, p2_mur, largeur_mur, largeur_trou, hauteur_porte);
            break;
        case 1:
            p1_mur.x = p2.x;
            p1_mur.y = p1.y;
            p1_mur.z = p1.z;
            p2_mur.x = p2.x;
            p2_mur.y = p2.y;
            p2_mur.z = p2.z;
            creer_mur_trouer(p1_mur, p2_mur, largeur_mur, largeur_trou);
            break;
        case 0:
            creer_mur(p2.x,p1.y, p2.z,p2.x,p2.y, p1.z + largeur_mur);
            break;
    }
    switch (trou_arriere){
        case 2:
            p1_mur.x = p1.x;
            p1_mur.y = p1.y;
            p1_mur.z = p2.z;
            p2_mur.x = p2.x;
            p2_mur.y = p2.y;
            p2_mur.z = p2.z;
            creer_mur_avec_porte(p1_mur, p2_mur, largeur_mur, largeur_trou, hauteur_porte);
            break;
        case 1:
            p1_mur.x = p1.x;
            p1_mur.y = p1.y;
            p1_mur.z = p2.z;
            p2_mur.x = p2.x;
            p2_mur.y = p2.y;
            p2_mur.z = p2.z;
            creer_mur_trouer(p1_mur, p2_mur, largeur_mur, largeur_trou);
            break;
        case 0:
            creer_mur(p2.x,p1.y, p2.z,p1.x + largeur_mur,p2.y, p2.z);
            break;
    }

    //dessous / dessus
    switch (trou_dessous){
        case 2:
            p1_mur.x = p1.x;
            p1_mur.y = p1.y;
            p1_mur.z = p1.z;
            p2_mur.x = p2.x;
            p2_mur.y = p1.y;
            p2_mur.z = p2.z;
            creer_mur_avec_porte(p1_mur, p2_mur, largeur_mur, largeur_trou, hauteur_porte);
            break;
        case 1:
            p1_mur.x = p1.x;
            p1_mur.y = p1.y;
            p1_mur.z = p1.z;
            p2_mur.x = p2.x;
            p2_mur.y = p1.y;
            p2_mur.z = p2.z;
            creer_mur_trouer(p1_mur, p2_mur, largeur_mur, largeur_trou);
            break;
        case 0:
            creer_mur(p1.x,p1.y, p1.z,p2.x,p1.y + largeur_mur, p2.z);
            break;
    }
    switch (trou_dessus){
        case 2:
            p1_mur.x = p1.x;
            p1_mur.y = p2.y;
            p1_mur.z = p1.z;
            p2_mur.x = p2.x;
            p2_mur.y = p2.y;
            p2_mur.z = p2.z;
            creer_mur_avec_porte(p1_mur, p2_mur, largeur_mur, largeur_trou, hauteur_porte);
            break;
        case 1:
            p1_mur.x = p1.x;
            p1_mur.y = p2.y;
            p1_mur.z = p1.z;
            p2_mur.x = p2.x;
            p2_mur.y = p2.y;
            p2_mur.z = p2.z;
            creer_mur_trouer(p1_mur, p2_mur, largeur_mur, largeur_trou);
            break;
        case 0:
            creer_mur(p2.x,p2.y, p2.z,p1.x,p2.y + largeur_mur, p1.z);
    }
}

void creer_objet_rammassable(int rayon, point p1, point p2){
    total ++;

    // Emplacement aléatoire à l'intérieur
    int minX = (p1.x < p2.x) ? p1.x : p2.x;
    int maxX = (p1.x > p2.x) ? p1.x : p2.x;
    minX += rayon * 2;
    maxX -= rayon * 2;
    if (minX > maxX){
        total --;
        return;
    }

    int minY = (p1.y < p2.y) ? p1.y : p2.y;
    int maxY = (p1.y > p2.y) ? p1.y : p2.y;
    minY += rayon;
    maxY -= rayon;
    if (minY > maxY){
        total --;
        return;
    }

    int minZ = (p1.z < p2.z) ? p1.z : p2.z;
    int maxZ = (p1.z > p2.z) ? p1.z : p2.z;
    minZ += rayon;
    maxZ -= rayon;
    if (minZ > maxZ){
        total --;
        return;
    }


    int x = minX + rand() % (maxX - minX + 1);
    int y = minY + rand() % (maxY - minY + 1);
    int z = minZ + rand() % (maxZ - minZ + 1);

    if (nb_objet_rammassable < MAX_BOULE){
        point p;
        p.x = x;
        p.y = y;
        p.z = z;
        boule b;
        b.p = p;
        b.rayon = rayon;
        tableau_objet_rammassable[nb_objet_rammassable] = b;
        nb_objet_rammassable++;
    }

}

void creer_porte(point p1, point p2){
    porte_sortie.p1 = p1;
    porte_sortie.p2 = p2;
}

void supprimer_porte(){
    porte_sortie.p1.x = 0;
    porte_sortie.p1.y = -100;
    porte_sortie.p1.z = 0;
    porte_sortie.p2.x = 0;
    porte_sortie.p2.y = -100;
    porte_sortie.p2.z = 0;
}


void creer_objets(){
    load_image_texture(img1);
    /*
    creer_mur(0,0, 50,5,300, 55);

    creer_mur(50,0,-50,70,300,-45);

    creer_mur(-50,0,-25,-80,300,-20);

    creer_mur(0,0,20,5,300,25);

    creer_boule(30,36,30,52);
    */
    
    point p1,p2;
    /*
    p1.x = -1000;p1.y = 0; p1.z = 1400;
    p2.x = -600;p2.y = 300; p2.z = 1800;
    creer_piece_ferme(p1,p2, 20);
    
    p1.x = -1000;p1.y = 0; p1.z = 1000;
    p2.x = -400;p2.y = 300; p2.z = 1000;
    creer_mur_trouer(p1,p2,20,200);

    p1.x = -1000;p1.y = 0; p1.z = 400;
    p2.x = -1000;p2.y = 300; p2.z = 1000;
    creer_mur_trouer(p1,p2,20,50);

    p1.x = -1000;p1.y = 0; p1.z = 400;
    p2.x = -400;p2.y = 0; p2.z = 1000;
    creer_mur_trouer(p1,p2,20,150);

    p1.x = -1000;p1.y = 300; p1.z = 400;
    p2.x = -400;p2.y = 300; p2.z = 1000;
    creer_mur_trouer(p1,p2,20,40);

    
    p1.x = 0;p1.y = 0; p1.z = 1000;
    p2.x = 400;p2.y = 300; p2.z = 1400;
    creer_piece_avec_porte(p1, p2, 1, 30, 45, 2,2,2,2,2,2);
    
    p1.x = 130;p1.y = 80; p1.z = 600;
    p2.x = 250;p2.y = 200; p2.z = 1000;
    creer_piece_avec_porte(p1, p2, 1, 30, 45, 1,0,0,1,0,0);
    */
    p1.x = -50;p1.y = 0; p1.z = 1500;
    p2.x = 50;p2.y = 150; p2.z = 1500;
    creer_porte(p1,p2);
    

    p1.x = -800;p1.y = 0; p1.z = 1500;
    p2.x = 800;p2.y = 1500; p2.z = 3500;
    creer_piece_avec_porte(p1, p2, 1, 100, 150, 2,2,2,2,0,1);

    //ajout_total
    for (int i=0;i<1;i++){
        creer_objet_rammassable(20,p1,p2);
    }

    // couloir plafond
    p1.x = -50;p1.y = 1500; p1.z = 2450;
    p2.x = 50;p2.y = 2500; p2.z = 2550;
    creer_piece_avec_porte(p1, p2, 1, 100, 0, 0,0,0,0,1,1);

    // salle du dessus
    p1.x = -800;p1.y = 2500; p1.z = 1500;
    p2.x = 800;p2.y = 4500; p2.z = 3500;
    creer_piece_avec_porte(p1, p2, 1, 100, 150, 0,0,0,0,1,0);

    //ajout_total
    for (int i=0;i<0;i++){
        creer_objet_rammassable(20,p1,p2);
    }

    // couloir arriere
    p1.x = -50;p1.y = 0; p1.z = 3500;
    p2.x = 50;p2.y = 150; p2.z = 4000;
    creer_piece_avec_porte(p1, p2, 1, 100, 150, 2,0,0,2,0,0);

    // salle de l'arrière
    p1.x = -800;p1.y = 0; p1.z = 4000;
    p2.x = 800;p2.y = 1500; p2.z = 6000;
    creer_piece_avec_porte(p1, p2, 1, 100, 150, 2,0,0,0,0,0);

    //ajout_total
    for (int i=0;i<0;i++){
        creer_objet_rammassable(20,p1,p2);
    }

    
    p1.x = -1500;p1.y = 0; p1.z = 2000;
    p2.x = -800;p2.y = 1000; p2.z = 3000;
    creer_piece_avec_porte(p1, p2, 1, 100, 150, 0,0,2,0,0,0);
    
    p1.x = 800;p1.y = 0; p1.z = 2000;
    p2.x = 1500;p2.y = 1000; p2.z = 3000;
    creer_piece_avec_porte(p1, p2, 1, 100, 150, 0,2,0,0,0,0);

}

void afficher_objets(){
    for (int i = 0;i<nb_mur;i++){
        mur m = tableau_mur[i];
        affiche_mur(m.p1.x,m.p1.y,m.p1.z,m.p2.x,m.p2.y,m.p2.z);
    }

    for (int i = 0;i<nb_boule;i++){
        boule b = tableau_boule[i];
        affiche_boule(b.rayon,b.p.x,b.p.y,b.p.z);
    }

    // couleur jaune
    glColor3f(1.0f, 1.0f, 0.0f);
    for (int i = 0;i<nb_objet_rammassable;i++){
        boule b = tableau_objet_rammassable[i];
        affiche_boule(b.rayon,b.p.x,b.p.y,b.p.z);
    }

    // couleur rouge
    glColor3f(1.0f, 0.0f, 0.0f);
    affiche_porte(porte_sortie.p1.x,porte_sortie.p1.y,porte_sortie.p1.z,porte_sortie.p2.x,porte_sortie.p2.y,porte_sortie.p2.z);

}


//faire tout les différents affichage



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

int collision(){

    // vérifier que x_vue,y_vue,z_vue ne rentre pas en collision avec un objet
    // plusieurs façons de considérer le joueur

    for(int i = 0;i<nb_objet_rammassable;i++){
        boule b = tableau_objet_rammassable[i];
        //gerer collsision entre boule b et joueur
        // si collision mettre collision à 1
        if (collision_boule(b.p.x,b.p.y,b.p.z,b.rayon)){
            niveauEssence = MAX_ESSENCE;
            tableau_objet_rammassable[i].p.x = 0;
            tableau_objet_rammassable[i].p.y = -200;
            tableau_objet_rammassable[i].p.z = 0;
            tableau_objet_rammassable[i].rayon = 0;
            collected++;
            if (total == collected){
                supprimer_porte();
            }
            return 1;
        }
    }

    for(int i = 0;i<nb_mur;i++){
        mur b = tableau_mur[i];
        // gerer collision entre mur b et joueur
        // si collision mettre collision à 1
        if (collision_mur(b.p1.x,b.p1.y,b.p1.z,b.p2.x,b.p2.y,b.p2.z)){
            return 1;
        }
    }

    for(int i = 0;i<nb_boule;i++){
        boule b = tableau_boule[i];
        //gerer collsision entre boule b et joueur
        // si collision mettre collision à 1
        if (collision_boule(b.p.x,b.p.y,b.p.z,b.rayon)){
            return 1;
        }
    }

    if (collision_mur(porte_sortie.p1.x,porte_sortie.p1.y,porte_sortie.p1.z,porte_sortie.p2.x,porte_sortie.p2.y,porte_sortie.p2.z)){
        return 1;
    }


    return 0;
}