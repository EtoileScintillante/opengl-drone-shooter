/**
 * mobs.h
 * 
 * Created by EtoileScintillante.
 * LoadTexture function from: https://learnopengl.com and modified by me.
 */

#ifndef MOBS.H
#define MOBS.H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include <random>
#include <vector>

#include "shader.h"
#include "data.h"

class Mobs {
public:
    std::vector < Data > verticesZombie;
    std::vector < Data > verticesCreeper;
    std::vector < glm::vec3 > treePositions;
    std::string texturePath;
    unsigned int terrainSize;
    float blockSize;
    bool isShot;

    /**
     * @brief Construct a new Mobs object. Also configures it automatically.
     * 
     * @param verticesZombie vertex data for zombie block.
     * @param verticesCreeper vertex data for creeper block.
     * @param treePositions tree positions (to make sure that a mob does not spawn inside a tree).
     * @param texturePath path to texture
     * @param terrainSize terrain size. Note: the actual size of the terrain is calculated by terrainSize * terrainSize
     * (the terrain is shaped like a square), the input variable here should correspond to the amount of blocks that make up one side of the square).
     * @param blockSize size of mob block
     * @param isShot has a mob been hit by player?
     */
    Mobs(std::vector < Data > verticesZombie, std::vector < Data > verticesCreeper, std::vector < glm::vec3 > treePositions, std::string texturePath, unsigned int terrainSize, float blockSize, bool isShot);

    /**
     * @brief draws a mob.
     * 
     * @param shader shader for mob blocks.
     */
    void Spawn(Shader &shader);

    void collisionDetection();

private:
    unsigned int texture = 6;
    unsigned int VAOcreeper, VAOzombie;
    unsigned int VBOcreeper, VBOzombie;
    glm::vec3 currentPosition;
    std::string currentMob; // has to be chosen in configrureMobs and changed in collisionDetection if mob had been shot

    /// sets up and configures buffers/arrays
    void configureMobs();

    /// @brief  generates a random spawn position.
    /// @return vector containing random positions.
    std::vector < glm::vec3 > randomPos();
};

#endif /*__MOBS__*/

