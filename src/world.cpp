#include "world.h"

const unsigned int World::N_TREES = 20;
const unsigned int World::N_SURROUNDINGS = 30;
const float World::TERRAIN_SIZE = 30;
const float World::GROUND_Y = -1.8f;

World::World()
{
    setupWorld();
}

World::World(const std::string& envType)
{
    // convert envType to lowercase
    std::string lowercaseEnvType = envType;
    std::transform(lowercaseEnvType.begin(), lowercaseEnvType.end(), lowercaseEnvType.begin(),
                   [](unsigned char c) { return std::tolower(c); });

    // check if envType is valid
    if (lowercaseEnvType == "desert" || lowercaseEnvType == "snow" ||
        lowercaseEnvType == "forest" || lowercaseEnvType == "night") {
        // set the environmentType if it's valid
        environmentType = lowercaseEnvType;
    }
    else
    {
        std::cout << "Invalid environment type: " << envType << std::endl;
        std::cout << "Valid types are: desert, forest, snow or night.\n";
        std::cout << "Program will now randomly choose one.\n";
    }

    // continue with the setup
    // in case if envType is not valid, it will choose a random env. type
    setupWorld();
}

void World::Draw(glm::mat4 View, glm::mat4 Projection)
{
    view = View;
    projection = Projection;

    // depth test
    glEnable(GL_DEPTH_TEST);

    // draw objects
    drawSkyBox(false);
    drawGround();
    drawSurroundings();
    drawTrees();
}

std::vector<glm::vec3> World::getTreePositions() const
{
    return treePos;
}

void World::setupWorld()
{
    if (environmentType.empty())
    {
        // select environment type randomly
        std::vector<std::string> envTypes = {"desert", "forest", "snow", "night"};
        // set up a random number gen that picks an index
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, envTypes.size() - 1);
        int idx = dis(gen);
        // use the selected environment type
        environmentType = envTypes[idx];
    }
    
    // create shaders
    shaderModel = Shader("shaders/instancing.vert", "shaders/model.frag");
    shaderGround = Shader("shaders/ground.vert", "shaders/ground.frag");
    shaderSkybox = Shader("shaders/skybox.vert", "shaders/skybox.frag");

    // load correct models, skybox and ground texture
    std::string dirName;
    if (environmentType == "desert")
    {
        surrounding = Model("resources/models/rocks/rock_desert/rock.obj", true);
        tree = Model("resources/models/trees/desert_land_tree/hoewa_Forsteriana_1.obj", true);
        dirName = "resources/skybox/desert_land/";
        groundTexture = TextureFromFile("desert_ground.png", "resources/textures", false);
    }
    if (environmentType == "forest")
    {
        surrounding = Model("resources/models/flowers/anemone_hybrida.obj", true);
        tree = Model("resources/models/trees/forest_land_tree/trees9.obj", true);
        dirName = "resources/skybox/forest_land/";
        groundTexture = TextureFromFile("forest_ground.png", "resources/textures", false);
    }
    if (environmentType == "snow")
    {   
        surrounding = Model("resources/models/rocks/rock_snow/rock.obj", true);
        tree = Model("resources/models/trees/snow_land_tree/Tree_Red-spruce.obj", true);
        dirName = "resources/skybox/snow_land/";
        groundTexture = TextureFromFile("snow_ground.png", "resources/textures", false);
    }
    if (environmentType == "night")
    {   
        surrounding = Model("resources/models/pumpkin/pumpkin face.obj", true);
        tree = Model("resources/models/trees/night_land_tree/Tree_001.obj", true);
        dirName = "resources/skybox/night_land/";
        groundTexture = TextureFromFile("night_ground.png", "resources/textures", false);
    }
    
    // generate positions, model matrices and set up instanced array buffers for trees and flowers
    createTreePositions();
    createSurroundingPositions();
    createTreeModelMatrices();
    createSurroundingModelMatrices();
    setupInstancedArray(tree, treeBuffer, treeModelMatrices, N_TREES);
    setupInstancedArray(surrounding, surroundingBuffer, surroundingModelMatrices, N_SURROUNDINGS);

    // initialize skybox object
    std::vector<std::string> filenames = {"px.jpg", "nx.jpg", "py.jpg", "ny.jpg", "pz.jpg", "nz.jpg"};
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
}

void World::drawTrees()
{
    // set uniforms and draw the trees
    shaderModel.use();
    shaderModel.setMat4("projection", projection);
    shaderModel.setMat4("view", view);
    shaderModel.setInt("texture_diffuse1", 0);

    if (environmentType == "desert")
    {
        int meshID = 0;
        for (unsigned int i = 0; i < 6; i++)
        {
            // mesh at index 0 has textures at indices 0,1,2
            // mesh at index 1 has textures at indices 3,4,5
            if (i >= 3) {meshID = 1;}
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, tree.textures_loaded[i].id);
            glBindVertexArray(tree.meshes[meshID].VAO);
            glDrawElementsInstanced(GL_TRIANGLES, static_cast<unsigned int>(tree.meshes[meshID].indices.size()), GL_UNSIGNED_INT, 0, N_TREES);
            glBindVertexArray(0);
            glActiveTexture(GL_TEXTURE0);
        }
    }
    if (environmentType == "forest" or environmentType == "snow")
    {
        int meshIdx = 1;
        int textureIdx = 1; 
        if (environmentType == "snow")
        {
            meshIdx = 5;
            textureIdx = 0;
        }
        // render only one specific tree (both the forest and snow tree model is made up of multiple trees)
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, tree.textures_loaded[textureIdx].id); 
        glBindVertexArray(tree.meshes[meshIdx].VAO); 
        glDrawElementsInstanced(GL_TRIANGLES, static_cast<unsigned int>(tree.meshes[meshIdx].indices.size()), GL_UNSIGNED_INT, 0, N_TREES);
        glBindVertexArray(0);
    }
    if (environmentType == "night")
    {
        // tree model consists of one meshes and two textures
        for (unsigned int i = 0; i < 2; i++)
        {
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, tree.textures_loaded[i].id); 
            glBindVertexArray(tree.meshes[0].VAO); 
            glDrawElementsInstanced(GL_TRIANGLES, static_cast<unsigned int>(tree.meshes[0].indices.size()), GL_UNSIGNED_INT, 0, N_TREES);
            glBindVertexArray(0); 
        }  
    }
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

void World::drawSkyBox(bool grayscale)
{
    // render skybox
    glDepthFunc(GL_LEQUAL);
    shaderSkybox.use();

    if (grayscale)
    {
        shaderSkybox.setBool("grayscale", true);
    }
    else
    {
        shaderSkybox.setBool("grayscale", false);
    }

    skybox.Draw(shaderSkybox, view, projection);
    glDepthFunc(GL_LESS);
}

void World::drawSurroundings()
{
    // set uniforms 
    shaderModel.use();
    shaderModel.setMat4("projection", projection);
    shaderModel.setMat4("view", view);
    shaderModel.setInt("texture_diffuse1", 0);

    // desert and snow environment have the same rocks, just different colors
    if (environmentType == "desert" or environmentType == "snow")
    {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, surrounding.textures_loaded[0].id);
        glBindVertexArray(surrounding.meshes[0].VAO);
        glDrawElementsInstanced(GL_TRIANGLES, static_cast<unsigned int>(surrounding.meshes[0].indices.size()), GL_UNSIGNED_INT, 0, N_SURROUNDINGS);
        glBindVertexArray(0);
        glActiveTexture(GL_TEXTURE0);
    }
    if (environmentType == "forest")
    {
        // note that not all textures and meshes are used to render the model
        int textureIDs[] = {0, 1, 3, 6, 6, 8};
        int meshIDs[] = {0, 1, 2, 4, 5, 6}; 
        for (unsigned int i = 0; i < 6; i++)
        {
            // draw flowers
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, surrounding.textures_loaded[textureIDs[i]].id);
            glBindVertexArray(surrounding.meshes[meshIDs[i]].VAO);
            glDrawElementsInstanced(GL_TRIANGLES, static_cast<unsigned int>(surrounding.meshes[meshIDs[i]].indices.size()), GL_UNSIGNED_INT, 0, N_SURROUNDINGS);
            glBindVertexArray(0);
            glActiveTexture(GL_TEXTURE0);
        }
    }
    if (environmentType == "night")
    {
        // pumpkin model consists three meshes and one texture
        for (unsigned int i = 0; i < 3; i++)
        {
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, surrounding.textures_loaded[0].id);
            glBindVertexArray(surrounding.meshes[i].VAO);
            glDrawElementsInstanced(GL_TRIANGLES, static_cast<unsigned int>(surrounding.meshes[i].indices.size()), GL_UNSIGNED_INT, 0, N_SURROUNDINGS);
            glBindVertexArray(0);
            glActiveTexture(GL_TEXTURE0);
        }
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

    // define range for x and z axis
    int a = (-TERRAIN_SIZE / 2) + 2;
    int b = (TERRAIN_SIZE / 2) - 2;
    std::uniform_int_distribution<> xzPlane(a, b);

    // create random positions for trees
    for (unsigned int i = 0; i < N_TREES; i++)
    {
        float posX = xzPlane(gen);
        float posZ = xzPlane(gen);
        glm::vec3 vec = {posX, GROUND_Y, posZ};
        treePos.push_back(vec);
    }
}

void World::createSurroundingPositions()
{
    std::random_device rd;  // obtain a random number from hardware
    std::mt19937 gen(rd()); // seed the generator

    // define range for x and z axis
    int a = (-TERRAIN_SIZE / 2) + 2;
    int b = (TERRAIN_SIZE / 2) - 2;
    std::uniform_int_distribution<> xzPlane(a, b);

    // create random positions for surroundings
    for (unsigned int i = 0; i < N_SURROUNDINGS; i++)
    {
        // generate positions until a unique position is found
        glm::vec3 vec;
        do
        {
            vec = {xzPlane(gen), GROUND_Y, xzPlane(gen)};
        } while (std::find(treePos.begin(), treePos.end(), vec) != treePos.end());

        // adjust y value where needed (otherwise some models are too high/low)
        if (environmentType == "desert" or environmentType == "snow")
        {
            vec.y += 0.2;
        }
        if (environmentType == "forest")
        {
            vec.y -= 0.1;
        }

        surroundingPos.push_back(vec);
    }
}

void World::createTreeModelMatrices()
{
    for (unsigned int i = 0; i < N_TREES; i++)
    {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, treePos[i]); // translation
        if (environmentType == "desert")
        {
            model = glm::scale(model, glm::vec3(0.06)); // scale down
        }
        if (environmentType == "forest" or environmentType == "night")
        {
            model = glm::scale(model, glm::vec3(0.4f)); // scale down
        }
        if (environmentType == "snow")
        {
            model = glm::scale(model, glm::vec3(0.9)); // scale down
        }
        treeModelMatrices.push_back(model);
    }
}

void World::createSurroundingModelMatrices()
{
    for (unsigned int i = 0; i < N_SURROUNDINGS; i++)
    {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, surroundingPos[i]); // translation
        float rotAngle = static_cast<float>((rand() % 360)); 
        model = glm::rotate(model, rotAngle, glm::vec3(0.0f, 1.0f, 0.0f)); // add (semi)random rotation
        if (environmentType == "desert" or environmentType == "snow")
        {
            model = glm::scale(model, glm::vec3(0.4)); // scale down
        }
        if (environmentType == "forest")
        {
            model = glm::scale(model, glm::vec3(2.5f)); // scale up
        }
        if (environmentType == "night")
        {
            model = glm::scale(model, glm::vec3(1.45f)); // scale up
        }
        surroundingModelMatrices.push_back(model);
    }
}

void World::setupInstancedArray(Model &model, unsigned int buffer, std::vector<glm::mat4> modelMatrices, int amount)
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