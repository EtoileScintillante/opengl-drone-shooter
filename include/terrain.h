/*                                  
 * terrain.h
 *
 * Created by: EtoileScintillante
 * 
 * This file contains functions that help setup a flat terrain made out of blocks, including some trees and lamps.
 */

#ifndef TERRAIN_H
#define TERRAIN_H

#include <glm/glm.hpp>
#include <vector>
#include <random>

/**
 * @brief generate random positions for the trees on a flat terrains.
 * 
 * @param numTrees number of trees for which a position will be generated.
 * @param terrainSize size of terrain (range goes from -terrainSize to terrainSize).
 * @param groundY y value of the ground.
 * @param blockSize blocksize. This will be added to groundY in order to place the tree on the ground.
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
 * @param terrainSize size of terrain (range goes from -terrainSize to terrainSize).
 * @return std::vector< glm::vec3 > vector containing all the positions.
 */
std::vector< glm::vec3 > generateRandomLampPositions(int numLamps, float heightLowerRange, float heightUpperRange, int terrainSize);

/**
 * @brief generate the positions for lamps in such a way that the lamp sticks to a tree.
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
 * the lamp will be partly rendered in the tree). Every third iteration, offsetX is subtracted from the x value of the tree
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
 * @param blockSize blocksize. This will be added or subtracted to x/y/z values of tree.
 * If blockSize is smaller than blocksize of tree, the leaves will be partly rendered in the tree. 
 * @param groundY y value of the ground.
 * @param offset this offset will be added or subtracted to x/y/z values of tree to prevent z fighting.
 * @param treePositions vector containing the positions of the trees.
 * @return std::vector< glm::vec3 > vector containing all the positions.
 */
std::vector< glm::vec3 > generateLeavesPositions(int heightTree, float blockSize, float groundY, float offset, std::vector< glm::vec3 > treePositions);

#endif