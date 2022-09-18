/*                                  
 * tree.h
 *
 * Created by: EtoileScintillante
 */

#ifndef __TREE_H__
#define __TREE_H__

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
#include "mesh.h"

class Tree {
public:
    std::vector < Vertex > verticesTrunk;
    std::vector < Vertex > verticesLeaves;
    std::vector < glm::vec3 > treePositions; 
    std::string pathTrunkTexture;
    std::string pathLeavesTexture;
    unsigned int numTrees;
    unsigned int heightTree;
    unsigned int terrainSize;
    float groundLevel;
    float blockSize;

    /// default constructor
    Tree();

    /**
     * @brief Construct a new Tree object. Also configures the tree automatically.
     * 
     * @param verticesTrunk vertex data for trunk.
     * @param verticesLeaves vertex data for leaves.
     * @param pathTrunkTexture path to trunk block texture.
     * @param pathLeavesTexture path to leaves block texture.
     * @param numTrees number of trees (could also be just one).
     * @param heightTree height of the tree.
     * @param terrainSize terrain size. Note: the actual size of the terrain is calculated by terrainSize * terrainSize
     * (the terrain is shaped like a square), the input variable here should correspond to the amount of blocks that make up one side of the square).
     * @param groundLevel y value of the ground's position
     * @param blockSize block size
     */
    Tree(std::vector < Vertex > verticesTrunk, std::vector < Vertex > verticesLeaves, std::string pathTrunkTexture, std::string pathLeavesTexture, unsigned int numTrees, unsigned int heightTree, unsigned int terrainSize, float groundLevel, float blockSize);

    /**
     * @brief draw the tree(s). The shaders do not have to be different.
     * 
     * @param shaderTrunk shader for trunk block.
     * @param shaderLeaves shader for leaves block. 
     * @param cameraView camera view matrix.
     * @param projection projection matrix.
     */
    void Draw(Shader &shaderTrunk, Shader &shaderLeaves, glm::mat4 cameraView, glm::mat4 projection);

private:
    std::vector < glm::vec3 > leavesPositions;
    unsigned int textureTrunk = 1;
    unsigned int textureLeaves = 2;
    unsigned int VAOtrunk, VAOleaves;
    unsigned int VBOtrunk, VBOleaves;

    /// sets up and configures buffers/arrays, also creates tree positions (if treePositions is empty) and leaves positions
    void configureTree();

    /// creates numTrees random positions
    void createTreePositions();

    /// creates positions for leaves blocks for every tree (5 leaves blocks per tree)
    void createLeavesPositions();
};

#endif /*__TREE__*/