/**
 * @file		main.cpp
 * @brief	Simple FreeGLUT usage example
 *
 * @author	Achille Peternier (C) SUPSI [achille.peternier@supsi.ch]
 */



//////////////
// #INCLUDE //
//////////////

   // GLM:
   #define GLM_FORCE_RADIANS
   #include <glm/glm.hpp>
   #include <glm/gtc/matrix_transform.hpp>
   #include <glm/gtc/type_ptr.hpp>
   
   // FreeGLUT:   
   #include <GL/freeglut.h>

   // C/C++:
   #include <iostream>      
   


///////////////
// CALLBACKS //
///////////////

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * This is the main rendering routine automatically invoked by FreeGLUT.
 */
void displayCallback()
{
   // Clear the screen:         
   glClearColor(1.0f, 0.6f, 0.1f, 1.0f); // RGBA components
   glClear(GL_COLOR_BUFFER_BIT);   
   
   // Render a simple triangle (world coordinates):
   glBegin(GL_TRIANGLES);
      glColor3f(0.0f, 0.0f, 1.0f);
      glVertex3f(-10.0f, -10.0f, -45.0f);
      glVertex3f(10.0f, -10.0f, -45.0f);
      glVertex3f(0.0f, 10.0f, -45.0f);
   glEnd();
   
   // Swap this context's buffer:     
   glutSwapBuffers();   

   // Force another invocation of this rendering callback:
   glutPostWindowRedisplay(glutGetWindow());
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * This callback is invoked each time the window gets resized (and once also when created).
 * @param width new window width
 * @param height new window height
 */
void reshapeCallback(int width, int height)
{
   // For your information...:
   std::cout << "[reshape func invoked]" << std::endl;

   // Change OpenGL viewport to match the new window sizes:
   glViewport(0, 0, width, height);

   // Update the projection matrix:
   glMatrixMode(GL_PROJECTION);
   glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float) width / (float) height, 1.0f, 100.0f);
   glLoadMatrixf(glm::value_ptr(projection));   
}



//////////
// MAIN //
//////////

/**
 * Application entry point.
 * @param argc number of command-line arguments passed 
 * @param argv array containing up to argc passed arguments
 * @return error code (0 on success, error code otherwise)
 */
int main(int argc, char *argv[])
{
   // Credits:
   std::cout << "Simple FreeGLUT example - A. Peternier (C) SUPSI" << std::endl;
   std::cout << std::endl;   

   // Init context:
   glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
   glutInitWindowPosition(100, 100);
   
   // FreeGLUT can parse command-line params, in case:
   glutInit(&argc, argv);   

   // Set some optional flags:
   glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);
   
   // Create the window with a specific title:   
   glutCreateWindow("FreeGLUT simple example");

   // The OpenGL context is now initialized...
   
   // Set callback functions:
   glutDisplayFunc(displayCallback);  
   glutReshapeFunc(reshapeCallback);   
      
   // Check OpenGL version:
   std::cout << "OpenGL context" << std::endl;
   std::cout << "   version  . . : " << glGetString(GL_VERSION) << std::endl;
   std::cout << "   vendor . . . : " << glGetString(GL_VENDOR) << std::endl;
   std::cout << "   renderer . . : " << glGetString(GL_RENDERER) << std::endl;
   std::cout << "   extensions . : " << glGetString(GL_EXTENSIONS) << std::endl;     
   
   // Enter the main FreeGLUT processing loop:     
   glutMainLoop();

   // Done:
   std::cout << "[application terminated]" << std::endl;
   return 0;
}