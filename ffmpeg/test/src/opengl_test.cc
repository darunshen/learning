#include <GL/freeglut.h>
#include <iostream>
#include <vector>
using namespace std;

GLfloat l_x = 0.0f, l_y = 0.0f, l_z = 0.0f;
GLfloat step = 0.005;

void init() {
  glLoadIdentity();
  glClearColor(0.0, 0.0, 0.0, 0.0);
}

void Draw(void) {
  glClear(GL_COLOR_BUFFER_BIT);

  glPushMatrix();
  glTranslatef(l_x, l_y, l_z);

  glBegin(GL_QUADS);
  glColor3f(1.0f, 0.0f, 0.0f);
  glVertex2f(-0.5f, 0.5f);
  glColor3f(0.0f, 1.0f, 0.0f);
  glVertex2f(0.5f, 0.5f);
  glColor3f(0.0f, 0.0f, 1.0f);
  glVertex2f(0.5f, -0.5f);
  glColor3f(0.5f, 0.5f, 0.5f);
  glVertex2f(-0.5f, -0.5f);
  glEnd();

  glPopMatrix();
  glutSwapBuffers();
}
void KeyEvent(int key, int x, int y) {
  switch (key) {
    case GLUT_KEY_LEFT:
      l_x -= step;
      break;
    case GLUT_KEY_RIGHT:
      l_x += step;
      break;
    case GLUT_KEY_UP:
      l_y += step;
      break;
    case GLUT_KEY_DOWN:
      l_y -= step;
      break;
    default:
      break;
  }

  glutPostRedisplay();
  return;
}

int main(int argc, char *argv[]) {
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
  glutInitWindowPosition(200, 100);
  glutInitWindowSize(800, 600);
  glutCreateWindow("test");

  init();

  glutDisplayFunc(Draw);
  glutSpecialFunc(KeyEvent);
  glutMainLoop();
  return 0;
}
