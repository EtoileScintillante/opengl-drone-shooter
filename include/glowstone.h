/*                                  
 * glowstone.h
 *
 * Created by: EtoileScintillante
 */

#ifndef __GLOWSTONE_H__
#define __GLOWSTONE_H__

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

class GlowStone {
public:
    std::vector < Data > verticesGlowStone;
    std::string texturePathGlowStone;
    std::vector < glm::vec3 > treePositions;
    std::vector < glm::vec3 > glowStonePositions;
    unsigned int numGlowStones;
    float height;

    /// default constructor
    GlowStone();

   /**
    * @brief Construct a new GlowStone object. Also configures the glow stone automatically.
    * 
    * @param verticesGlowStone vertex data for glow stone.
    * @param texturePathGlowStone path to dirt texture.
    * @param treePositions positions of tree(s). Needed for drawing the glow stone(s) on the side of a tree trunk.
    * @param numGlowStones number of glow stones.
    * @param height floating height of glow stone(s). Measured from the the bottom trunk block of a tree.
    */
    GlowStone(std::vector < Data > verticesGlowStone, std::string texturePathGlowStone, std::vector < glm::vec3 > treePositions, unsigned int numGlowStones, float height);

    /**
     * @brief draw the glow stone(s). 
     * 
     * @param shader shader for glow stone.
     * @param cameraView camera view matrix.
     * @param projection projection matrix.
     */
    void Draw(Shader &shader, glm::mat4 cameraView, glm::mat4 projection);

private:
    unsigned int textureGlowStone = 6;
    unsigned int VAO;
    unsigned int VBO;

    /// sets up and configures buffers/arrays.
    void configureGlowStone();

    /// creates a vector containing all the positions. For the purpose of this program, the glow stones are positioned on the side of a tree trunk.
    void createPositions();
};

#endif /*__GLOWSTONE__*/