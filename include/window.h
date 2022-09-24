/**
 * setup.h
 * 
 * Created by EtoileScintillante.
 * This header was created so that the window related functions don't have to be implemented in main.cpp.
 */

#ifndef __WINDOW_H__
#define __WINDOW_H__

#include "player.h"

/**
 * @brief Initializes and configures glwf and creates a window.
 * 
 * @param title window title.
 * @param height window height.
 * @param width window width.
 * @return GLFWwindow* pointer to window object.
 */
GLFWwindow* setup(const char* title, int height, int width);

/// glfw: whenever the window size changed (by OS or user resize) this callback function executes.
void framebuffer_size_callback(GLFWwindow *window, int width, int height);

#endif /*__WINDOW__*/