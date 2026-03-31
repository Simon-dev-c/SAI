all : clean base

base : base.c param_jeu.h mouvement.h terrain.h
	gcc base.c -o base -framework OpenGL -framework GLUT

clean :
	rm -f base