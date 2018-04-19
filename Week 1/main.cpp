#include <iostream>
#include <GL/freeglut.h>

#include "main.h"

const int xRotationTime = 5000;
const int yRotationTime = 5000;
const int zRotationTime = 5000;
const GLfloat cube2Speed = 0.25;

int stageHeight = 600;
int stageWidth = 800;

GLfloat angleX = 0;
GLfloat angleY = 0;
GLfloat angleZ = 0;
GLfloat hueValue = 0;
int polyMode = 0;
int perspectiveMode = 0;

GLfloat cube2X = 0;
GLfloat cube2Y = 0;

void switchPerspectiveMode();
void switchPolyMode();

void applyPerspectiveMode() {
	switch (perspectiveMode) {
	case 0:
		gluPerspective(70, (float)stageWidth / stageHeight, 0.1, 50);
		break;
	case 1:
		float factor = 0.005;
		glOrtho(-stageWidth * factor, stageWidth * factor, -stageHeight * factor, stageHeight * factor, 0.1, 50);
		break;
	}
}

void color3fHue(GLfloat h) {
	while (h > 360)
		h -= 360;
	while (h < 0)
		h += 360;

	if (h < 60)			glColor3f(1, h / 60, 0);
	else if (h < 120)	glColor3f(1 - (h - 60) / 60, 1, 0);
	else if (h < 180)	glColor3f(0, 1, (h - 120) / 60);
	else if (h < 240)	glColor3f(0, 1 - (h - 180) / 60, 1);
	else if (h < 300)	glColor3f((h - 240) / 60, 0, 1);
	else if (h < 360)	glColor3f(1, 0, 1 - (h - 300) / 60);
}

void drawCube(GLfloat width, GLfloat height, GLfloat depth, GLfloat hue) {
	GLfloat hw = width / 2;
	GLfloat hh = height / 2;
	GLfloat hd = depth / 2;
	GLboolean useHue = hue >= 0;

	glBegin(GL_TRIANGLES);

	if (useHue)
		color3fHue(hue);

	// Front
	if (!useHue)
		glColor3f(1, 0, 0);
	glVertex3f(-hw,  hh,  hd);
	glVertex3f(-hw, -hh,  hd);
	glVertex3f( hw, -hh,  hd);
	glVertex3f( hw,  hh,  hd);
	glVertex3f(-hw,  hh,  hd);
	glVertex3f( hw, -hh,  hd);

	// Top
	if (!useHue)
		glColor3f(0, 1, 0);
	glVertex3f(-hw,  hh, -hd);
	glVertex3f(-hw,  hh,  hd);
	glVertex3f( hw,  hh,  hd);
	glVertex3f( hw,  hh, -hd);
	glVertex3f(-hw,  hh, -hd);
	glVertex3f( hw,  hh,  hd);

	// Right
	if (!useHue)
		glColor3f(0, 0, 1);
	glVertex3f( hw,  hh,  hd);
	glVertex3f( hw, -hh,  hd);
	glVertex3f( hw, -hh, -hd);
	glVertex3f( hw,  hh, -hd);
	glVertex3f( hw,  hh,  hd);
	glVertex3f( hw, -hh, -hd);

	// Back
	if (!useHue)
		glColor3f(1, 1, 0);
	glVertex3f( hw,  hh, -hd);
	glVertex3f( hw, -hh, -hd);
	glVertex3f(-hw, -hh, -hd);
	glVertex3f(-hw,  hh, -hd);
	glVertex3f( hw,  hh, -hd);
	glVertex3f(-hw, -hh, -hd);

	// Bottom
	if (!useHue)
		glColor3f(0, 1, 1);
	glVertex3f(-hw, -hh,  hd);
	glVertex3f(-hw, -hh, -hd);
	glVertex3f( hw, -hh, -hd);
	glVertex3f( hw, -hh,  hd);
	glVertex3f(-hw, -hh,  hd);
	glVertex3f( hw, -hh, -hd);

	// Left
	if (!useHue)
		glColor3f(1, 0, 1);
	glVertex3f(-hw,  hh, -hd);
	glVertex3f(-hw, -hh, -hd);
	glVertex3f(-hw, -hh,  hd);
	glVertex3f(-hw,  hh,  hd);
	glVertex3f(-hw,  hh, -hd);
	glVertex3f(-hw, -hh,  hd);

	glEnd();
}

int main(int argc, char** argv) {
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(stageWidth, stageHeight);
	glutInit(&argc, argv);

	glutCreateWindow("Week 1");
	glutDisplayFunc(onDraw);
	glutIdleFunc(onIdle);
	glutKeyboardFunc(onKeyPressed);
	glutPassiveMotionFunc(onMouseMove);
	glutReshapeFunc(onResize);
	glutSpecialFunc(onKeyCodePressed);

	glEnable(GL_DEPTH_TEST);

	glutMainLoop();
}

void onDraw() {
	int elapsedMs = glutGet(GLUT_ELAPSED_TIME);

	angleX = (GLfloat)(elapsedMs % xRotationTime) / xRotationTime * 360;
	angleY = (GLfloat)(elapsedMs % yRotationTime) / yRotationTime * 360;
	angleZ = (GLfloat)(elapsedMs % zRotationTime) / zRotationTime * 360;

	glClearColor(0.5, 0, 1, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	applyPerspectiveMode();

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(
		0, 2.5, 5,	// Eyes
		0, 0, 0,	// Center
		0, 1, 0		// Up
	);
	glPushMatrix();

	// Cube 1: x-axis
	glTranslatef(-3, 0, 0);
	glRotatef(angleX, 1, 0, 0);
	drawCube(1, 1, 1, -1);
	
	glPopMatrix();
	glPushMatrix();

	// Cube 2: y-axis (keyboard)
	glTranslatef(cube2X, cube2Y, 0);
	glRotatef(angleY, 0, 1, 0);
	drawCube(1, 1, 1, -1);

	glPopMatrix();
	glPushMatrix();

	// Cube 3: z-axis
	glTranslatef(3, 0, 0);
	glRotatef(angleZ, 0, 0, 1);
	drawCube(1, 1, 1, -1);

	glPopMatrix();
	glPushMatrix();

	// Cube 4: background hue disco (mouse)
	glTranslatef(0, 0, -3);
	glRotatef(angleX, 1, 0, 0);
	glRotatef(angleY, 0, 1, 0);
	drawCube(1, 1, 1, hueValue);

	glPopMatrix();

	glutSwapBuffers();
}

void onIdle() {
	glutPostRedisplay();
}

void onKeyPressed(unsigned char key, int x, int y) {
	std::cout << "Key: " << key << std::endl;
	switch (key) {
		case 27:
			glutLeaveMainLoop();
			break;
		case ' ':
			std::cout << "Changed perspective" << std::endl;
			switchPerspectiveMode();
			break;
		case 'r':
		case 'R':
			std::cout << "Changed draw method" << std::endl;
			switchPolyMode();
			break;
	}
}

void onKeyCodePressed(int keyCode, int x, int y) {
	switch (keyCode) {
		case GLUT_KEY_UP:
			cube2Y += cube2Speed;
			break;
		case GLUT_KEY_DOWN:
			cube2Y -= cube2Speed;
			break;
		case GLUT_KEY_LEFT:
			cube2X -= cube2Speed;
			break;
		case GLUT_KEY_RIGHT:
			cube2X += cube2Speed;
			break;
	}
}

void onMouseMove(int x, int y) {
	hueValue = (float)x / stageWidth * 360;
}

void onResize(int w, int h) {
	stageWidth = w;
	stageHeight = h;

	glViewport(0, 0, stageWidth, stageHeight);
}

void switchPerspectiveMode() {
	perspectiveMode = (perspectiveMode + 1) % 2;
}

void switchPolyMode() {
	polyMode = (polyMode + 1) % 3;
	switch (polyMode) {
	case 0:
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		break;
	case 1:
		glPolygonMode(GL_FRONT, GL_LINE);
		glPolygonMode(GL_BACK, GL_POINT);
		break;
	case 2:
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		break;
	}
}
