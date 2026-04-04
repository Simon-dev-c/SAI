#define MAX_VIE 100
#define MAX_ESSENCE 300
#define RAYON_JOUEUR 5.0f

int WindowSizeX = 1400;
int WindowSizeY = 800;

int niveauVie = 100;
int niveauEssence = 300;

GLfloat nbPixelBarre = 1;

// Vecteur vers le haut
int upX = 0 , upY = 1 , upZ = 0;

// où l'on est (changer les variables pour changer où l'on apparait)
float x_vue = 0,y_vue = 30,z_vue = 0;

#define SOL_Y (RAYON_JOUEUR + 10.0f)
#define GRAVITE       0.3f
#define FORCE_SAUT    6.0f
#define FORCE_JETPACK 0.4f

float vitesse_y = 0.0f;

float vitesse = 1.6f;


GLdouble vision_proche = 10;
GLdouble vision_loin = 5000;