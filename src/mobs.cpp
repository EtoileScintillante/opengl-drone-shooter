#include "mobs.h"

Mobs::Mobs(std::vector < Data > verticesZombie, std::vector < Data > verticesCreeper, std::vector < glm::vec3 > treePositions, std::string texturePath, unsigned int terrainSize, float groundY, float blockSize, float minHeight, float maxHeight)
{
    this->verticesZombie = verticesZombie;
    this->verticesZombie = verticesCreeper;
    this->treePositions = treePositions;
    this->texturePath = texturePath;
    this->terrainSize = terrainSize;
    this->groundY = groundY;
    this->blockSize = blockSize;
    this->minHeight = minHeight;
    this->maxHeight = maxHeight;

    configureMobs();
}