#include "world.h"

const unsigned int World::N_TREES = 15;
const float World::TERRAIN_SIZE = 30;
const float World::GROUND_Y = -1.8f;

World::World()
{
    setupWorld();
}

void World::Draw()
{
    drawTrees();
    drawGround();
    drawSkyBox();
    drawFlowers();
}

void World::setupWorld()
{
    // create shaders
    shaderModel = Shader("shaders/model_loading.vert", "shaders/model_loading.frag");
    shaderGround = Shader("shaders/ground.vert", "shaders/ground.frag");
    shaderSkybox = Shader("shaders/skybox.vert", "shaders/skybox.frag");

    // get ground vertex data and set up the buffers
    groundVertices = getGroundVertexData();
    glGenVertexArrays(1, &groundVAO);
    glGenBuffers(1, &groundVBO);
    glBindVertexArray(groundVAO);
    glBindBuffer(GL_ARRAY_BUFFER, groundVBO);
    glBufferData(GL_ARRAY_BUFFER, groundVertices.size()*sizeof(float), &groundVertices[0], GL_STATIC_DRAW);
    // positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
    // texture coords
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float)));
    glBindVertexArray(0);

    // load ground texture
    groundTexture = TextureFromFile("field.png", "resources/textures", false);

    // get skybox vertex data and initialize skybox object
    skyboxVertices = getSkyboxVertexData();
    std::vector<std::string> filenames = {"right.jpg", "left.jpg", "top.jpg", "bottom.jpg", "front.jpg", "back.jpg"};
    std::string dirName = "resources/skybox";
    skybox = SkyBox(skyboxVertices, filenames, dirName);

    // load models
    flowers = Model("resources/models/plant/anemone_hybrida.obj", true);
    tree = Model("resources/models/trees/trees9.obj", true);

    // generate positions
    createTreePositions();
    createFlowerPositions();
}

void World::drawTrees()
{
    // set uniforms for tree shader and draw the trees
    shaderModel.use();
    shaderModel.setMat4("projection", projection);
    shaderModel.setMat4("view", view);
    for (unsigned i = 0; i < World::N_TREES; i++)
    {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, treePos[i]);
        model = glm::scale(model, glm::vec3(0.4f));
        shaderModel.setMat4("model", model);
        tree.drawSpecificMesh(shaderModel, 1);
    }
}

void World::drawGround()
{
    // set uniforms for ground shader and draw the ground
    shaderGround.use();
    shaderGround.setMat4("projection", projection);
    shaderGround.setMat4("view", view);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, groundTexture);
    glBindVertexArray(groundVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    // set everything back to default
    glBindVertexArray(0);
    glActiveTexture(GL_TEXTURE0);
}

void World::drawSkyBox()
{
    // render skybox
    glDepthFunc(GL_LEQUAL);
    skybox.Draw(shaderSkybox, view, projection);
    glDepthFunc(GL_LESS);
}

void World::drawFlowers()
{
    // set uniforms for tree shader and draw the trees
    shaderModel.use();
    shaderModel.setMat4("projection", projection);
    shaderModel.setMat4("view", view);
    for (unsigned int i = 0; i < flowerPos.size(); i++)
    {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, flowerPos[i]);
        model = glm::scale(model, glm::vec3(2.5f));
        shaderModel.setMat4("model", model);
        flowers.Draw(shaderModel);
    }
}

std::vector<float> World::getSkyboxVertexData()
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

std::vector<float> World::getGroundVertexData()
{
    std::vector<float> v = {
        // positions                                     // texcoords
         TERRAIN_SIZE / 2, GROUND_Y,  TERRAIN_SIZE / 2,  TERRAIN_SIZE / 2, 0.0f,
        -TERRAIN_SIZE / 2, GROUND_Y,  TERRAIN_SIZE / 2,  0.0f, 0.0f,
        -TERRAIN_SIZE / 2, GROUND_Y, -TERRAIN_SIZE / 2,  0.0f, TERRAIN_SIZE / 2,

         TERRAIN_SIZE / 2, GROUND_Y,  TERRAIN_SIZE / 2,  TERRAIN_SIZE / 2, 0.0f,
        -TERRAIN_SIZE / 2, GROUND_Y, -TERRAIN_SIZE / 2,  0.0f, TERRAIN_SIZE / 2,
         TERRAIN_SIZE / 2, GROUND_Y, -TERRAIN_SIZE / 2,  TERRAIN_SIZE / 2, TERRAIN_SIZE / 2
        };

    return v;
}

void World::createTreePositions()
{
    std::random_device rd;  // obtain a random number from hardware
    std::mt19937 gen(rd()); // seed the generator

    // create random positions
    std::uniform_int_distribution<> xzPlane((-TERRAIN_SIZE / 2) + 1, (TERRAIN_SIZE / 2) - 1); // define the range for x and z axis
    for (unsigned int i = 0; i < N_TREES; i++)
    {
        float posX = xzPlane(gen);
        float posZ = xzPlane(gen);
        glm::vec3 vec = {posX, GROUND_Y, posZ}; 
        treePos.push_back(vec);
    }
}

void World::createFlowerPositions()
{
    for (unsigned int i = 0; i < N_TREES; i++)
    {
        glm::vec3 pos = glm::vec3(treePos[i].x + 1.2, treePos[i].y - 0.1, treePos[i].z);
        glm::vec3 pos1 = glm::vec3(treePos[i].x - 0.5, treePos[i].y - 0.1, treePos[i].z);
        flowerPos.push_back(pos);
        flowerPos.push_back(pos1);
    }
}