/**
 * texture_loading.h
 * 
 * Created by: EtoileScintillante
 * This file was created to avoid copying the same functions for loading a texture
 * in multiple files that need them. All functions are originally from https://www.learnopengl.com.
 */

#ifndef __TEXTURE_LOADING__
#define __TEXTURE_LOADING__

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>

#include <iostream>
#include <vector>
#include <string>

/**
 * @brief loads a cubemap texture from 6 individual texture faces in the following order:
 * +X (right)
 * -X (left)
 * +Y (top)
 * -Y (bottom)
 * +Z (front)
 * -Z (back)
 * 
 * @param faces paths to the 6 sides (faces) of the cubemap. Paths must be ordered as stated in function description.
 * @return unsigned int (texture ID)
 */
unsigned int loadCubemap(std::vector<std::string> faces);

/**
 * @brief loads a texture.
 * 
 * @param path path to texture.
 * @param ID texture ID.
 * @param flipVertically flip texture vertically on load or not?
 */
void loadTexture(std::string path, unsigned int ID, bool flipVertically);

/**
 * @brief loads a texture from a file (used in model.h).
 * 
 * @param path filename
 * @param directory directory where filename is located.
 * @param gamma gamma correction? default is false.
 * @return unsigned int texture ID.
 */
unsigned int TextureFromFile(const char *path, const std::string &directory, bool gamma = false);

#endif /*__TEXTURE_LOADING__*/