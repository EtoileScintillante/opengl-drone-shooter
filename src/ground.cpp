#include "ground.h"

Ground::Ground(std::vector<Data> verticesDirt, std::vector<Data> verticesStone, std::string texturePathDirt, std::string texturePathStone, unsigned int terrainSize, float groundLevel)
{
    this->verticesDirt = verticesDirt;
    this->verticesStone = verticesStone;
    this->texturePathDirt = texturePathDirt;
    this->texturePathStone = texturePathStone;
    this->terrainSize = terrainSize;
    this->groundLevel = groundLevel;

    configureGround();
}

void Ground::Draw(Shader &shader)
{
    int indexOrder = 0;
    for (unsigned int i = 0; i < terrainSize; i++)
    {
        for (unsigned int j = 0; j < terrainSize; j++)
        {
            int num = blockOrder[indexOrder + j];
            if (num == 1) // dirt block
            {
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, textureDirt);
                glBindVertexArray(VAOdirt);
                glm::mat4 modelDirt = glm::mat4(1.0f);
                modelDirt = glm::translate(modelDirt, glm::vec3(i, groundLevel, j));
                shader.setMat4("model", modelDirt);
                glDrawArrays(GL_TRIANGLES, 0, 36);
            }
            if (num == 2) // stone block
            {
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, textureStone);
                glBindVertexArray(VAOstone);
                glm::mat4 modelStone = glm::mat4(1.0f);
                modelStone = glm::translate(modelStone, glm::vec3(i, groundLevel, j));
                shader.setMat4("model", modelStone);
                glDrawArrays(GL_TRIANGLES, 0, 36);
            }
        }
        indexOrder += terrainSize; // update index otherwise we keep indexing the first 50 spots
    }
}

void Ground::configureGround()
{
    // create buffers/arrays for dirt block first
    glGenVertexArrays(1, &VAOdirt);
    glGenBuffers(1, &VBOdirt);
    glBindVertexArray(VAOdirt);
    // load vertex data into vertex buffers
    glBindBuffer(GL_ARRAY_BUFFER, VBOdirt);
    glBufferData(GL_ARRAY_BUFFER, verticesDirt.size() * sizeof(Data), &verticesDirt[0], GL_STATIC_DRAW);
    // set the vertex attribute pointers
    // positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Data), (void *)0);
    // texture coords
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Data), (void *)offsetof(Data, TexCoords));

    // now do the same for the stone
    glGenVertexArrays(1, &VAOstone);
    glGenBuffers(1, &VBOstone);
    glBindVertexArray(VAOstone);
    // load vertex data into vertex buffers
    glBindBuffer(GL_ARRAY_BUFFER, VBOstone);
    glBufferData(GL_ARRAY_BUFFER, verticesStone.size() * sizeof(Data), &verticesStone[0], GL_STATIC_DRAW);
    // set the vertex attribute pointers
    // positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Data), (void *)0);
    // texture coords
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Data), (void *)offsetof(Data, TexCoords));

    loadTexture(texturePathDirt, textureDirt);
    loadTexture(texturePathStone, textureStone);
    orderOfBlocks();
}

void Ground::orderOfBlocks()
{
    std::random_device rd; // obtain a random number from hardware
    std::mt19937 gen(rd()); // seed the generator

    std::uniform_int_distribution<> numbers(1, 2); 
    for (unsigned int i = 0; i < (terrainSize * terrainSize); i++)
    {
        srand(time(NULL));
        int choice = numbers(gen);
        blockOrder.push_back(choice);
    }
}

void Ground::loadTexture(std::string path, unsigned int ID)
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