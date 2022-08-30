#include "terrain.h"

std::vector< glm::vec3 > generateTreePositions(int numTrees, int terrainSize, float groundY, float blockSize, float offsetY)
{
    std::random_device rd; // obtain a random number from hardware
    std::mt19937 gen(rd()); // seed the generator

    // create random positions 
    std::vector < glm::vec3 > positions;
    std::uniform_int_distribution<> xzPlane(-terrainSize + 1, terrainSize - 1); // define the range for x and z axis 
    for (unsigned int i = 0; i < numTrees; i++)
    {
        float posX = xzPlane(gen); 
        float posZ = xzPlane(gen);
        glm::vec3 vec = {posX, groundY + blockSize + offsetY, posZ}; 
        positions.push_back(vec);
    }

    return positions;
}

std::vector< glm::vec3 > generateRandomLampPositions(int numLamps, float heightLowerRange, float heightUpperRange, int terrainSize)
{
    std::random_device rd; // obtain a random number from hardware
    std::mt19937 gen(rd()); // seed the generator

    // create random positions 
    std::vector < glm::vec3 > positions;
    std::uniform_int_distribution<> xzPlane(-terrainSize + 1, terrainSize - 1); // define the range for x and z axis 
    std::uniform_int_distribution<> yPlane(heightLowerRange, heightUpperRange); // define the range for y axis 
    for (unsigned int i = 0; i < numLamps; i++)
    {
        float posX = xzPlane(gen); 
        float posZ = xzPlane(gen);
        float posY = yPlane(gen);
        glm::vec3 vec = {posX, posY, posZ}; 
        positions.push_back(vec);
    }

    return positions;
    
}

std::vector< glm::vec3 > generateLampPosStickToTree(int numLamps, int heightTree, float groundY, float blockSize, float offsetY, float offsetX, std::vector< glm::vec3 > treePositions)
{
    std::random_device rd; // obtain a random number from hardware
    std::mt19937 gen(rd()); // seed the generator

    std::uniform_int_distribution<> lights(0, treePositions.size()); // generate random number between 0 and number of trees correspondng to the nth tree in treePositions
    std::vector < int > indexes; // the trees with their position index in this array will be getting a lamp attached to them
    for (unsigned int i = 0; i < numLamps; i++)
    {
        int index = lights(gen); 
        indexes.push_back(index);
    }
    std::vector < glm::vec3 > lampPositions;
    for (int i = 0; i < numLamps; i++)
    {
        // to position the lamp to make it look like it sticks to the side of the tree, we lower or increase x by offsetX and subtract offsetY from heightTree
        glm::vec3 vec = {treePositions[i].x + offsetX, groundY + blockSize + heightTree - offsetY, treePositions[i].z};
        if (i % 3 == 0) {vec = {treePositions[i].x - offsetX, groundY + blockSize + heightTree - offsetY, treePositions[i].z};} // to add some variation in the direction the lamps are pointing to
        lampPositions.push_back(vec);
    }

    return lampPositions;
}

std::vector< glm::vec3 > generateLeavesPositions(int heightTree, float blockSize, float groundY, float offset, std::vector< glm::vec3 > treePositions)
{
    std::vector < glm::vec3 > positions;
    for (unsigned int i = 0; i < treePositions.size(); i++)
    {
        glm::vec3 leavesBlock1 = {treePositions[i].x - blockSize - offset, groundY + heightTree, treePositions[i].z}; // negative x side
        glm::vec3 leavesBlock2 = {treePositions[i].x + blockSize + offset, groundY + heightTree, treePositions[i].z}; // positive x side
        glm::vec3 leavesBlock3 = {treePositions[i].x, groundY + heightTree, treePositions[i].z - blockSize - offset}; // negative z side
        glm::vec3 leavesBlock4 = {treePositions[i].x, groundY + heightTree, treePositions[i].z + blockSize + offset}; // positive z side
        glm::vec3 leavesBlock5 = {treePositions[i].x, groundY + heightTree + blockSize + offset, treePositions[i].z}; // top
        
        positions.push_back(leavesBlock1);
        positions.push_back(leavesBlock2);
        positions.push_back(leavesBlock3);
        positions.push_back(leavesBlock4);
        positions.push_back(leavesBlock5);
    }

    return positions;
}