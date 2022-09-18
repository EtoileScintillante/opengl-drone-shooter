#include "tree.h"

Tree::Tree(){};

Tree::Tree(std::vector< Vertex > verticesTrunk, std::vector< Vertex > verticesLeaves, std::string pathTrunkTexture, std::string pathLeavesTexture, unsigned int numTrees, unsigned int heightTree, unsigned int terrainSize, float groundLevel, float blockSize)
{
    this->verticesTrunk = verticesTrunk;
    this->verticesLeaves = verticesLeaves;
    this->pathTrunkTexture = pathTrunkTexture;
    this->pathLeavesTexture = pathLeavesTexture;
    this->numTrees = numTrees;
    this->heightTree = heightTree;
    this->terrainSize = terrainSize;
    this->groundLevel = groundLevel;
    this->blockSize = blockSize;

    configureTree();
}

void Tree::Draw(Shader &shaderTrunk, Shader &shaderLeaves, glm::mat4 cameraView, glm::mat4 projection)
{   
    // set uniforms
    shaderTrunk.use();
    shaderTrunk.setMat4("view", cameraView);
    shaderTrunk.setMat4("projection", projection); 

    // first render tree trunk(s)
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureTrunk);
    glBindVertexArray(VAOtrunk);
    for (unsigned int i = 0; i < treePositions.size(); i++)
    {
        glm::vec3 tree = treePositions[i];
        for (unsigned int j = 0; j < heightTree; j++)
        {
            glm::mat4 modelTree = glm::mat4(1.0f);
            modelTree = glm::translate(modelTree, tree);
            shaderTrunk.setMat4("model", modelTree);
            glDrawArrays(GL_TRIANGLES, 0, 36);
            tree.y += blockSize; // increase y with BLOCK_SIZE to place next wooden block exactly on top of the one before that
        }
    }
    glBindTexture(GL_TEXTURE_2D, 0);

    // set uniforms
    shaderLeaves.use();
    shaderLeaves.setMat4("view", cameraView);
    shaderLeaves.setMat4("projection", projection); 

    // then render leaves
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureLeaves);
    glBindVertexArray(VAOleaves);
    for (unsigned int i = 0; i < leavesPositions.size(); i++)
    {
        glm::mat4 modelLeaves = glm::mat4(1.0f);
        modelLeaves = glm::translate(modelLeaves, leavesPositions[i]);
        shaderLeaves.setMat4("model", modelLeaves);
        glDrawArrays(GL_TRIANGLES, 0, 36);
    }
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Tree::configureTree()
{
    // create buffers/arrays for trunk part first
    glGenVertexArrays(1, &VAOtrunk);
    glGenBuffers(1, &VBOtrunk);
    glBindVertexArray(VAOtrunk);
    // load vertex data into vertex buffers
    glBindBuffer(GL_ARRAY_BUFFER, VBOtrunk);
    glBufferData(GL_ARRAY_BUFFER, verticesTrunk.size() * sizeof(Vertex), &verticesTrunk[0], GL_STATIC_DRAW);
    // set the vertex attribute pointers
    // positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)0);
    // texture coords
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, TexCoords));

    // now do the same for the leaves
    glGenVertexArrays(1, &VAOleaves);
    glGenBuffers(1, &VBOleaves);
    glBindVertexArray(VAOleaves);
    // load vertex data into vertex buffers
    glBindBuffer(GL_ARRAY_BUFFER, VBOleaves);
    glBufferData(GL_ARRAY_BUFFER, verticesLeaves.size() * sizeof(Vertex), &verticesLeaves[0], GL_STATIC_DRAW);
    // set the vertex attribute pointers
    // positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)0);
    // texture coords
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, TexCoords));

    loadTexture(pathTrunkTexture, textureTrunk, true);
    loadTexture(pathLeavesTexture, textureLeaves, true);
    if (treePositions.size() == 0) {createTreePositions();} // generate positions for trees if not already done
    createLeavesPositions();
}

void Tree::createTreePositions()
{
    std::random_device rd;  // obtain a random number from hardware
    std::mt19937 gen(rd()); // seed the generator

    // create random positions
    std::uniform_int_distribution<> xzPlane(1, terrainSize - 1); // define the range for x and z axis
    for (unsigned int i = 0; i < numTrees; i++)
    {
        float posX = xzPlane(gen);
        float posZ = xzPlane(gen);
        glm::vec3 vec = {posX, groundLevel + blockSize + 0.0001f, posZ};
        treePositions.push_back(vec);
    }
}

void Tree::createLeavesPositions()
{
    for (unsigned int i = 0; i < treePositions.size(); i++)
    {
        // 5 leaves blocks per tree
        glm::vec3 leavesBlock1 = {treePositions[i].x - blockSize - 0.0001f, groundLevel + heightTree, treePositions[i].z}; // negative x side
        glm::vec3 leavesBlock2 = {treePositions[i].x + blockSize + 0.0001f, groundLevel + heightTree, treePositions[i].z}; // positive x side
        glm::vec3 leavesBlock3 = {treePositions[i].x, groundLevel + heightTree, treePositions[i].z - blockSize - 0.0001f}; // negative z side
        glm::vec3 leavesBlock4 = {treePositions[i].x, groundLevel + heightTree, treePositions[i].z + blockSize + 0.0001f}; // positive z side
        glm::vec3 leavesBlock5 = {treePositions[i].x, groundLevel + heightTree + blockSize + 0.0001f, treePositions[i].z}; // top

        leavesPositions.push_back(leavesBlock1);
        leavesPositions.push_back(leavesBlock2);
        leavesPositions.push_back(leavesBlock3);
        leavesPositions.push_back(leavesBlock4);
        leavesPositions.push_back(leavesBlock5);
    }
}