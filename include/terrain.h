/*                                  
 * terrain.h
 *
 * Created by: EtoileScintillante
 * 
 * This file contains functions that help setup a flat terrain made out of blocks, 
 * including some trees and glowstones attached to some of those trees.
 */

#ifndef TERRAIN_H
#define TERRAIN_H

#include <glm/glm.hpp>
#include <vector>
#include <random>

/**
 * @brief generates random positions for trees on a flat terrain.
 * 
 * @param numTrees number of trees for which a position will be generated.
 * @param terrainSize terrain size. Note: the actual size of the terrain is calculated by terrainSize * terrainSize
 * (the terrain is shaped like a square), the input variable here should correspond to the amount of blocks that make up one side of the square).
 * @param groundY y value of the ground.
 * @param blockSize blocksize. this value will be added to groundY in order to place the tree on the ground.
 * @param offsetY this value will be added to groundY and blockSize to avoid z fighting.
 * @return std::vector< glm::vec3 > vector containing all the positions.
 */
std::vector< glm::vec3 > generateTreePositions(int numTrees, int terrainSize, float groundY, float blockSize, float offsetY);

/**
 * @brief generates random positions for lamps. 
 * 
 * @param numLamps number of lamps.
 * @param heightLowerRange lowest y value possible for position of lamp.
 * @param heightUpperRange highest y value possible for position of lamp.
 * @param terrainSize terrain size. Note: the actual size of the terrain is calculated by terrainSize * terrainSize
 * (the terrain is shaped like a square), the input variable here should correspond to the amount of blocks that make up one side of the square).
 * @return std::vector< glm::vec3 > vector containing all the positions.
 */
std::vector< glm::vec3 > generateRandomLampPositions(int numLamps, float heightLowerRange, float heightUpperRange, int terrainSize);

/**
 * @brief generates positions for lamps in such a way that the lamps stick to the side of the trees.
 * This is done in two steps: 
 * 1. randomly choose numLamps tree positions from the treePosition vector.
 * 2. create position for lamp based on position of tree by adding/subtracting offsets.
 * 
 * @param numLamps number of lamps.
 * @param heightTree height of tree (number of blocks that make up a tree).
 * @param groundY y value of the ground.
 * @param blockSize blocksize. This will be added to groundY.
 * @param offsetY this value will be subtracted from groundY + blockSize + heightTree, creating a y value for the position of the lamps.
 * @param offsetX this value will be added to the x value of the tree position (if offsetX is smaller than blocksize of tree, 
 * the lamp will be partly rendered inside the tree). Every third iteration, offsetX is subtracted from the x value of the tree
 * position to create some variation in the direction the lamps are pointing at. 
 * @param treePositions vector containing the positions of the trees. 
 * @return std::vector< glm::vec3 > vector containing the positions.
 */
std::vector< glm::vec3 > generateLampPosStickToTree(int numLamps, int heightTree, float groundY, float blockSize, float offsetY, float offsetX, std::vector< glm::vec3 > treePositions);

/**
 * @brief generates positions for the blocks that make up the leaves of a tree.
 * The idea is simple: every tree has 5 leaves blocks attached to it. 4 at the sides (-x, x, -z, z), attached to the
 * highest block of tree and 1 on top of this highest block. 
 * 
 * @param heightTree height of tree (number of blocks that make up a tree).
 * @param blockSize blocksize. this value will be added to (or subtracted from) the x/y/z values of tree.
 * If blockSize is smaller than blocksize of tree, the leaves will be partly rendered inside the tree. 
 * @param groundY y value of the ground.
 * @param offset this offset will be added to (or subtracted from) the x/y/z values of tree to prevent z fighting.
 * @param treePositions vector containing the positions of the trees.
 * @return std::vector< glm::vec3 > vector containing all the positions.
 */
std::vector< glm::vec3 > generateLeavesPositions(int heightTree, float blockSize, float groundY, float offset, std::vector< glm::vec3 > treePositions);

/**
 * @brief creates a vector containing 1s and 2s in a random order. This function helps
 * with creating a ground that is made up two types of blocks, so that there
 * is no need to hard code an order in which the blocks are rendered. 
 * 
 * @param terrainSize terrain size. Note: the actual size of the terrain is calculated by terrainSize * terrainSize
 * (the terrain is shaped like a square), the input variable here should correspond to the amount of blocks that make up one side of the square).
 * @return std::vector < int > containing 1s and 2s.
 */
std::vector < int > dirtStonePositions(int terrainSize);

#endif