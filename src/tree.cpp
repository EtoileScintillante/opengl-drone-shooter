#include "tree.h"

Tree::Tree(std::vector<Data> verticesTrunk, std::vector<Data> verticesLeaves, std::string pathTrunkTexture, std::string pathLeavesTexture, unsigned int numTrees, unsigned int heightTree, unsigned int terrainSize, float groundLevel, float blockSize)
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

void Tree::Draw(Shader &shaderTrunk, Shader &shaderLeaves)
{
    // first render tree trunk(s)
    shaderTrunk.setInt("ourTexture", 0);
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

    // then render leaves
    shaderLeaves.setInt("ourTexture", 0);
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
}

void Tree::configureTree()
{
    // create buffers/arrays for trunk part first
    glGenVertexArrays(1, &VAOtrunk);
    glGenBuffers(1, &VBOtrunk);
    glBindVertexArray(VAOtrunk);
    // load data into Data buffers
    glBindBuffer(GL_ARRAY_BUFFER, VBOtrunk);
    glBufferData(GL_ARRAY_BUFFER, verticesTrunk.size() * sizeof(Data), &verticesTrunk[0], GL_STATIC_DRAW);
    // set the Data attribute pointers
    // Data positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Data), (void *)0);
    // Data texture coords
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Data), (void *)offsetof(Data, TexCoords));

    // now do the same for the leaves
    glGenVertexArrays(1, &VAOleaves);
    glGenBuffers(1, &VBOleaves);
    glBindVertexArray(VAOleaves);
    // load data into Data buffers
    glBindBuffer(GL_ARRAY_BUFFER, VBOleaves);
    glBufferData(GL_ARRAY_BUFFER, verticesLeaves.size() * sizeof(Data), &verticesLeaves[0], GL_STATIC_DRAW);
    // set the Data attribute pointers
    // Data positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Data), (void *)0);
    // Data texture coords
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Data), (void *)offsetof(Data, TexCoords));

    loadTexture(pathTrunkTexture, textureTrunk);
    loadTexture(pathLeavesTexture, textureLeaves);
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

void Tree::loadTexture(std::string path, unsigned int ID)
{
    const char *c = path.c_str(); // convert std::string to const char*

    glGenTextures(1, &ID);

    stbi_set_flip_vertically_on_load(true);
    int width, height, nrComponents;
    unsigned char *data = stbi_load(c, &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else
        {
            format = GL_RGBA;
        }

        glBindTexture(GL_TEXTURE_2D, ID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, format == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, format == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }
}