/*                                  
 * ground.h
 *
 * Created by: EtoileScintillante
 */

#ifndef __GROUND_H__
#define __GROUND_H__

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

class Ground {
public:
    std::vector < Data > verticesDirt;
    std::vector < Data > verticesStone;
    std::string texturePathDirt;
    std::string texturePathStone;
    unsigned int terrainSize;
    float groundLevel;

   /**
    * @brief Construct a new Ground object. Also configures the ground automatically.
    * 
    * @param verticesDirt vertex data for dirt.
    * @param verticesStone vertex data for stone.
    * @param texturePathDirt path to dirt texture.
    * @param texturePathStone path to stone texture.
    * @param terrainSize terrain size. Note: the actual size of the terrain is calculated by terrainSize * terrainSize
     * (the terrain is shaped like a square), the input variable here should correspond to the amount of blocks that make up one side of the square).
    * @param groundLevel y level of the ground.
    */
    Ground(std::vector < Data > verticesDirt, std::vector < Data > verticesStone, std::string texturePathDirt, std::string texturePathStone,  unsigned int terrainSize, float groundLevel);

    /**
     * @brief draw the ground. 
     * 
     * @param shader shader.
     * @param cameraView camera view matrix.
     * @param projection projection matrix.
     */
    void Draw(Shader &shader, glm::mat4 cameraView, glm::mat4 projection);

private:
    std::vector < int > blockOrder;
    unsigned int textureDirt = 3;
    unsigned int textureStone = 4;
    unsigned int VAOdirt, VAOstone;
    unsigned int VBOdirt, VBOstone;

    /// sets up and configures buffers/arrays.
    void configureGround();

    /// creates a vector containing 1s and 2s in random order which can be used as the order in which the dirt and stone blocks are rendered to create some variation
    void orderOfBlocks();
};

#endif /*__GROUND__*/