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
#include "texture_loading.h"
#include "box.h"
#include "model.h"

class Mobs {
public:
    std::vector < Vertex > verticesZombie;
    std::vector < Vertex > verticesCreeper;
    std::vector < glm::vec3 > treePositions;
    std::string texturePath;
    unsigned int terrainSize;
    float groundY;
    float blockSize;
    float minHeight;
    float maxHeight;
    float deathTime; 
    bool hasDied;

    /// default constructor
    Mobs();

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
     * @param minHeight minimum y value for floating height of mob (measured from the ground).
     * @param maxheight maximum y value for floating height of mob (measured from the ground).
     */
    Mobs(std::vector < Vertex > verticesZombie, std::vector < Vertex > verticesCreeper, std::vector < glm::vec3 > treePositions, std::string texturePath, unsigned int terrainSize, float groundY, float blockSize, float minHeight, float maxHeight);

    /**
     * @brief draws a mob.
     * 
     * @param shader shader for mob blocks.
     * @param time current time/frame, to let mob rotate over time (y - axis rotation).
     * @param cameraView camera view matrix.
     * @param projection projection matrix.
     */
    void Spawn(Shader &shader, float time, glm::mat4 cameraView, glm::mat4 projection);

    /**
     * @brief detects whether the mob has been hit by a bullet (using ray-box intersection).
     * Info from https://www.scratchapixel.com/lessons/3d-basic-rendering/minimal-ray-tracer-rendering-simple-shapes/ray-box-intersection.
     * 
     * @param bulletStartPos starting position of the bullet (in this program that is the camera position).
     * @param bulletDir direction of the bullet (in this program that is the front vector of the camera).
     * @param bulletRange range of bullet (maximum distance it can travel).
     */
    void collisionDetection(glm::vec3 bulletStartPos, glm::vec3 bulletDir, float bulletRange);

    /**
     * @brief renders a 3D skull at the position of the mob that just got killed.
     * 
     * @param skullModel 3D skull model.
     * @param skullShader shader of skull model.
     * @param cameraView camera view matrix.
     * @param projection projection matrix.
     * @param deltaTime time between 2 frames. Needed to control the time between a mob dying and another one spawning.
     * @param time current time/frame, to let skull rotate over time (y - axis rotation).
     */
    void died(Model &skullModel, Shader &skullShader, glm::mat4 cameraView, glm::mat4 projection, float deltaTime, float time);

    /// returns the current position of the mob.
    glm::vec3 getCurrentPos() const;

private:
    unsigned int texture = 5;
    unsigned int creeperVAO, creeperVBO;
    unsigned int zombieVAO, zombieVBO;
    glm::vec3 currentPosition; // has to be chosen in configrureMobs and changed in dyingAnimation if mob has been shot
    unsigned int currentMob; // has to be chosen in configrureMobs and changed in dyingAnimation if mob has been shot

    /// sets up and configures buffers/arrays
    void configureMobs();

    /// generates a random position
    void getRandomPos();

    /// chooses a mob (zombie or creeper).
    void chooseMob();

    /**
     * @brief checks whether getRandomPos created a position that overlaps with a position of a tree.
     * 
     * @param position position of tree.
     * @return true if overlap.
     * @return false if no overlap.
     */
    bool spawnsInTree(glm::vec3 position);
};

#endif /*__MOBS__*/

