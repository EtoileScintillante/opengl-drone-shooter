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
    static const float TERRAIN_SIZE;     // terrain size (it's a square)
    static const float GROUND_Y;         // y level of ground
    // shader related
    glm::mat4 view;       // camera view matrix
    glm::mat4 projection; // projection matrix

    /// constructs a new world object.
    World();

    /// renders the terrain
    void Draw();

    /// returns the tree positions
    std::vector<glm::vec3> getTreePositions() const;

private:
    // objects & shaders
    Shader shaderTree;   // tree model shader
    Shader shaderGround; // ground shader
    Shader shaderSkybox; // skybox shader
    Model tree;          // tree model
    SkyBox skybox;       // skybox
    // object related
    std::vector<glm::vec3> treePos;    // tree positions
    std::vector<float> skyboxVertices; // skybox vertex data
    std::vector<float> groundVertices; // ground vertex data
    unsigned int groundTexture;
    unsigned int groundVAO, groundVBO; // buffers for ground data

    /// sets up all the objects so that they can be rendered.
    void setupWorld();

    /// renders trees
    void drawTrees();

    /// renders ground
    void drawGround();

    /// renders skybox
    void drawSkyBox();
    
    /// returns the skybox vertex data.
    std::vector<float> getSkyboxVertexData();

    /// returns vertex data for ground. Data includes positions and texture coords.
    std::vector<float> getGroundVertexData();

    /// creates N_TREES random positions.
    void createTreePositions();
};

#endif