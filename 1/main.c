#include <stdlib.h>
#include <GLUT/glut.h>

GLdouble width, height;
int wd;                 /* GLUT window handle */

void init(void) {
    width = 1280.0;
    height = 800.0;
}

/* GLUT callbacks */

void display(void) {
    glClear(GL_COLOR_BUFFER_BIT);
    glFlush();
}

void reshape(int w, int h) {
    width = (GLdouble) w;
    height = (GLdouble) h;

    glViewport(0, 0, (GLsizei) width, (GLsizei) height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, width, 0.0, height, -1.f, 1.f);
}

int main(int argc, char **argv) {
    init();

    glutInit(&argc, argv);

    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);

    glutInitWindowSize((int) width, (int) height);

    wd = glutCreateWindow("Simplest possible GLUT program");

    glutReshapeFunc(reshape);
    glutDisplayFunc(display);

    glClearColor(0.0, 0.0, 0.5, 0.0);

    glutMainLoop();

    exit(0);
}
