/**
 * glfw_setup.h
 * 
 * This file was created so that the glfw setup and window related functions don't have to be in main.cpp.
 * 
 * Created by EtoileScintillante.
 */

#ifndef __GLFW_SETUP_H__
#define __GLFW_SETUP_H__

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

/**
 * @brief Initializes and configures glwf and creates a window. 
 * Also loads OpenGL function pointers. If window creation fails
 * or the function pointers can't be loaded, a message indicating the 
 * problem will be printed to the console and glfwTerminate() will be called.
 * 
 * @param title window title.
 * @param height window height.
 * @param width window width.
 * @return GLFWwindow* pointer to window object.
 */
GLFWwindow* setup(const char* title, int height, int width);

/// glfw: whenever the window size changed (by OS or user resize) this callback function executes.
void framebuffer_size_callback(GLFWwindow *window, int width, int height);

#endif /*__GLFW_SETUP__*/