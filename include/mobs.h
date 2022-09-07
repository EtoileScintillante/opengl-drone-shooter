/**
 * mobs.h
 * 
 * Created by EtoileScintillante.
 */

#ifndef __MOBS_H__
#define __MOBS_H__

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
#include "texture_loading.h"

class Mobs {
public:
    std::vector < Data > verticesZombie;
    std::vector < Data > verticesCreeper;
    std::vector < glm::vec3 > treePositions;
    std::string texturePath;
    unsigned int terrainSize;
    float groundY;
    float blockSize;
    float minHeight;
    float maxHeight;
    float currentTime; // needed to play animation

    /**
     * @brief Construct a new Mobs object. Also configures it automatically.
     * 
     * @param verticesZombie vertex data for zombie block.
     * @param verticesCreeper vertex data for creeper block.
     * @param treePositions tree positions (needed to avoid mobs spawning inside a tree)
     * @param texturePath path to mobs texture.
     * @param terrainSize size of terrain. Note: the actual size of the terrain is calculated by terrainSize * terrainSize
     * (the terrain is shaped like a square), the input variable here should correspond to the amount of blocks that make up one side of the square).
     * @param groundY y value of ground.
     * @param blockSize size of a block.
     * @param minHeight minimum y value for floating height of mob.
     * @param maxheight maximum y value for floating height of mob.
     */
    Mobs(std::vector < Data > verticesZombie, std::vector < Data > verticesCreeper, std::vector < glm::vec3 > treePositions, std::string texturePath, unsigned int terrainSize, float groundY, float blockSize, float minHeight, float maxHeight);

    /**
     * @brief draws a mob.
     * 
     * @param shader shader for mob blocks.
     * @param time current time/frame, to let mob rotate over time (y - axis rotation).
     * @param cameraView camera view matrix.
     * @param projection projection matrix.
     */
    void Spawn(Shader &shader, float time, glm::mat4 cameraView, glm::mat4 projection);

    // TODO
    void collisionDetection();

private:
    unsigned int texture = 6;
    unsigned int VAOcreeper, VAOzombie;
    unsigned int VBOcreeper, VBOzombie;
    glm::vec3 currentPosition; // has to be chosen in configrureMobs and changed in dyingAnimation if mob has been shot
    unsigned int currentMob; // has to be chosen in configrureMobs and changed in dyingAnimation if mob has been shot
    bool isShot;

    /// sets up and configures buffers/arrays
    void configureMobs();

    /// play little animation when mob has been killed.
    void dyingAnimation();

    /// generates a random position
    void getRandomPos();

    /// chooses a mob (zombie or creeper).
    void chooseMob();

    /// @brief checks whether getRandomPos created a position that overlaps with a position of a tree.
    /// @param position position vector.
    /// @return true if there is an overlap, else false.
    bool spawnsInTree(glm::vec3 position);
};

#endif /*__MOBS__*/

