#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include <GL/glew.h> // OpenGL
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

// #include <libguile.h> // Scheme interfac

using namespace glm;


void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}

void* register_functions(void* data)
{
  return NULL;
}

int main()
{
  if (!glfwInit())
    {
      fprintf(stderr, "Failed to initialize GLFW; errno: %s\n", strerror(errno));
      return -1;
    }

  /* glfwWindowHint(GLFW_SAMPLES, 4); // 4x antialiasing
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); 
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
  glfwWindowHint(GLFW_OPENGL_ANY_PROFILE, GLFW_OPENGL_CORE_PROFILE); // We don't want the old OpenGL */

  glfwSetErrorCallback(error_callback); // set error callback

  // Open a window and create its OpenGL context
  GLFWwindow* window = glfwCreateWindow( 1024, 768, "First", glfwGetPrimaryMonitor(), NULL);
  if(window == NULL)
    {
      fprintf( stderr, "Failed to open GLFW window. errno: %s\n", strerror(errno));
      glfwTerminate();
      return -1;
    }

  glfwMakeContextCurrent(window); // Initialize GLEW
  glewExperimental=true; // Needed in core profile
  if (glewInit() != GLEW_OK)
    {
      fprintf(stderr, "Failed to initialize GLEW\n");
      return -1;
    }

  // Ensure we can capture the escape key being pressed below
  glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

  do{
    // Swap buffers
    glfwSwapBuffers(window);
    glfwPollEvents();

  } // Check if the ESC key was pressed or the window was closed
  while(glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window) == 0);

  // test if Guile can be launched as well
  // scm_with_guile(&register_functions, NULL);
  return 0;
}

  
