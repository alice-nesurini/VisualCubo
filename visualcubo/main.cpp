#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GL/freeglut.h>
#include <iostream>

int shininess = 7;
float angle = 0.0f;
float distance = -45.0f;
float move_distance = 0.0f;
int windowId;
bool toggleRun;
float colors[18];
bool showNormal = true;

// material
glm::vec4 ambient(0.1f, 0.1f, 0.5f, 1.0f);
glm::vec4 diffuse(0.5f, 0.5f, 0.5f, 1.0f);
glm::vec4 specular(1.0f, 1.0f, 1.0f, 1.0f);

void animation(int value) {
	angle += 0.01;
	glutPostWindowRedisplay(windowId);
}

void idleCallback() {
	glutTimerFunc(100, animation, 0);
}

void displayTriAndNorm(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3){
	// Compute normal:
	glm::vec3 v1 = p2 - p1;
	glm::vec3 v2 = p3 - p1;
	glm::vec3 n = glm::normalize(glm::cross(v1, v2));

	// Compute triangle baricenter:
	glm::vec3 center = (p1 + p2 + p3) / 3.0f;
	glm::vec3 dest = center + n * 6.0f;

	// Render triangle:
	glBegin(GL_TRIANGLES);
	glNormal3fv(glm::value_ptr(n));
	glVertex3fv(glm::value_ptr(p1));
	glVertex3fv(glm::value_ptr(p2));
	glVertex3fv(glm::value_ptr(p3));
	glEnd();

	// Render normal:
	if(showNormal){
	glBegin(GL_LINES);
	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3fv(glm::value_ptr(center));
	glColor3f(0.2f, 0.2f, 1.0f);
	glVertex3fv(glm::value_ptr(dest));
	glEnd();
	}
}

void drawCube(float edge) {
	//THE CUBE
	float size = edge / 2.0f;
	// back
	displayTriAndNorm(glm::vec3(size, 0, -size), glm::vec3(-size, 0, -size), glm::vec3(size, edge, -size));
	displayTriAndNorm(glm::vec3(-size, 0, -size), glm::vec3(-size, edge, -size), glm::vec3(size, edge, -size));

	// front
	displayTriAndNorm(glm::vec3(-size, 0, size), glm::vec3(size, 0, size), glm::vec3(-size, edge, size));
	displayTriAndNorm(glm::vec3(size, 0, size), glm::vec3(size, edge, size), glm::vec3(-size, edge, size));

	// left
	displayTriAndNorm(glm::vec3(-size, 0, -size), glm::vec3(-size, 0, size), glm::vec3(-size, edge, -size));
	displayTriAndNorm(glm::vec3(-size, edge, -size), glm::vec3(-size, 0, size), glm::vec3(-size, edge, size));

	// right
	displayTriAndNorm(glm::vec3(size, 0, -size), glm::vec3(size, edge,- size), glm::vec3(size, 0, size));
	displayTriAndNorm(glm::vec3(size, edge, -size), glm::vec3(size, edge, size), glm::vec3(size, 0, size));

	// bottom
	displayTriAndNorm(glm::vec3(size, 0, -size), glm::vec3(size, 0, size), glm::vec3(-size, 0, -size));
	displayTriAndNorm(glm::vec3(-size, 0, -size), glm::vec3(size, 0, size), glm::vec3(-size, 0, size));

	// top
	displayTriAndNorm(glm::vec3(size, edge, -size), glm::vec3(-size, edge, -size), glm::vec3(size, edge, size));
	displayTriAndNorm(glm::vec3(-size, edge, -size), glm::vec3(-size, edge, size), glm::vec3(size, edge, size));
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
	if (key == 'n') {
		showNormal = !showNormal;
	}
}

void displayCallback() {
	glClear(GL_COLOR_BUFFER_BIT);
	glClear(GL_DEPTH_BUFFER_BIT);

	// lighting
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, glm::value_ptr(ambient));
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, glm::value_ptr(diffuse));
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, glm::value_ptr(specular));
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, powf(2.0f, (float)shininess));

	glm::vec4 ambientLight(1.0f, 1.0f, 1.0f, 1.0f);
	glm::vec4 diffuseLight(1.0f, 1.0f, 1.0f, 1.0f);
	glm::vec4 specularLight(1.0f, 1.0f, 1.0f, 1.0f);
	glLightfv(GL_LIGHT0, GL_AMBIENT, glm::value_ptr(ambientLight));
	glLightfv(GL_LIGHT0, GL_DIFFUSE, glm::value_ptr(diffuseLight));
	glLightfv(GL_LIGHT0, GL_SPECULAR, glm::value_ptr(specularLight));

	//movement matrix
	glm::mat4 translation = glm::translate(glm::mat4(), glm::vec3(0.0f, 0.0f, distance));
	glm::mat4 moveH = glm::translate(glm::mat4(), glm::vec3(move_distance, 0.0f, 0.0f));
	glm::mat4 rotationXY = glm::rotate(glm::mat4(), glm::radians(angle), glm::vec3(1.0f, 1.0f, 0.0f));
	//model matrix to load
	glm::mat4 f = moveH * translation * rotationXY;
	//load to opengl
	glLoadMatrixf(glm::value_ptr(f));

	drawCube(10);

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
		move_distance-=speed;
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
	glutInit(&argc, argv);
	//Flags
	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);
	windowId = glutCreateWindow("Cubo");
	//opengl context ready

	//z buffer + lighting
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glEnable(GL_CULL_FACE);
	glEnable(GL_NORMALIZE);

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