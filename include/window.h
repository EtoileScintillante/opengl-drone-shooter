/**
 * setup.h
 * 
 * Created by EtoileScintillante.
 * This header was created so that the window related functions don't have to be implemented in main.cpp.
 * Based on code made by Joey de Vries (from learnopengl).
 */

#ifndef __WINDOW_H__
#define __WINDOW_H__

#include "player.h"

/// @brief Initializes and configures glwf and creates a window.
/// @return GLFW window object pointer
GLFWwindow* setup();

/// @brief processes all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly.
/// @param window pointer to glfw window object.
/// @param[out] player player object.
/// @param deltaTime time passed between two frames.
void processInput(GLFWwindow *window, Player &player, float deltaTime);

/// glfw: whenever the window size changed (by OS or user resize) this callback function executes.
void framebuffer_size_callback(GLFWwindow *window, int width, int height);

#endif /*__WINDOW__*/