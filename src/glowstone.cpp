#include "glowstone.h"

GlowStone::GlowStone(){};

GlowStone::GlowStone(std::vector< Vertex > verticesGlowStone, std::string texturePathGlowStone, std::vector < glm::vec3 > treePositions, unsigned int numGlowStones, float height)
{
    this->verticesGlowStone = verticesGlowStone;
    this->texturePathGlowStone = texturePathGlowStone;
    this->treePositions = treePositions;
    this->numGlowStones = numGlowStones;
    this->height = height;

    configureGlowStone();
}

void GlowStone::Draw(Shader &shader, glm::mat4 cameraView, glm::mat4 projection)
{
    // set uniforms
    shader.use();
    shader.setMat4("view", cameraView);
    shader.setMat4("projection", projection); 

    // draw
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
    glBindTexture(GL_TEXTURE_2D, 0);
}

void GlowStone::configureGlowStone()
{
    // create buffers/arrays for glow stone block
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    // load vertex data into vertex buffers
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, verticesGlowStone.size() * sizeof(Vertex), &verticesGlowStone[0], GL_STATIC_DRAW);
    // set the vertex attribute pointers
    // positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)0);
    // texture coords
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, TexCoords));

    loadTexture(texturePathGlowStone, textureGlowStone, true);
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