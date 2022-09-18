/**
 * world.h
 *
 * Created by EtoileScintillante.
 */

#ifndef __WORLD_H__
#define __WORLD_H__

#include "tree.h"
#include "ground.h"
#include "glowstone.h"
#include "mobs.h"
#include "vertex_data.h"

class World {
public:
    // terrain settings
    static const unsigned int N_GLOWSTONES;   // number of glowstones
    static const unsigned int N_TREES;        // number of trees
    static const unsigned int TERRAIN_SIZE;   // actual terrain size = TERRAIN_SIZE * TERRAIN_SIZE (it's a square)
    static const unsigned int HEIGHT_TREE;    // height of tree (amount of blocks that make up a trunk)
    static const float MIN_HEIGHT_MOB;        // minimum floating height of mobs
    static const float MAX_HEIGHT_MOB;        // maximum floating height of mobs
    static const float HEIGHT_GLOWSTONES;     // height of glow stone (counted from GROUND_Y + BLOCK_SIZE)
    static const float BLOCK_SIZE;            // size of the blocks (initially they all have the same size)
    static const float GROUND_Y;              // y level of ground
    // objects & shaders
    Ground ground;            // ground object
    Tree trees;               // tree obect
    GlowStone glowstones;     // glow stone object
    Mobs mobs;                // mobs object
    Shader shader;            // shader for all non-transparent blocks
    Shader shaderTransparent; // shader for transparent blocks (only leaves)

    /// constructs a new world object.
    World();

    /**
     * @brief draws the world (ground + trees + glow stones + mobs)
     * 
     * @param cameraView camera view matrix.
     * @param projection projection matrix.
     * @param currentTime current time/frame.
     */
    void Draw(glm::mat4 cameraView, glm::mat4 projection, float currentTime);

private:
    // vertex data
    std::vector< Vertex > trunkVertices;      // vertex data needed for rendering trunk blocks
    std::vector< Vertex > leavesVertices;     // vertex data needed for rendering leaves blocks
    std::vector< Vertex > dirtVertices;       // vertex data needed for rendering dirt blocks
    std::vector< Vertex > glowStoneVertices;  // vertex data needed for rendering glow stone blocks
    std::vector< Vertex > stoneVertices;      // vertex data needed for rendering stone blocks
    std::vector< Vertex > creeperVertices;    // vertex data needed for rendering creeper head blocks
    std::vector< Vertex > zombieVertices;     // vertex data needed for rendering zombie head blocks
    // textures
    std::string texturePath;      // path to where all the texture files are located
    std::string pathTrunkDirtTex; // filename of trunk and dirt blocks texture 
    std::string pathLeavesTex;    // filename of leaves block texture
    std::string pathGlowStoneTex; // filename of glow stone block texture
    std::string pathStoneTex;     // filename of stone block texture
    std::string pathMobsTex;      // filename of mob block texture
    
    /// sets up all the objects so that they can be rendered.
    void setupWorld();

    /// sets up vertex data for all the blocks.
    void setupData();
};

#endif