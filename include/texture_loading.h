/**
 * texture_loading.h
 * 
 * This file contains functions to load textures.
 * All functions are originally from https://www.learnopengl.com.
 * loadTexture and TextureFromFile have been modified.
 * 
 * Created by EtoileScintillante.
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
 * @brief Loads a cubemap texture from 6 individual texture faces in the following order:
 * +X (right)
 * -X (left)
 * +Y (top)
 * -Y (bottom)
 * +Z (front)
 * -Z (back)
 * 
 * @param faces paths to the 6 sides (faces) of the cubemap. Paths must be ordered as stated in function description.
 * @param ID unsigned int texture ID.
 */
void loadCubemap(std::vector<std::string> faces, unsigned int ID);

/**
 * @brief Loads a texture.
 * 
 * @param path path to texture.
 * @param ID texture ID.
 * @param flipVertically flip texture vertically on load or not?
 */
void loadTexture(std::string path, unsigned int ID, bool flipVertically);

/**
 * @brief Loads a texture (used in model.h).
 * 
 * @param filename filename.
 * @param directory directory where filename is located.
 * @param flipVertically flip texture vertically on load or not?
 * @param gamma gamma correction? Default is false.
 * @return unsigned int texture ID.
 */
unsigned int TextureFromFile(const char *filename, const std::string &directory, bool flipVertically, bool gamma = false);

#endif /*__TEXTURE_LOADING__*/