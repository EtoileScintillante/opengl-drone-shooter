#include "glowstone.h"

GlowStone::GlowStone(std::vector<Data> verticesGlowStone, std::string texturePathGlowStone, std::vector < glm::vec3 > treePositions, unsigned int numGlowStones, unsigned int height)
{
    this->verticesGlowStone = verticesGlowStone;
    this->texturePathGlowStone = texturePathGlowStone;
    this->treePositions = treePositions;
    this->numGlowStones = numGlowStones;
    this->height = height;

    configureGlowStone();
}

void GlowStone::Draw(Shader &shader)
{
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureGlowStone);
    glBindVertexArray(VAO);
    for (unsigned int i = 0; i < numGlowStones; i++)
    {
        glm::mat4 modelGS = glm::mat4(1.0f);
        modelGS = glm::translate(modelGS, glowStonePositions[i]);
        modelGS = glm::scale(modelGS, glm::vec3(0.5f)); // make it a smaller cube
        shader.setMat4("model", modelGS);
        glDrawArrays(GL_TRIANGLES, 0, 36);
    }
}

void GlowStone::configureGlowStone()
{
    // create buffers/arrays for glow stone block
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    // load vertex data into vertex buffers
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, verticesGlowStone.size() * sizeof(Data), &verticesGlowStone[0], GL_STATIC_DRAW);
    // set the vertex attribute pointers
    // positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Data), (void *)0);
    // texture coords
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Data), (void *)offsetof(Data, TexCoords));

    loadTexture(texturePathGlowStone, textureGlowStone);
    if (glowStonePositions.size() == 0) {createPositions();} // create positions for the glow stones if they are not already provided
}

void GlowStone::createPositions()
{
    std::random_device rd; // obtain a random number from hardware
    std::mt19937 gen(rd()); // seed the generator

    std::uniform_int_distribution<> lights(0, treePositions.size() - 1); // generate random number between 0 and number of trees correspondng to the nth tree in treePositions
    std::vector < int > indexes; // the trees with their position index in this array will be getting a glow stone attached to them
    for (unsigned int i = 0; i < numGlowStones; i++)
    {
        int index = lights(gen); 
        indexes.push_back(index);
    }
    for (int i = 0; i < numGlowStones; i++)
    {
        // here we position the glow stone(s) in such a way that they stick to a tree
        // we subtract 1 from y otherwise the statement "Measured from the the bottom trunk block of a tree" is not correct.
        glm::vec3 vec = {treePositions[indexes[i]].x + 0.5f, treePositions[indexes[i]].y + height - 1, treePositions[indexes[i]].z}; 
        if (i % 3 == 0) {vec = {treePositions[indexes[i]].x, treePositions[indexes[i]].y + height - 1, treePositions[indexes[i]].z - 0.5f};} // to add some variation in the direction the glow stone(s) is/are pointing to
        glowStonePositions.push_back(vec);
    }

}

void GlowStone::loadTexture(std::string path, unsigned int ID)
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