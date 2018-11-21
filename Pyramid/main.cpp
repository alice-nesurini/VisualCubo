#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GL/freeglut.h>
#include <iostream>

float angle = 0.0f;
float distance = -45.0f;
float move_distance = 0.0f;
int windowId;
bool toggleRun;
float colors[18];
// cube size
int size = 5.0f;

void animation(int value) {
	angle += 0.01;
	glutPostWindowRedisplay(windowId);
}

void idleCallback() {
	glutTimerFunc(100, animation, 0);
}

void drawCube() {
	//THE CUBE
	glBegin(GL_TRIANGLE_STRIP);
	glColor3f(colors[0], colors[1], colors[2]);
	glVertex3f(-size, -size, size);
	glVertex3f(-size, size, size);
	glVertex3f(size, -size, size);
	glVertex3f(size, size, size);
	glEnd();

	glBegin(GL_TRIANGLE_STRIP);
	glColor3f(colors[3], colors[4], colors[5]);
	glVertex3f(-size, -size, -size);
	glVertex3f(-size, size, -size);
	glVertex3f(size, -size, -size);
	glVertex3f(size, size, -size);
	glEnd();

	glBegin(GL_TRIANGLE_STRIP);
	glColor3f(0.5f, 0.5f, 0.2f);
	glColor3f(colors[6], colors[7], colors[8]);
	glVertex3f(-size, size, -size);
	glVertex3f(-size, -size, -size);
	glVertex3f(-size, size, size);
	glVertex3f(-size, -size, size);
	glEnd();

	glBegin(GL_TRIANGLE_STRIP);
	glColor3f(colors[9], colors[10], colors[11]);
	glVertex3f(size, size, -size);
	glVertex3f(size, -size, -size);
	glVertex3f(size, size, size);
	glVertex3f(size, -size, size);
	glEnd();

	glBegin(GL_TRIANGLE_STRIP);
	glColor3f(colors[12], colors[13], colors[14]);
	glVertex3f(-size, size, -size);
	glVertex3f(-size, size, size);
	glVertex3f(size, size, -size);
	glVertex3f(size, size, size);
	glEnd();

	glBegin(GL_TRIANGLE_STRIP);
	glColor3f(colors[15], colors[16], colors[17]);
	glVertex3f(-size, -size, -size);
	glVertex3f(-size, -size, size);
	glVertex3f(size, -size, -size);
	glVertex3f(size, -size, size);
	glEnd();
}

void keyboardCallback(unsigned char key, int x, int y) {
	if (key == ' ') {
		//stop idle
		toggleRun = !toggleRun;
		glutIdleFunc(toggleRun ? idleCallback : NULL);
	}
	if (key == 'r') {
		//random colors
		for (int i = 0; i < sizeof(colors) / sizeof(float); i++) {
			colors[i] = ((float)rand() / (RAND_MAX));
		}
	}
}

void displayCallback() {
	glClearColor(0.9f, 0.8f, 0.2f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glClear(GL_DEPTH_BUFFER_BIT);

	//movement matrix
	glm::mat4 translation = glm::translate(glm::mat4(), glm::vec3(0.0f, 0.0f, distance));
	glm::mat4 moveH = glm::translate(glm::mat4(), glm::vec3(move_distance, 0.0f, 0.0f));
	glm::mat4 moveY = glm::translate(glm::mat4(), glm::vec3(0.0f, 0.0f, 0.0f));
	glm::mat4 rotationY = glm::rotate(glm::mat4(), glm::radians(angle), glm::vec3(0.0f, 1.0f, 0.0f));
	//model matrix to load
	glm::mat4 f = moveH * moveY * translation * rotationY;
	//load to opengl
	glLoadMatrixf(glm::value_ptr(f));

	// draw first cube (base)
	drawCube();
	// remodel cube by scaling & reload matrix (middle)
	glm::mat4 scaling = glm::scale(glm::mat4(), glm::vec3(0.50f, 0.50f, 0.50f));
	glm::mat4 yAxis = glm::translate(glm::mat4(), glm::vec3(0.0f, size+size*0.5, 0.0f));
	f = moveH * translation * yAxis * scaling * rotationY;
	glLoadMatrixf(glm::value_ptr(f));
	drawCube();
	// remodel (top)
	scaling = glm::scale(glm::mat4(), glm::vec3(0.250f, 0.250f, 0.250f));
	yAxis = glm::translate(glm::mat4(), glm::vec3(0.0f, size*2+size*0.250f, 0.0f));
	f = moveH * translation * yAxis * scaling * rotationY;
	glLoadMatrixf(glm::value_ptr(f));
	drawCube();

	glutSwapBuffers();
}


void reshapeCallback(int width, int height) {
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)width / (float)height, 1.0f, 100.0f);
	glLoadMatrixf(glm::value_ptr(projection));
	glMatrixMode(GL_MODELVIEW);
}

void specialCallback(int key, int mouseX, int mouseY) {
	const float speed = 0.5f;
	switch (key) {
	case GLUT_KEY_UP:
		distance -= speed;
		break;

	case GLUT_KEY_DOWN:
		distance += speed;
		break;

	case GLUT_KEY_LEFT:
		move_distance -= speed;
		break;

	case GLUT_KEY_RIGHT:
		move_distance += speed;
		break;
	}

	glutPostWindowRedisplay(windowId);
}


int main(int argc, char *argv[]) {
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(500,500);
	glutInit(&argc, argv);
	//Flags
	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);
	windowId = glutCreateWindow("Cubo");
	//opengl context ready

	//z buffer
	glEnable(GL_DEPTH_TEST);

	// per vedere se è giusto :)
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	//set callback functions:
	glutDisplayFunc(displayCallback);
	glutReshapeFunc(reshapeCallback);
	glutSpecialFunc(specialCallback);
	glutIdleFunc(idleCallback);
	glutKeyboardFunc(keyboardCallback);

	//color ready
	for (int i = 0; i < sizeof(colors) / sizeof(float); i++) {
		colors[i] = ((float)rand() / (RAND_MAX));
	}
	glutMainLoop();
	return 0;
}