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

void Mobs::Spawn(Shader &shader, float time, glm::mat4 cameraView, glm::mat4 projection)
{
    // set uniforms
    shader.use();
    shader.setMat4("view", cameraView);
    shader.setMat4("projection", projection); 

    // draw
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    if (currentMob == 1) // 1 = zombie
    {
        glBindVertexArray(VAOzombie);
        glm::mat4 modelZombie = glm::mat4(1.0f);
        modelZombie = glm::translate(modelZombie, currentPosition);
        modelZombie = glm::rotate(modelZombie, time, glm::vec3(0.0f, 1.0f, 0.0f));
        shader.setMat4("model", modelZombie);
        glDrawArrays(GL_TRIANGLES, 0, 36);
    }
    if (currentMob == 2) // 2 = creeper
    {
        glBindVertexArray(VAOcreeper);
        glm::mat4 modelCreeper = glm::mat4(1.0f);
        modelCreeper = glm::translate(modelCreeper, currentPosition);
        modelCreeper = glm::rotate(modelCreeper, time, glm::vec3(0.0f, 1.0f, 0.0f));
        shader.setMat4("model", modelCreeper);
        glDrawArrays(GL_TRIANGLES, 0, 36);
    }
}

void Mobs::configureMobs()
{
    // create buffers/arrays for zombie block first
    glGenVertexArrays(1, &VAOzombie);
    glGenBuffers(1, &VBOzombie);
    glBindVertexArray(VAOzombie);
    // load vertex data into vertex buffers
    glBindBuffer(GL_ARRAY_BUFFER, VBOzombie);
    glBufferData(GL_ARRAY_BUFFER, verticesZombie.size() * sizeof(Data), &verticesZombie[0], GL_STATIC_DRAW);
    // set the vertex attribute pointers
    // positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Data), (void *)0);
    // texture coords
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Data), (void *)offsetof(Data, TexCoords));

    // now do the same for the creeper block
    glGenVertexArrays(1, &VAOcreeper);
    glGenBuffers(1, &VBOcreeper);
    glBindVertexArray(VAOcreeper);
    // load vertex data into vertex buffers
    glBindBuffer(GL_ARRAY_BUFFER, VBOcreeper);
    glBufferData(GL_ARRAY_BUFFER, verticesCreeper.size() * sizeof(Data), &verticesCreeper[0], GL_STATIC_DRAW);
    // set the vertex attribute pointers
    // positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Data), (void *)0);
    // texture coords
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Data), (void *)offsetof(Data, TexCoords));

    loadTexture(texturePath, texture, true);
    getRandomPos();
    chooseMob();
}

void Mobs::getRandomPos()
{
    std::random_device rd;  // obtain a random number from hardware
    std::mt19937 gen(rd()); // seed the generator

    std::uniform_int_distribution<> xzPlane(1, terrainSize - 1); // define the range for x and z axis
    std::uniform_int_distribution<>yPlane(minHeight, maxHeight); // define the range for y axis

    currentPosition = glm::vec3(xzPlane(gen), yPlane(gen), xzPlane(gen)); // create random pos
}

void Mobs::chooseMob()
{
    std::random_device rd; // obtain a random number from hardware
    std::mt19937 gen(rd()); // seed the generator

    std::uniform_int_distribution<> numbers(1, 2); // define the range, we only have two mobs

    currentMob = numbers(gen); // choose mob (1 = zombie and 2 = creeper)
}

bool Mobs::spawnsInTree(glm::vec3 position)
{
    for (unsigned int i = 0; i < treePositions.size(); i++)
    {
        if (treePositions[i].x == position.x && treePositions[i].z == position.z) // is there an overlap?
        {
            return true;
        }
    }

    return false; // no overlap
}