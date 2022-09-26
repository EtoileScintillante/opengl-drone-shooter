#include "skybox.h"

SkyBox::SkyBox(){};

SkyBox::SkyBox(std::vector< std::string > filenames, std::string dirName)
{
    this->filenames = filenames;
    this->dirName = dirName;

    skyboxTexture = 8; // skybox texture ID

    configureSkybox();
}

void SkyBox::Draw(Shader &shader, glm::mat4 cameraView, glm::mat4 projection)
{
    // set uniforms
    shader.use();
    glm::mat4 view = glm::mat4(glm::mat3(cameraView)); // remove translation from the view matrix
    shader.setMat4("view", view);
    shader.setMat4("projection", projection);

    // bind texture and draw
    glBindVertexArray(VAO);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTexture);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
}

void SkyBox::configureSkybox()
{
    skyboxVertices = getSkyboxVertexData();

    // configure buffer/array
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    // load vertex data into vertex buffer
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, skyboxVertices.size()*sizeof(float), &skyboxVertices[0], GL_STATIC_DRAW);
    // set the vertex attribute pointer
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    // load skybox texture, but first create vector containing paths to skybox textures
    std::vector< std::string > faces;
    for (unsigned int i = 0; i < filenames.size(); i++)
    {
        std::string s = dirName + "/" + filenames[i];
        faces.push_back(s);
    }
    loadCubemap(faces, skyboxTexture);
}

std::vector<float> SkyBox::getSkyboxVertexData()
{
    std::vector<float> v = {
        // positions
        -1.0f, 1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, 1.0f, -1.0f,
        -1.0f, 1.0f, -1.0f,

        -1.0f, -1.0f, 1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f, 1.0f, -1.0f,
        -1.0f, 1.0f, -1.0f,
        -1.0f, 1.0f, 1.0f,
        -1.0f, -1.0f, 1.0f,

        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f, 1.0f,
        -1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, -1.0f, 1.0f,
        -1.0f, -1.0f, 1.0f,

        -1.0f, 1.0f, -1.0f,
        1.0f, 1.0f, -1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f, 1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f, 1.0f,
        1.0f, -1.0f, 1.0f
        };

    return v;
}