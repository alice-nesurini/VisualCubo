
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <GL/freeglut.h>

#include <iostream>

float angle = 0.0f;
float distance = -45.0f;
int windowId;
bool toggleRun;
float colors[18];

void animation(int value) {
	angle += 0.01;
	glutPostWindowRedisplay(windowId);
}

void idleCallback() {
	glutTimerFunc(100, animation, 0);
}

void keyboardCallback(unsigned char key,int x, int y) {
	if (key==' ') {
		//stop idle
		toggleRun = !toggleRun;
		glutIdleFunc(toggleRun?idleCallback:NULL);
	}
	if (key == 'r') {
		//random colors
		for (int i = 0; i < sizeof(colors) / sizeof(float); i++) {
			colors[i] = ((float)rand() / (RAND_MAX));
		}
	}
}

void displayCallback(){
	glClearColor(1.0f, 0.6f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glClear(GL_DEPTH_BUFFER_BIT);

	//move matrix
	glm::mat4 translation = glm::translate(glm::mat4(), glm::vec3(0.0f, 0.0f, distance));
	glm::mat4 rotationXY = glm::rotate(glm::mat4(), glm::radians(angle), glm::vec3(1.0f, 1.0f, 0.0f));
	//model matrix to load
	glm::mat4 f = translation * rotationXY;
	//load to opengl
	glLoadMatrixf(glm::value_ptr(f));

	//THE CUBE 
	glBegin(GL_TRIANGLE_STRIP);
	//glColor3f(0.0f, 1.0f, 1.0f);
	glColor3f(colors[0], colors[1], colors[2]);
	glVertex3f(-10.0f, -10.0f, 10.0f);
	glVertex3f(-10.0f, 10.0f, 10.0f);
	glVertex3f(10.0f, -10.0f, 10.0f);
	glVertex3f(10.0f, 10.0f, 10.0f);
	glEnd();

	glBegin(GL_TRIANGLE_STRIP);
	//glColor3f(0.5f, 0.0f, 1.0f);
	glColor3f(colors[3], colors[4], colors[5]);
	glVertex3f(-10.0f, -10.0f, -10.0f);
	glVertex3f(-10.0f, 10.0f, -10.0f);
	glVertex3f(10.0f, -10.0f, -10.0f);
	glVertex3f(10.0f, 10.0f, -10.0f);
	glEnd();
	
	glBegin(GL_TRIANGLE_STRIP);
	glColor3f(0.5f, 0.5f, 0.2f);
	glColor3f(colors[6], colors[7], colors[8]);
	glVertex3f(-10.0f, 10.0f, -10.0f);
	glVertex3f(-10.0f, -10.0f, -10.0f);
	glVertex3f(-10.0f, 10.0f, 10.0f);
	glVertex3f(-10.0f, -10.0f, 10.0f);
	glEnd();

	glBegin(GL_TRIANGLE_STRIP);
	//glColor3f(0.1f, 0.7f, 0.2f);
	glColor3f(colors[9], colors[10], colors[11]);
	glVertex3f(10.0f, 10.0f, -10.0f);
	glVertex3f(10.0f, -10.0f, -10.0f);
	glVertex3f(10.0f, 10.0f, 10.0f);
	glVertex3f(10.0f, -10.0f, 10.0f);
	glEnd();

	glBegin(GL_TRIANGLE_STRIP);
	//glColor3f(1.0f, 0.0f, 0.0f);
	glColor3f(colors[12], colors[13], colors[14]);
	glVertex3f(-10.0f, 10.0f, -10.0f);
	glVertex3f(-10.0f, 10.0f, 10.0f);
	glVertex3f(10.0f, 10.0f, -10.0f);
	glVertex3f(10.0f, 10.0f, 10.0f);
	glEnd();

	glBegin(GL_TRIANGLE_STRIP);
	//glColor3f(0.0f, 0.0f, 1.0f);
	glColor3f(colors[15], colors[16], colors[17]);
	glVertex3f(-10.0f, -10.0f, -10.0f);
	glVertex3f(-10.0f, -10.0f, 10.0f);
	glVertex3f(10.0f, -10.0f, -10.0f);
	glVertex3f(10.0f, -10.0f, 10.0f);
	glEnd();

	//Swap this context's buffer:
	glutSwapBuffers();
}


void reshapeCallback(int width, int height){
	std::cout << "[reshape func invoked]" << std::endl;
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)width / (float)height, 1.0f, 100.0f);
	glLoadMatrixf(glm::value_ptr(projection));
	glMatrixMode(GL_MODELVIEW);
}

void specialCallback(int key, int mouseX, int mouseY){
	std::cout << "[key pressed]" << std::endl;
	const float speed = 0.5f;
	switch (key){
	case GLUT_KEY_UP:
		distance -= speed;
		break;

	case GLUT_KEY_DOWN:
		distance += speed;
		break;

	case GLUT_KEY_LEFT:
		angle += speed;
		break;

	case GLUT_KEY_RIGHT:
		angle -= speed;
		break;
	}

	glutPostWindowRedisplay(windowId);
}


int main(int argc, char *argv[]){

	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowPosition(100, 100);
	glutInit(&argc, argv);
	//Flags
	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);
	windowId = glutCreateWindow("Cubo");

	//z buffer
	glEnable(GL_DEPTH_TEST);
	// The OpenGL context is now initialized...
	// Set callback functions:
	glutDisplayFunc(displayCallback);
	glutReshapeFunc(reshapeCallback);
	glutSpecialFunc(specialCallback);
	glutIdleFunc(idleCallback);
	glutKeyboardFunc(keyboardCallback);

	//color ready
	for(int i = 0; i<sizeof(colors)/sizeof(float); i++){
		colors[i] = ((float)rand() / (RAND_MAX));
	}
	glutMainLoop();

	std::cout << "[EXIT]" << std::endl;
	return 0;
}