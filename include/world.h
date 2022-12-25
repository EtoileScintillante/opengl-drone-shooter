/**
 * world.h
 *
 * This file contains a World class.
 * The world consists of four objects: ground, trees, flowers and a skybox.
 * 
 * Created by EtoileScintillante.
 */

#ifndef __WORLD_H__
#define __WORLD_H__

#include <random>

#include "shader.h"
#include "model.h"
#include "skybox.h"

class World
{
public:
    // terrain settings
    static const unsigned int N_TREES;   // number of trees
    static const unsigned int N_FLOWERS; // number of flowers
    static const float TERRAIN_SIZE;     // terrain size; it's a square with (x, z) = (0, 0) as center
    static const float GROUND_Y;         // y level of ground
    // matrices
    glm::mat4 view;       // camera view matrix
    glm::mat4 projection; // projection matrix

    /// Default constructor; constructs a new world object.
    World();

    /**
     * @brief Renders the world (ground, trees, flowers and skybox).
     * 
     * @param view view matrix.
     * @param projection projection matrix.
     */
    void Draw(glm::mat4 view, glm::mat4 projection);

    /**
     * @brief Renders skybox.
     * 
     * @param grayscale add grayscale filter if true.
     */
    void drawSkyBox(bool grayscale);

    /// Returns the tree positions.
    std::vector<glm::vec3> getTreePositions() const;

private:
    // objects and shaders
    Shader shaderModel;  // shader for flower and tree models
    Shader shaderGround; // ground shader
    Shader shaderSkybox; // skybox shader
    Model tree;          // tree model
    Model flowers;       // flower model
    SkyBox skybox;       // skybox
    // object related attributes
    std::vector<glm::vec3> treePos;    // tree positions
    std::vector<glm::vec3> flowerPos;  // flower positions
    std::vector<float> groundVertices; // ground vertex data
    unsigned int groundTexture;        // ground texture
    unsigned int groundVAO, groundVBO; // buffers for ground data
    unsigned int treeBuffer;           // buffer for tree models
    unsigned int flowerBuffer;         // buffer for flower models
    // matrix data (used for instancing)
    std::vector<glm::mat4> treeModelMatrices;   // tree model matrices
    std::vector<glm::mat4> flowerModelMatrices; // flower model matrices    

    /// Sets up all the objects so that they can be rendered.
    void setupWorld();

    /// Renders trees.
    void drawTrees();

    /// Renders ground.
    void drawGround();

    /// Renders flowers.
    void drawFlowers();

    /// Returns vertex data for ground. Data includes positions and texture coords.
    std::vector<float> getGroundVertexData() const;

    /// Creates N_TREES random positions.
    void createTreePositions();

    /// Creates flower positions (half of trees will be surrounded by four flower models).
    void createFlowerPositions();

    /// Creates model matrices for the trees.
    void createTreeModelMatrices();

    /// Creates model matrices for the flowers.
    void createFlowerModelMatrices();

    /**
     * @brief Sets up an instanced array for a model.
     * 
     * @param model Model object.
     * @param buffer instanced array buffer.
     * @param modelMatrices vector containing the model matrices.
     * @param amount number of instances.
     */
    void setupInstancedArray(Model &model, unsigned int buffer, std::vector<glm::mat4> modelMatrices, int amount);
};

#endif /*__WORLD__*/