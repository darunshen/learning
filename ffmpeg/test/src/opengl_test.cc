#ifndef GLUT_DISABLE_ATEXIT_HACK
#define GLUT_DISABLE_ATEXIT_HACK
#endif #define GLEW_STATIC
#include <GL/freeglut.h>
#include <vector>
// undefine if u want to use the default bending constraint of pbd
#include <iostream>
using namespace std;

GLfloat rtx = 0.0f, rty = 0.0f, rtz = 0.0f;
GLfloat step = 0.005;
// float exp1 = 1e-3;

void init() {
  glLoadIdentity();
  glClearColor(0.0, 0.0, 0.0, 0.0);
}

void draw(void) {
  glClear(GL_COLOR_BUFFER_BIT);

  glPushMatrix();
  printf("%f %f %f\n", rtx, rty, rtz);
  glTranslatef(rtx, rty, rtz);

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

/* special key defined in glut
#define GLUT_KEY_F1			1
#define GLUT_KEY_F2			2
#define GLUT_KEY_F3			3
#define GLUT_KEY_F4			4
#define GLUT_KEY_F5			5
#define GLUT_KEY_F6			6
#define GLUT_KEY_F7			7
#define GLUT_KEY_F8			8
#define GLUT_KEY_F9			9
#define GLUT_KEY_F10			10
#define GLUT_KEY_F11			11
#define GLUT_KEY_F12			12
#define GLUT_KEY_LEFT			100
#define GLUT_KEY_UP			101
#define GLUT_KEY_RIGHT			102
#define GLUT_KEY_DOWN			103
#define GLUT_KEY_PAGE_UP		104
#define GLUT_KEY_PAGE_DOWN		105
#define GLUT_KEY_HOME			106
#define GLUT_KEY_END			107
#define GLUT_KEY_INSERT			108
*/
void processKeyBoard1(int key, int x, int y) {
  switch (key) {
    case GLUT_KEY_LEFT:
      if (rtx - step > -0.53) rtx -= step;
      break;
    case GLUT_KEY_RIGHT:
      if (rtx + step < 0.53) rtx += step;
      break;
    case GLUT_KEY_UP:
      if (rty + step < 0.53) rty += step;
      break;
    case GLUT_KEY_DOWN:
      if (rty - step > -0.53) rty -= step;
      break;
    default:
      break;
  }

  glutPostRedisplay();
  return;
}

// float step = 0.001;
void move() {
  if (rtx + step > 0.5 || rtx + step < -0.5) step = -step;
  rtx += step;
  glutPostRedisplay();
}

int main(int argc, char *argv[]) {
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
  glutInitWindowPosition(100, 100);
  glutInitWindowSize(600, 600);
  glutCreateWindow("test");

  init();

  glutDisplayFunc(draw);
  //	glutIdleFunc(move);               //自动运动
  glutSpecialFunc(processKeyBoard1);  //响应键盘
  glutMainLoop();
  return 0;
}