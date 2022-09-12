#include "mobs.h"

Mobs::Mobs(){};

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
    deathTime = 0;
    hasDied = false;

    configureMobs();
}

void Mobs::Spawn(Shader &shader, float time, glm::mat4 cameraView, glm::mat4 projection)
{
    // draw mob if it is alive
    if (!hasDied)
    {
        // set uniforms
        shader.use();
        shader.setMat4("view", cameraView);
        shader.setMat4("projection", projection);
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, currentPosition);
        model = glm::rotate(model, time, glm::vec3(0.0f, 1.0f, 0.0f));
        shader.setMat4("model", model);

        // draw
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);
        if (currentMob == 1) // 1 = zombie
        {
            glBindVertexArray(zombieVAO);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
        if (currentMob == 2) // 2 = creeper
        {
            glBindVertexArray(creeperVAO);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
        glBindTexture(GL_TEXTURE_2D, 0);
    }
}

void Mobs::collisionDetection(glm::vec3 bulletStartPos, glm::vec3 bulletDir, float bulletRange)
{   
    // comstruct ray object with start position and direction of bullet 
    glm::vec3 orig = bulletStartPos;
    glm::vec3 dir = bulletDir;
    Ray ray(orig, dir);

    // construct bounding box object using the current position of the mob 
    glm::vec3 vmin = {currentPosition.x - blockSize/2, currentPosition.y - blockSize/2, currentPosition.z - blockSize/2};
    glm::vec3 vmax = {currentPosition.x + blockSize/2, currentPosition.y + blockSize/2, currentPosition.z + blockSize/2};
    AABBox box(vmin, vmax);

    if (box.intersect(ray, bulletRange) == true)
    {
        hasDied = true;
    }
}

void Mobs::died(Model skullModel, Shader skullShader, glm::mat4 cameraView, glm::mat4 projection, float deltaTime, float time)
{
    deathTime += deltaTime;

    if (deathTime < 1.0) // skull will be visible for about one second
    {
        // set uniforms
        skullShader.use();
        skullShader.setMat4("view", cameraView);
        skullShader.setMat4("projection", projection);
        glm::mat4 model = glm::mat4(1.0f);
        // here we add some rotations so that the skull faces the same direction as the face of the  mob
        model = glm::translate(model, glm::vec3(currentPosition.x, currentPosition.y - 0.42f, currentPosition.z));
        model = glm::rotate(model, glm::radians(270.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::rotate(model, time, glm::vec3(0.0f, 0.0f, 1.0f));
        // and we make the skull smaller so that it fits in the mob block (as if the mob really has a skull inside)
        model = glm::scale(model, glm::vec3(0.042f));
        skullShader.setMat4("model", model);
        // draw
        skullModel.Draw(skullShader);
    }
    else
    {
        // reset variables, get new position and choose new mob
        deathTime = 0; 
        hasDied = false;
        getRandomPos();
        chooseMob();
    }
}

glm::vec3 Mobs::getCurrentPos()
{
    return currentPosition;
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

    std::uniform_int_distribution<> xzPlane(1, terrainSize - 1);  // define the range for x and z axis
    std::uniform_int_distribution<> yPlane(minHeight, maxHeight); // define the range for y axis

    glm::vec3 newPos = glm::vec3(xzPlane(gen), yPlane(gen) + groundY, xzPlane(gen));

    if (!spawnsInTree(newPos)) {currentPosition = newPos;} // make sure the mob does not spawn inside a tree
    else {getRandomPos();}

}

void Mobs::chooseMob()
{
    std::random_device rd;  // obtain a random number from hardware
    std::mt19937 gen(rd()); // seed the generator

    std::uniform_int_distribution<> numbers(1, 2); // define the range, we only have two mobs

    currentMob = numbers(gen);
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