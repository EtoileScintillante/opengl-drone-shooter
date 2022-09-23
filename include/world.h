/**
 * world.h
 *
 * Created by EtoileScintillante.
 */

#ifndef __WORLD_H__
#define __WORLD_H__

#include "shader.h"
#include "model.h"
#include "skybox.h"

class World
{
public:
    // terrain settings
    static const unsigned int N_TREES;   // number of trees
    static const unsigned int N_FLOWERS; // number of flowers
    static const float TERRAIN_SIZE;     // terrain size (it's a square with (x, z) = (0, 0) as center)
    static const float GROUND_Y;         // y level of ground
    // shader related
    glm::mat4 view;       // camera view matrix
    glm::mat4 projection; // projection matrix

    /// constructs a new world object.
    World();

    /// destructor (frees allocated memory).
    ~World();

    /// renders the terrain.
    void Draw();

    /// returns the tree positions
    std::vector<glm::vec3> getTreePositions() const;

private:
    // objects & shaders
    Shader shaderModel;  // model shader
    Shader shaderGround; // ground shader
    Shader shaderSkybox; // skybox shader
    Model tree;          // tree model
    Model flowers;       // flower model
    SkyBox skybox;       // skybox
    // object related
    std::vector<glm::vec3> treePos;    // tree positions
    std::vector<glm::vec3> flowerPos;  // flower positions
    std::vector<float> groundVertices; // ground vertex data
    unsigned int groundTexture;        // ground texture
    unsigned int groundVAO, groundVBO; // buffers for ground data
    unsigned int treeBuffer;           // buffer for tree models
    unsigned int flowerBuffer;         // buffer for flower models
    // matrix data (used for instancing)
    glm::mat4 *treeModelMatrices = new glm::mat4[N_TREES];     // tree model matrices
    glm::mat4 *flowerModelMatrices = new glm::mat4[N_FLOWERS]; // flower model matrices

    /// sets up all the objects so that they can be rendered.
    void setupWorld();

    /// renders trees.
    void drawTrees();

    /// renders ground.
    void drawGround();

    /// renders skybox.
    void drawSkyBox();

    /// renders flowers.
    void drawFlowers();

    /// returns vertex data for ground. Data includes positions and texture coords.
    std::vector<float> getGroundVertexData();

    /// creates N_TREES random positions.
    void createTreePositions();

    /// creates flower positions (half of trees will be surrounded by four flower models).
    void createFlowerPositions();

    /// create model matrices for the trees.
    void createTreeModelMatrices();

    /// create model matrices for the flowers.
    void createFlowerModelMatrices();

    /**
     * @brief setup instanced array for a model.
     * 
     * @param[out] model model.
     * @param buffer instanced array buffer.
     * @param modelMatrices pointer to an array containing the model matrices for the instances of the model.
     * @param amount number of instances.
     */
    void setupInstancedArray(Model &model, unsigned int buffer, glm::mat4 *modelMatrices, int amount);
};

#endif