#include "world.h"

const unsigned int World::N_TREES = 15;
const unsigned int World::N_FLOWERS = (N_TREES / 2) * 4;
const float World::TERRAIN_SIZE = 30;
const float World::GROUND_Y = -1.8f;

World::World()
{
    setupWorld();
}

World::~World()
{
    delete[] treeModelMatrices;
    delete[] flowerModelMatrices;
}

void World::Draw()
{
    drawSkyBox();
    drawGround();
    drawFlowers();
    drawTrees();
}

std::vector<glm::vec3> World::getTreePositions() const
{
    return treePos;
}

void World::setupWorld()
{
    // create shaders
    shaderModel = Shader("shaders/instancing.vert", "shaders/model.frag");
    shaderGround = Shader("shaders/ground.vert", "shaders/ground.frag");
    shaderSkybox = Shader("shaders/skybox.vert", "shaders/skybox.frag");

    // load models
    flowers = Model("resources/models/flowers/anemone_hybrida.obj", true);
    tree = Model("resources/models/trees/trees9.obj", true);

    // generate positions, model matrices and set up instanced array buffers for trees and flowers
    createTreePositions();
    createFlowerPositions();
    createTreeModelMatrices();
    createFlowerModelMatrices();
    setupInstancedArray(tree, treeBuffer, treeModelMatrices, N_TREES);
    setupInstancedArray(flowers, flowerBuffer, flowerModelMatrices, N_FLOWERS);

    // initialize skybox object
    std::vector<std::string> filenames = {"right.jpg", "left.jpg", "top.jpg", "bottom.jpg", "front.jpg", "back.jpg"};
    std::string dirName = "resources/skybox";
    skybox = SkyBox(filenames, dirName);

    // get ground vertex data and set up the buffers
    groundVertices = getGroundVertexData();
    glGenVertexArrays(1, &groundVAO);
    glGenBuffers(1, &groundVBO);
    glBindVertexArray(groundVAO);
    glBindBuffer(GL_ARRAY_BUFFER, groundVBO);
    glBufferData(GL_ARRAY_BUFFER, groundVertices.size() * sizeof(float), &groundVertices[0], GL_STATIC_DRAW);
    // positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
    // texture coords
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float)));
    glBindVertexArray(0);

    // load ground texture
    groundTexture = TextureFromFile("field.png", "resources/textures", false);
}

void World::drawTrees()
{
    // set uniforms and draw the trees
    shaderModel.use();
    shaderModel.setMat4("projection", projection);
    shaderModel.setMat4("view", view);
    shaderModel.setInt("texture_diffuse1", 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, tree.textures_loaded[1].id); 
    glBindVertexArray(tree.meshes[1].VAO); // render only one specific tree (the whole model is made up of more trees)
    glDrawElementsInstanced(GL_TRIANGLES, static_cast<unsigned int>(tree.meshes[1].indices.size()), GL_UNSIGNED_INT, 0, N_TREES);
    glBindVertexArray(0);
}

void World::drawGround()
{
    // set uniforms and draw the ground
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
    // set uniforms 
    shaderModel.use();
    shaderModel.setMat4("projection", projection);
    shaderModel.setMat4("view", view);
    shaderModel.setInt("texture_diffuse1", 0);

    // note that not all textures and meshes are used to render the model
    int textureIDs[] = {0, 1, 3, 6, 6, 8};
    int meshIDs[] = {0, 1, 2, 4, 5, 6}; 
    for (unsigned int i = 0; i < 6; i++)
    {
        // draw flowers
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, flowers.textures_loaded[textureIDs[i]].id);
        glBindVertexArray(flowers.meshes[meshIDs[i]].VAO);
        glDrawElementsInstanced(GL_TRIANGLES, static_cast<unsigned int>(flowers.meshes[meshIDs[i]].indices.size()), GL_UNSIGNED_INT, 0, N_FLOWERS);
        glBindVertexArray(0);
        glActiveTexture(GL_TEXTURE0);
    }
}

std::vector<float> World::getGroundVertexData() const
{
    std::vector<float> v = {
        // positions                                     // texcoords
        TERRAIN_SIZE / 2, GROUND_Y, TERRAIN_SIZE / 2,    TERRAIN_SIZE / 2, 0.0f,
       -TERRAIN_SIZE / 2, GROUND_Y, TERRAIN_SIZE / 2,    0.0f, 0.0f,
       -TERRAIN_SIZE / 2, GROUND_Y, -TERRAIN_SIZE / 2,   0.0f, TERRAIN_SIZE / 2,

        TERRAIN_SIZE / 2, GROUND_Y, TERRAIN_SIZE / 2,    TERRAIN_SIZE / 2, 0.0f,
       -TERRAIN_SIZE / 2, GROUND_Y, -TERRAIN_SIZE / 2,   0.0f, TERRAIN_SIZE / 2,
        TERRAIN_SIZE / 2, GROUND_Y, -TERRAIN_SIZE / 2,   TERRAIN_SIZE / 2, TERRAIN_SIZE / 2
        };

    return v;
}

void World::createTreePositions()
{
    std::random_device rd;  // obtain a random number from hardware
    std::mt19937 gen(rd()); // seed the generator

    // create random positions
    std::uniform_int_distribution<> xzPlane((-TERRAIN_SIZE / 2) + 2, (TERRAIN_SIZE / 2) - 2); // define the range for x and z axis
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
    // half of the trees will be surrounded by four flower models
    for (unsigned int i = 0; i < N_TREES / 2; i++)
    {
        glm::vec3 pos = glm::vec3(treePos[i].x + 1.2, treePos[i].y - 0.1, treePos[i].z);
        glm::vec3 pos1 = glm::vec3(treePos[i].x, treePos[i].y - 0.1, treePos[i].z);
        glm::vec3 pos2 = glm::vec3(treePos[i].x, treePos[i].y - 0.1, treePos[i].z - 1.0);
        glm::vec3 pos3 = glm::vec3(treePos[i].x + 1.2, treePos[i].y - 0.1, treePos[i].z - 1.0);
        flowerPos.push_back(pos);
        flowerPos.push_back(pos1);
        flowerPos.push_back(pos2);
        flowerPos.push_back(pos3);
    }
}

void World::createTreeModelMatrices()
{
    for (unsigned int i = 0; i < N_TREES; i++)
    {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, treePos[i]); // translation
        model = glm::scale(model, glm::vec3(0.4f)); // make model a bit smaller
        treeModelMatrices[i] = model; // add matrix to array of matrices
    }
}

void World::createFlowerModelMatrices()
{
    for (unsigned int i = 0; i < N_FLOWERS; i++)
    {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, flowerPos[i]); // translation
        float rotAngle = static_cast<float>((rand() % 360)); 
        model = glm::rotate(model, rotAngle, glm::vec3(0.0f, 1.0f, 0.0f)); // add (semi)random rotation
        model = glm::scale(model, glm::vec3(2.5f)); // make model bigger
        flowerModelMatrices[i] = model; // add matrix to array of matrices
    }
}

void World::setupInstancedArray(Model &model, unsigned int buffer, glm::mat4 *modelMatrices, int amount)
{
    // configure instanced array
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, amount * sizeof(glm::mat4), &modelMatrices[0], GL_STATIC_DRAW);

    // set transformation matrices as an instance vertex attribute 
    for (unsigned int i = 0; i < model.meshes.size(); i++)
    {
        unsigned int VAO = model.meshes[i].VAO;
        glBindVertexArray(VAO);
        // set attribute pointers for matrix (4 times vec4)
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)0);
        glEnableVertexAttribArray(4);
        glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4)));
        glEnableVertexAttribArray(5);
        glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(2 * sizeof(glm::vec4)));
        glEnableVertexAttribArray(6);
        glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(3 * sizeof(glm::vec4)));

        glVertexAttribDivisor(3, 1);
        glVertexAttribDivisor(4, 1);
        glVertexAttribDivisor(5, 1);
        glVertexAttribDivisor(6, 1);

        glBindVertexArray(0);
    }
}