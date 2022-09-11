#include "world.h"

const unsigned int World::N_GLOWSTONES = 8;   
const unsigned int World::N_TREES = 15;        
const unsigned int World::TERRAIN_SIZE = 30;  
const unsigned int World::HEIGHT_TREE = 5;    
const float World::HEIGHT_GLOWSTONES = 3.0f;  
const float World::MIN_HEIGHT_MOB = 2.0f;
const float World::MAX_HEIGHT_MOB = 6.0f;   
const float World::BLOCK_SIZE = 1.0f;            
const float World::GROUND_Y = -1.8f;

World::World()
{
   setupWorld();
}

void World::Draw(glm::mat4 cameraView, glm::mat4 projection, float currentTime)
{
    trees.Draw(shader, shaderTransparent, cameraView, projection);
    glowstones.Draw(shader, cameraView,projection);
    ground.Draw(shader, cameraView, projection);
    mobs.Spawn(shader, currentTime, cameraView, projection);
}

void World::setupWorld()
{
    // setup shaders
    shader = Shader("shaders/blocks.vert", "shaders/blocks.frag");
    shaderTransparent = Shader("shaders/blocks.vert", "shaders/transparent.frag");

    // paths to the block textures
    texturePath = "resources/textures/";
    pathTrunkDirtTex = texturePath + "blocks.JPG";
    pathLeavesTex = texturePath + "leaves.png";
    pathGlowStoneTex = texturePath + "glowstone.jpg";
    pathStoneTex = texturePath + "stone.jpg";
    pathMobsTex = texturePath + "mobs.JPG";

    // setup vertex data
    setupData();

    // setup objects
    trees = Tree(trunkVertices, leavesVertices, pathTrunkDirtTex, pathLeavesTex, N_TREES, HEIGHT_TREE, TERRAIN_SIZE, GROUND_Y, BLOCK_SIZE);
    ground = Ground(dirtVertices, stoneVertices, pathTrunkDirtTex, pathStoneTex, TERRAIN_SIZE, GROUND_Y);
    mobs = Mobs(zombieVertices, creeperVertices, trees.treePositions, pathMobsTex, TERRAIN_SIZE, GROUND_Y, BLOCK_SIZE, MIN_HEIGHT_MOB, MAX_HEIGHT_MOB);
    glowstones = GlowStone(glowStoneVertices, pathGlowStoneTex, trees.treePositions, N_GLOWSTONES, HEIGHT_GLOWSTONES);
}

void World::setupData()
{
    std::vector<float> positionData = getPositionData();       // position data for blocks
    std::vector<float> textureCoords = getTextureCoordsData(); // texture coordinates for blocks

    int indexPos = 0;
    int indexTex = 0;
    for (unsigned i = 0; i < getPositionData().size(); i++) // 36 lines of position data (all objects, except model(s) are just blocks)
    {
        Data vertexTrunk, vertexLeaves, vertexDirt, vertexCreeper, vertexZombie;
        // all blocks have the same positon data (leaves, glow stone and stone also have same texture coords data)
        vertexTrunk.Position = vertexLeaves.Position = vertexDirt.Position = vertexCreeper.Position = vertexZombie.Position = glm::vec3(positionData[indexPos], positionData[indexPos + 1], positionData[indexPos + 2]);
        // a line of texture coords data in textureCoords[] looks like: trunk.x, trunk.y, dirt.x, dirt.y, leaves.x, leaves.y (0, 1, 2, 3, 4, 5)
        vertexTrunk.TexCoords = glm::vec2(textureCoords[indexTex], textureCoords[indexTex + 1]);
        vertexLeaves.TexCoords = glm::vec2(textureCoords[indexTex + 4], textureCoords[indexTex + 5]);
        vertexDirt.TexCoords = glm::vec2(textureCoords[indexTex + 2], textureCoords[indexTex + 3]);
        vertexCreeper.TexCoords = glm::vec2(textureCoords[indexTex + 6], textureCoords[indexTex + 7]);
        // zombie tex coords are almost the same as creeper tex coords, the only difference is that each y in zombie tex coords is 0.5f lower than y in creeper tex coords
        vertexZombie.TexCoords = glm::vec2(textureCoords[indexTex + 6], textureCoords[indexTex + 7] - 0.5f); // subtract 0.5f from y value
        // push data into vectors
        trunkVertices.push_back(vertexTrunk);
        leavesVertices.push_back(vertexLeaves);
        dirtVertices.push_back(vertexDirt);
        glowStoneVertices.push_back(vertexLeaves);
        stoneVertices.push_back(vertexLeaves);
        creeperVertices.push_back(vertexCreeper);
        zombieVertices.push_back(vertexZombie);
        // move to the next line in the arrays
        indexPos += 3;
        indexTex += 8;
    }
}