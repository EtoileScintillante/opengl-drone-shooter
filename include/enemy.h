/**
 * enemy.h
 *
 * Created by EtoileScintillante.
 */

#ifndef __ENEMY_H__
#define __ENEMY_H__

#include "world.h"
#include "ray.h"
#include "box.h"

class Enemy
{
public:
    static const float MAX_FLOAT_HEIGHT;  // maximum floating height of enemy, measured from y = 0
    static const float MIN_FLOAT_HEIGHT;  // minimum floating height of enemy, measured from y = 0
    std::vector<glm::vec3> treePositions; // tree positions (needed to check if enemy spawns inside a tree)
    bool isDead;                          // is enemy dead?
    Model drone;                          // enemy model (in this program it's a drone)
    Shader shader;                        // enemy shader (must include geometry shader for explosion effect)
    glm::mat4 projection;                 // projection matrix
    glm::mat4 view;                       // camera view matrix
    float currentFrame;                   // current frame/time
    float deltaTime;                      // time passed between two frames

    /// default constructor.
    Enemy();

    /// @brief creates Enemy object.
    /// @param treePos vector containing tree positions (World attribute).
    Enemy(std::vector<glm::vec3> treePos);
    
    /// spawns the enemy.
    void spawn();

    /**
     * @brief detects whether the enemy has been hit by a bullet (using ray-box intersection).
     * Info from https://www.scratchapixel.com/lessons/3d-basic-rendering/minimal-ray-tracer-rendering-simple-shapes/ray-box-intersection.
     *
     * @param bulletStartPos starting position of the bullet (in this program that is the camera position).
     * @param bulletDir direction of the bullet (in this program that is the front vector of the camera).
     * @param bulletRange range of bullet (maximum distance the bullet can travel).
     */
    void collisionDetection(glm::vec3 bulletStartPos, glm::vec3 bulletDir, float bulletRange);

    /// make enemy explode when it gets shot (using the geometry shader).
    void dyingAnimation();

private:
    glm::vec3 position;    // position of enemy
    glm::mat4 modelMatrix; // model matrix for enemy
    float deathTime;       // used to control the duration of the dying animation

    /// generates random position for enemy.
    void generatePosition();

    /// generate nodel matrix for enemy.
    void generateModelMatrix();

    /// checks whether a given position is also a position of a tree.
    bool spawnsInTree(glm::vec3 position);
};

#endif /*__ENEMY__*/