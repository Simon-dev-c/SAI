all : clean base

base :
	gcc base.c -o base -framework OpenGL -framework GLUT

clean :
	rm -f base