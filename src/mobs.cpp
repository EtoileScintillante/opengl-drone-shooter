#include "mobs.h"

Mobs::Mobs(std::vector < Data > verticesZombie, std::vector < Data > verticesCreeper, std::vector < glm::vec3 > treePositions, std::string texturePath, unsigned int terrainSize, float groundY, float blockSize, float minHeight, float maxHeight)
{
    this->verticesZombie = verticesZombie;
    this->verticesCreeper = verticesCreeper;
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
        glBindVertexArray(zombieVAO);
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, currentPosition);
        model = glm::rotate(model, time, glm::vec3(0.0f, 1.0f, 0.0f));
        shader.setMat4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 36);
    }
    if (currentMob == 2) // 2 = creeper
    {
        glBindVertexArray(creeperVAO);
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, currentPosition);
        model = glm::rotate(model, time, glm::vec3(0.0f, 1.0f, 0.0f));
        shader.setMat4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 36);
    }
}

// TODO: finish this function (mob has to die and new mob has to spawn in new position)
void Mobs::collisionDetection(glm::vec3 bulletStartPos, glm::vec3 bulletDir, float bulletRange)
{   
    // comstruct ray object with start position and direction of bullet (but first we need to convert them to Vec3)
    Vec3<float> orig = {bulletStartPos.x, bulletStartPos.y, bulletStartPos.z};
    Vec3<float> dir = {bulletDir.x, bulletDir.y, bulletDir.z};
    Ray ray(orig, dir);

    // construct bounding box object using the current position of the mob (also conversion to vec3 needed)
    Vec3<float> vmin = {currentPosition.x - blockSize/2, currentPosition.y - blockSize/2, currentPosition.z - blockSize/2};
    Vec3<float> vmax = {currentPosition.x + blockSize/2, currentPosition.y + blockSize/2, currentPosition.z + blockSize/2};
    AABBox box(vmin, vmax);

    if (box.intersect(ray, bulletRange) == true)
    {
        std::cout << "HIT" << std::endl;
    }
    else {std::cout << "NO HIT" << std::endl;}
}

void Mobs::configureMobs()
{
    // create buffers/arrays for zombie block first
    glGenVertexArrays(1, &zombieVAO);
    glGenBuffers(1, &zombieVBO);
    glBindVertexArray(zombieVAO);
    // load vertex data into vertex buffers
    glBindBuffer(GL_ARRAY_BUFFER, zombieVBO);
    glBufferData(GL_ARRAY_BUFFER, verticesZombie.size() * sizeof(Data), &verticesZombie[0], GL_STATIC_DRAW);
    // set the vertex attribute pointers
    // positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Data), (void *)0);
    // texture coords
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Data), (void *)offsetof(Data, TexCoords));

    // now do the same for the creeper block
    glGenVertexArrays(1, &creeperVAO);
    glGenBuffers(1, &creeperVBO);
    glBindVertexArray(creeperVAO);
    // load vertex data into vertex buffers
    glBindBuffer(GL_ARRAY_BUFFER, creeperVBO);
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

    currentPosition = glm::vec3(xzPlane(gen), yPlane(gen) + groundY, xzPlane(gen)); // create random pos
}

void Mobs::chooseMob()
{
    std::random_device rd; // obtain a random number from hardware
    std::mt19937 gen(rd()); // seed the generator

    std::uniform_int_distribution<> numbers(1, 2); // define the range, we only have two mobs

    currentMob = numbers(gen);
    std::cout << currentMob << std::endl;
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